import sys
import zerorpc
import gevent
import random
import cPickle as pickle
from pynamo.vectorclock import VectorClock
from pynamo.dynamomessages import ClientPut, ClientPutRsp
from pynamo.dynamomessages import ClientGet, ClientGetRsp


class MyDynamoClientNode(object):
    def __init__(self, addr, master):
        self.addr = addr
        self.master = master
        # gevent.spawn(self.run)
        # self.run()

    def send(self, msg, to_node):
        msg_str = pickle.dumps(msg)
        c = zerorpc.Client(timeout=10)
        c.connect("tcp://" + to_node)
        retry = 1
        # while retry:
        try:
            c.rcvmsg(msg_str)
            return True
            # break
        except zerorpc.TimeoutExpired:
            print "Connect to %s timeout" % to_node
            return False
        # retry -= 1
        c.close()


    def run(self):
        while True:
            gevent.sleep(1)

    def get_dest_node(self):
        c = zerorpc.Client(timeout=5)
        c.connect("tcp://" + self.master)
        retry = 3
        while retry:
            try:
                members = c.get_members()
                destnode = random.choice(members)
                break
            except zerorpc.TimeoutExpired:
                print "Connect to master timeout"
            retry -= 1
        if retry == 0:
            print "Connect to master failed"
        c.close()
        return destnode

    def put(self, key, metadata, value, destnode=None):
        if destnode is None:  # Pick a random node to send the request to
            # destnode = random.choice(DynamoNode.nodelist)
            destnode = self.get_dest_node()
        # Input metadata is always a sequence, but we always need to insert a
        # single VectorClock object into the ClientPut message
        if len(metadata) == 1 and metadata[0] is None:
            metadata = VectorClock()
        else:
            # A Put operation always implies convergence
            metadata = VectorClock.converge(metadata)
        putmsg = ClientPut(self.addr, destnode, key, value, metadata)
        # putmsg = [self.addr, destnode, key, value, metadata]
        # Framework.send_message(putmsg)
        if not self.send(putmsg, destnode):
            #self.put(key, [None], value)
            pass
            #self.send(putmsg, destnode, "ClientPut")
            #return putmsg

    def get(self, key, destnode=None):
        if destnode is None:
            destnode = self.get_dest_node()
        getmsg = ClientGet(self.addr, destnode, key)
        self.send(getmsg, destnode)

    def rcvmsg(self, msg_str):
        msg = pickle.loads(msg_str)
        if isinstance(msg, ClientPutRsp):
            print "Put successfully"
        if isinstance(msg, ClientGetRsp):
            print "%s:%s" % (msg.key, msg.value)
            # self.lastmsg = msg


if __name__ == "__main__":
    addr = sys.argv[1]
    master = sys.argv[2]
    dynamo_client = MyDynamoClientNode(addr, master)
    s = zerorpc.Server(dynamo_client)
    s.bind("tcp://" + addr)
    print "Start dynamo client %s" % addr
    gevent.spawn(s.run)
    gevent.spawn(dynamo_client.run)
    while True:
        gevent.sleep(1)
        cmd = raw_input()
        cmd_type, data = cmd.split(" ", 1)
        if cmd_type == "put":
            key, value = data.split()
            dynamo_client.put(key, [None], value)
        elif cmd_type == "get":
            key = data
            dynamo_client.get(key)
        else:
            break
    
        