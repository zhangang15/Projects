__author__ = 'ang'

import kazoo.client
from kazoo.protocol.states import EventType
import sys
import gevent
import zerorpc
import leveldb
import logging
import os


class Server(object):
    def __init__(self, address, config_file='server_config'):

        self.address = address
        self.leader = None
        if os.path.exists(self.address) is False:
            os.mkdir(self.address)
        self.db = leveldb.LevelDB(self.address)
        self.status = 'Idle'
        self.servers = []
        self.check_greenlet = None
        self.on_server = [True, True, True]

        self.logger = logging.getLogger()
        file = logging.FileHandler(self.address + 'log')
        self.logger.addHandler(file)
        self.logger.setLevel(logging.INFO)
        formatter = logging.Formatter("%(levelname)s %(message)s")
        file.setFormatter(formatter)

        self.tran_id = 0
        if os.path.exists(self.address + 'log') is True:
            print "now start finding max id"
            logfile = open(self.address + 'log', 'r')
            temp_max_id = None
            for line in logfile.readlines():
                line = line.rstrip()
                if line.split(",")[-1] == 'd' or line.split(",")[-1] == 'w':
                    temp_max_id = line.split(",")[0][5:]
                    # print temp_max_id
            if temp_max_id is not None:
                self.tran_id = int(temp_max_id)
        else:
            open(self.address + 'log', 'w').close()

        file = open(config_file, 'r')
        for line in file.readlines():
            print line
            line = line.rstrip()
            self.servers.append(line)
        print "My address: %s" % (str(self.address))
        print "Server list: %s" % (str(self.servers))

        self.n = len(self.servers)

        self.connections = []

        for i, server in enumerate(self.servers):
            if server == self.address:
                self.i = i
                self.connections.append(self)
            else:
                c = zerorpc.Client(timeout=2)
                c.connect('tcp://' + server)
                self.connections.append(c)

        self.votes = 0

        print 'leader: ', self.leader
        self.zk = kazoo.client.KazooClient(hosts="10.0.0.218:2181")
        # self.zk = kazoo.client.KazooClient(hosts="127.0.0.1:2181,127.0.0.1:2182,127.0.0.1:2183")
        self.zk.start()
        self.zk.ensure_path("/ELECTION")
        self.zk.ensure_path("/Leader")
        self.path = self.zk.create("/ELECTION/guid-n_", ephemeral=True, sequence=True)
        self.id = self.path.split("_")[-1]
        self.elect()
        # self.pool = gevent.pool.Group()
        # self.pool.spawn(self.sync_function,self.tran_id, self.address)


    def sync_function(self, follower_tran_id, follower_address):

        self.on_server[self.servers.index(follower_address)] = True

        while self.status != 'Idle':
            print"sleeping syn"
            gevent.sleep(1)
            continue
        # if self.address != self.leader:
        self.status = 'Synchronizing'
        print "Now is synchronizing!"
        logfile = open(self.address + 'log', 'r')

        for line in logfile.readlines():
            line = line.rstrip()

            if line.split(",")[-1] != "d" and line.split(",")[-1] != "w":
                print "still continue..."
                continue

            if line.split(",")[-1] == "d" or line.split(",")[-1] == "w":
                loglist = line.split(',')

            if int(loglist[0][5:]) > follower_tran_id:
                # print "start comparing id "
                if loglist[-1] == 'w':
                    try:
                        #print 'entering wwwwwwwwwwwwwwwww'
                        self.connections[self.servers.index(follower_address)].commit_put(loglist[0][4:], loglist[1],
                                                                                          self.db.Get(loglist[1]))
                    except zerorpc.TimeoutExpired:
                        print '1Timeout!'

                elif loglist[-1] == 'd':
                    try:
                        self.connections[self.servers.index(follower_address)].commit_delete(loglist[0][4:], loglist[1])
                    except zerorpc.TimeoutExpired:
                        print '2Timeout!'

        self.status = 'Idle'
        print "Now finish Synchronizing!"


    def elect(self):
        print 'start electing...'

        def watcher(event):
            self.zk.exists("/ELECTION/" + children[sequence - 1], watcher)

            if event.type == EventType.DELETED:
                self.elect()

            elif event.type == EventType.CHANGED:
                self.leader = self.zk.get("/ELECTION/" + children[sequence - 1])[0]
                print 'leader: ', self.leader
                self.zk.set(self.path, self.leader)

        children = self.zk.get_children("/ELECTION")
        children.sort()
        if self.path == "/ELECTION/" + children[0]:
            self.leader = self.address
            self.zk.set("/Leader", self.address)
            self.zk.set(self.path, self.address)
            print 'leader: ', self.leader
        else:
            for i in range(0, len(children)):
                if self.path == "/ELECTION/" + children[i]:
                    sequence = i

            self.leader = self.zk.get("/Leader")[0]
            self.zk.exists("/ELECTION/" + children[sequence - 1], watcher)

            print 'leader: ', self.leader
        print "is leader " + str(self.address == self.leader)

        if self.address != self.leader:
            try:
                self.connections[self.servers.index(self.leader)].sync_function(self.tran_id, self.address)
                print "entering call leader sync"
            except zerorpc.TimeoutExpired:
                print '3Timeout!'


    def vote(self):


        # print "start voting"
        return True

    def commit_put(self, tran_id, key, value):

        self.db.Put(str(key), str(value))
        self.logger.info(str(tran_id) + ',' + str(key) + ',' + 'w')
        self.tran_id = tran_id

    def request_put(self, key, value):
        while self.status == 'Synchronizing':
            gevent.sleep(1)
        temp_tran_id = self.tran_id + 1
        temp_votes = 0

        for server in self.servers:
            try:
                if self.on_server[self.servers.index(server)] is False:
                    continue
            except zerorpc.TimeoutExpired:
                print "Timeout!"
            try:
                if self.connections[self.servers.index(server)].vote() is True:
                    temp_votes += 1
            except zerorpc.TimeoutExpired:
                print ' 4 Timeout!'
                self.on_server[self.servers.index(server)] = False

        self.votes = temp_votes
        if self.votes < 0.5 * self.n:
            print self.votes < 0.5 * self.n
            return False
        else:
            for server in self.servers:
                try:
                    if self.on_server[self.servers.index(server)] is True:
                        self.connections[self.servers.index(server)].commit_put(temp_tran_id, key, value)
                except zerorpc.TimeoutExpired:
                    print '5 Timeout!'
                    self.on_server[self.servers.index(server)] = False
            self.tran_id = temp_tran_id
            return True

    def commit_delete(self, tran_id, key):


        self.db.Delete(str(key))
        self.logger.info(str(tran_id) + ',' + str(key) + ',' + 'd')
        self.tran_id = tran_id

    def request_delete(self, key):

        while self.status == 'Synchronizing':
            gevent.sleep(1)

        temp_tran_id = self.tran_id + 1
        temp_votes = 0

        for server in self.servers:
            try:
                if self.on_server[self.servers.index(server)] is False:
                    continue
            except zerorpc.TimeoutExpired:
                print "Timeout!"
            try:
                if self.connections[self.servers.index(server)].vote() is True:
                    temp_votes += 1
            except zerorpc.TimeoutExpired:
                print ' 4 Timeout!'
                self.on_server[self.servers.index(server)] = False

        self.votes = temp_votes
        if self.votes < 0.5 * self.n:
            print self.votes < 0.5 * self.n
            return False
        else:
            for server in self.servers:
                try:
                    if self.on_server[self.servers.index(server)] is True:
                        self.connections[self.servers.index(server)].commit_delete(temp_tran_id, key)
                except zerorpc.TimeoutExpired:
                    print '5 Timeout!'
                    self.on_server[self.servers.index(server)] = False
            self.tran_id = temp_tran_id
            return True

    def get(self, key):

        return self.db.Get(key)


if __name__ == '__main__':
    addr = sys.argv[1]

    el = Server(addr)
    s = zerorpc.Server(el)
    s.bind('tcp://' + addr)
    s.run()
