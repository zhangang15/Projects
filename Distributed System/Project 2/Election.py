__author__ = 'ang'

import kazoo.client
from kazoo.protocol.states import EventType
import sys
import gevent
import zerorpc


class Election(object):
    def __init__(self, address):
        self.address = address
        self.leader = None

        print 'leader: ', self.leader
        self.zk = kazoo.client.KazooClient(hosts="10.0.0.218:2181")
        self.zk.start()
        self.zk.ensure_path("/ELECTION")
        self.zk.ensure_path("/Leader")
        self.path = self.zk.create("/ELECTION/guid-n_", ephemeral=True, sequence=True)
        print "self.address: " + self.address
        print "id: " + self.path
        self.id = self.path.split("_")[-1]
        self.elect()

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


if __name__ == '__main__':
    addr = sys.argv[1]
    el = Election(addr)
    s = zerorpc.Server(el)
    s.bind('tcp://' + addr)
    s.run()