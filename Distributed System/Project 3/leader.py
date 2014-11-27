import zerorpc
import gevent
import sys


class Leader(object):
    def __init__(self, addr):
        self.addr = addr
        self.members = []


    def join_in(self, new_addr):
        if new_addr in self.members:
            members = self.members[:]
            members.remove(new_addr)
            return members
        members = self.members[:]
        self.members.append(new_addr)
        for addr in members:
            c = zerorpc.Client(timeout=2)
            c.connect("tcp://" + addr)
            try:
                c.notify(new_addr)
            except zerorpc.TimeoutExpired:
                pass
            c.close()
        return members

    def get_members(self):
        return self.members

    def run(self):
        while True:
            gevent.sleep(2)
            print "My members: %s" % ",".join([addr for
                                               addr in self.members])


if __name__ == "__main__":
    addr = sys.argv[1]
    leader = Leader(sys.argv[1])
    s = zerorpc.Server(leader)
    s.bind("tcp://" + addr)
    print "Start leader %s" % addr
    gevent.spawn(leader.run)
    s.run()