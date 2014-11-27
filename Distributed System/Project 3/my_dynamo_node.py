import copy
import logging
import cPickle as pickle
import pynamo.logconfig as logconfig

import leveldb
import zerorpc
import gevent
import sys
import pynamo.dynamo as dynamo
from pynamo.dynamomessages import ClientPut, ClientGet
from pynamo.dynamomessages import PutReq, PutRsp
from pynamo.dynamomessages import GetReq, GetRsp
from pynamo.dynamomessages import PingReq, PingRsp
from pynamo.dynamomessages import ClientPutRsp, ClientGetRsp
from pynamo.hash_multiple import ConsistentHashTable
from pynamo.merkle import MerkleTree
from pynamo.vectorclock import VectorClock

logconfig.init_logging()
_logger = logging.getLogger('dynamo')


class MyDynamoNode(object):
    def __init__(self, addr, master):
        self.addr = addr
        self.master = master
        self.members = [self.addr]

        self.N = 3
        self.W = 2
        self.R = 2
        self.T = 1

        self.next_sequence_number = 0
        self.register()
        # self.local_store = MerkleTree()  # key => (value, metadata)
        self.local_store = leveldb.LevelDB(self.addr)
        self.pending_put_rsp = {}  # seqno => set of nodes that have stored
        self.pending_put_msg = {}  # seqno => original client message
        self.pending_get_rsp = {}  # seqno => set of (node, value, metadata) tuples
        self.pending_get_msg = {}  # seqno => original client message
        # seqno => set of requests sent to other nodes, for each message class
        self.pending_req = {PutReq: {}, GetReq: {}}
        self.failed_nodes = []
        self.pending_handoffs = {}
        self.chash = ConsistentHashTable(self.members, self.T)

    def generate_sequence_number(self):
        """Generate next sequence number for this Node"""
        self.next_sequence_number = self.next_sequence_number + 1
        return self.next_sequence_number

    def send(self, msg, to_node):
        msg_str = pickle.dumps(msg)
        c = zerorpc.Client(timeout=10)
        c.connect("tcp://" + to_node)

        try:
            c.rcvmsg(msg_str)
            c.close()
            return True
        except zerorpc.TimeoutExpired:
            # print "Connect to %s timeout" % to_node
            c.close()
            if not isinstance(msg, ClientPutRsp) or not isinstance(msg, ClientGetRsp):
                self.failed_nodes.append(to_node)
            return False


    def register(self):
        '''
        New added Dynamo node register itself to the ring via master.
        '''
        c = zerorpc.Client(timeout=2)
        c.connect("tcp://" + self.master)
        try:
            members = c.join_in(self.addr)
            for addr in members:
                self.members.append(addr)
        except zerorpc.TimeoutExpired:
            print "connect to leader timeout"
        c.close()
        self.chash = ConsistentHashTable(self.members, self.T)

    def notify(self, new_addr):
        '''
        Get notified of new added Dynamo node from master.
        '''
        self.members.append(new_addr)
        self.chash = ConsistentHashTable(self.members, self.T)

    def run(self):
        gevent.spawn(self.retry_failed_node)
        while True:
            gevent.sleep(2)

            # PART retry_failed_node

    def retry_failed_node(self):  # Permanently repeating timer
        while True:
            gevent.sleep(2)
            if self.failed_nodes:
                node = self.failed_nodes.pop(0)
                # Send a test message to the oldest failed node
                pingmsg = PingReq(self.addr, node)
                self.send(pingmsg, node)

    def rcv_pingreq(self, pingmsg):
        # Always reply to a test message
        pingrsp = PingRsp(pingmsg)
        self.send(pingrsp, pingrsp.to_node)

    def rcv_pingrsp(self, pingmsg):
        # Remove all instances of recovered node from failed node list
        recovered_node = pingmsg.from_node
        while recovered_node in self.failed_nodes:
            self.failed_nodes.remove(recovered_node)
        if recovered_node in self.pending_handoffs:
            for key in self.pending_handoffs[recovered_node]:
                # Send our latest value for this key
                (value, metadata) = self.retrieve(key)
                putmsg = PutReq(self.addr, recovered_node, key, value, metadata)
                self.send(putmsg, recovered_node)
            del self.pending_handoffs[recovered_node]

    def rcvmsg(self, msg_str):
        msg = pickle.loads(msg_str)
        if isinstance(msg, ClientPut):
            # print "receive clientput"
            return self.rcv_clientput(msg)
        elif isinstance(msg, PutReq):
            # print "receive putreq"
            return self.rcv_put(msg)
        elif isinstance(msg, PutRsp):
            # print "receive putrsp"
            return self.rcv_putrsp(msg)
        elif isinstance(msg, ClientGet):
            # print "receive clientget"
            return self.rcv_clientget(msg)
        elif isinstance(msg, GetReq):
            # print "receive getreq"
            return self.rcv_get(msg)
        elif isinstance(msg, GetRsp):
            # print "receive getrsp"
            return self.rcv_getrsp(msg)
        elif isinstance(msg, PingReq):
            # print "receive pringreq"
            return self.rcv_pingreq(msg)
        elif isinstance(msg, PingRsp):
            # print "receive pingrsp"
            return self.rcv_pingrsp(msg)
        else:
            raise TypeError("Unexpected message type %s", msg.__class__)


            # PART rcv_clientput

    def rcv_clientput(self, msg):
        preference_list, avoided = self.chash.find_nodes(msg.key, self.N,
                                                         self.failed_nodes)
        non_extra_count = self.N - len(avoided)
        # Determine if we are in the list
        if self.addr not in preference_list:
            # Forward to the coordinator for this key
            _logger.info("put(%s=%s) maps to %s", msg.key, msg.value,
                         preference_list)
            coordinator = preference_list[0]
            if not self.send(msg, coordinator):
                self.rcv_clientput(msg)
                # print "send to coordinator %s" % coordinator
        else:
            # Use an incrementing local sequence number to distinguish
            # multiple requests for the same key
            seqno = self.generate_sequence_number()
            _logger.info("%s, %d: put %s=%s", self.addr, seqno, msg.key,
                         msg.value)
            # The metadata for a key is passed in by the client, and updated by the coordinator node.
            metadata = copy.deepcopy(msg.metadata)
            metadata = metadata.update(self.addr, seqno)
            # Send out to preference list, and keep track of who has replied
            # self.pending_req[PutReq][seqno] = set()
            self.pending_put_rsp[seqno] = set()
            self.pending_put_msg[seqno] = msg
            put_rsp = 0
            reqcount = 0
            for ii, node in enumerate(preference_list):
                if ii >= non_extra_count:
                    # This is an extra node that's only include because of a failed node
                    handoff = avoided
                else:
                    handoff = None
                # Send message to get node in preference list to store
                putmsg = PutReq(self.addr, node, msg.key, msg.value,
                                metadata, msg_id=seqno, handoff=handoff)
                if self.send(putmsg, node):
                    put_rsp += 1
                reqcount = reqcount + 1
                if reqcount >= self.N:
                    # preference_list may have more than N entries to allow for failed nodes
                    break
            if put_rsp < self.W - 1:
                self.rcv_clientput(msg)

                # PART rcv_clientget

    def rcv_clientget(self, msg):
        preference_list = self.chash.find_nodes(msg.key, self.N,
                                                self.failed_nodes)[0]
        # print "client get:%s" % preference_list
        # Determine if we are in the list
        if self.addr not in preference_list:
            # Forward to the coordinator for this key
            _logger.info("get(%s=?) maps to %s", msg.key, preference_list)
            coordinator = preference_list[0]
            self.send(msg, coordinator)
        else:
            seqno = self.generate_sequence_number()
            # self.pending_req[GetReq][seqno] = set()
            self.pending_get_rsp[seqno] = set()
            self.pending_get_msg[seqno] = msg
            reqcount = 0
            get_rsp = 0
            for node in preference_list:
                getmsg = GetReq(self.addr, node, msg.key, msg_id=seqno)
                #self.pending_req[GetReq][seqno].add(getmsg)
                if self.send(getmsg, node):
                    get_rsp += 1
                reqcount = reqcount + 1
                if reqcount >= self.N:
                    # preference_list may have more than N entries to allow for failed nodes
                    break
            if get_rsp < self.R - 1:
                self.rcv_clientget(msg)
                pass

                # PART rcv_put

    def rcv_put(self, putmsg):
        _logger.info("%s: store %s=%s", self.addr, putmsg.key, putmsg.value)
        self.store(putmsg.key, putmsg.value, putmsg.metadata)
        if putmsg.handoff is not None:
            for failed_node in putmsg.handoff:
                self.failed_nodes.append(failed_node)
                if failed_node not in self.pending_handoffs:
                    self.pending_handoffs[failed_node] = set()
                self.pending_handoffs[failed_node].add(putmsg.key)
        putrsp = PutRsp(putmsg)
        self.send(putrsp, putrsp.to_node)

    # PART rcv_putrsp
    def rcv_putrsp(self, putrsp):
        seqno = putrsp.msg_id
        if seqno in self.pending_put_rsp:
            self.pending_put_rsp[seqno].add(putrsp.from_node)
            if len(self.pending_put_rsp[seqno]) >= self.W:
                _logger.info("%s: written %d copies of %s=%s so done",
                             self.addr, self.W, putrsp.key, putrsp.value)
                _logger.debug("  copies at %s", [node for node in
                                                 self.pending_put_rsp[seqno]])
                # Tidy up tracking data structures
                original_msg = self.pending_put_msg[seqno]
                # del self.pending_req[PutReq][seqno]
                del self.pending_put_rsp[seqno]
                del self.pending_put_msg[seqno]
                # Reply to the original client
                client_putrsp = ClientPutRsp(original_msg, putrsp.metadata)
                self.send(client_putrsp, client_putrsp.to_node)
        else:
            pass  # Superfluous reply

            # PART rcv_get

    def rcv_get(self, getmsg):
        _logger.info("%s: retrieve %s=?", self.addr, getmsg.key)
        (value, metadata) = self.retrieve(getmsg.key)
        getrsp = GetRsp(getmsg, value, metadata)
        self.send(getrsp, getrsp.to_node)

    # PART rcv_getrsp
    def rcv_getrsp(self, getrsp):
        seqno = getrsp.msg_id
        if seqno in self.pending_get_rsp:
            self.pending_get_rsp[seqno].add((getrsp.from_node, getrsp.value, getrsp.metadata))
            if len(self.pending_get_rsp[seqno]) >= self.R:
                _logger.info("%s: read %d copies of %s=? so done",
                             self.addr, self.R, getrsp.key)
                _logger.debug("  copies at %s", [(node, value) for
                                                 (node, value, _) in
                                                 self.pending_get_rsp[seqno]])
                # Coalesce all compatible (value, metadata) pairs across the responses
                results = VectorClock.coalesce2([(value, metadata) for
                                                 (node, value, metadata) in
                                                 self.pending_get_rsp[seqno]])
                # Tidy up tracking data structures
                original_msg = self.pending_get_msg[seqno]
                # del self.pending_req[GetReq][seqno]
                del self.pending_get_rsp[seqno]
                del self.pending_get_msg[seqno]
                # Reply to the original client, including all received values
                client_getrsp = ClientGetRsp(original_msg,
                                             [value for (value, metadata) in results],
                                             [metadata for (value, metadata) in results])
                self.send(client_getrsp, client_getrsp.to_node)
        else:
            pass  # Superfluous reply

            # PART get_contents

    def get_contents(self):
        results = []
        # for key, value in self.local_store.items():
        # results.append("%s:%s" % (key, value[0]))
        for key, value_str in self.local_store.RangeIter():
            (value, metadata) = pickle.loads(value_str)
            results.append("%s:%s" % (key, value))
        return results

    # PART storage
    def store(self, key, value, metadata):
        # self.local_store[key] = (value, metadata)
        value_str = pickle.dumps((value, metadata))
        self.local_store.Put(key, value_str)

        print "I am %s" % self.addr
        print "My content: \n%s" % "\n".join(self.get_contents())

    def retrieve(self, key):
        # if key in self.local_store:
        # return self.local_store[key]
        #else:
        #    return (None, None)
        try:
            value_str = self.local_store.Get(key)
            (value, metadata) = pickle.loads(value_str)
            return (value, metadata)
        except:
            return (None, None)

    pass


if __name__ == "__main__":
    addr = sys.argv[1]
    master = sys.argv[2]
    my_dynamo_node = MyDynamoNode(addr, master)
    s = zerorpc.Server(my_dynamo_node)
    s.bind("tcp://" + addr)
    gevent.spawn(my_dynamo_node.run)
    print "Start dynamo node %s" % addr
    s.run()
    
    

