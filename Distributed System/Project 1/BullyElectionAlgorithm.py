# The Bully Election Algorithm


import logging
import sys

import gevent
import zerorpc


class BullyElection(object):
    def __init__(self, address, config_file='server_config'):

        self.address = address
        self.status = 'Election'
        self.coordinator = None
        self.define_task = None
        self.halted_address = None
        self.UP_node_set = []
        self.answer = "Yes"

        self.check_greenlet = None

        self.servers = []
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

    def are_you_there(self):
        print "are you there>>>>>>>>>>>>"

        return True

    def are_you_normal(self):

        print "start>>>>>>are_you_normal"
        if self.status == 'Normal':
            return True
        else:
            return False

    def halt(self, remote_address):

        print "start>>>>>>halt"
        self.status = "Election"
        self.halted_address = remote_address

        print "%s is halting %s." % (remote_address, self.address)

    def new_coordinator(self, remote_address):

        print "start>>>>>>>>>>new_coordinator"

        if self.halted_address == remote_address and self.status == "Election":
            self.coordinator = remote_address
            self.status = "Reorganization"
        print "self coordinator is %s, self status is %s" % (self.coordinator, self.status)


    def ready(self, remote_address, task):

        print "start>>>>>>ready"
        if self.coordinator == remote_address and self.status == "Reorganization":
            self.define_task = task
            self.status = "Normal"

        print "self task : %s .  self status : %s" % (self.define_task, self.status)


def election(self):
    print "start>>>>>>election"
    for server in self.servers[self.n:self.i:-1]:
        try:
            result = self.connections[self.servers.index(server)].are_you_there()
            print '%s : are_you_there = %s' % (server, result)

            if self.check_greenlet == None:
                self.coordinator = server
                self.status = 'Normal'
                self.check_greenlet = self.pool.spawn(self.check)
            return
        except zerorpc.TimeoutExpired:
            print 'Timeout!'

    self.status = "Election"
    self.halted_address = self.address
    self.UP_node_set = []

    print 'server list:   ', self.servers[self.i::-1]
    for server in self.servers[self.i::-1]:

        try:
            print 'halt server:  ', server
            self.connections[self.servers.index(server)].halt(self.address)
        except zerorpc.TimeoutExpired:
            print 'Timeout!'
            continue
        self.UP_node_set.append(server)

    self.coordinator = self.address
    self.status = "Reorganization"

    print 'Up list:  ', self.UP_node_set
    for node in self.UP_node_set:
        try:
            self.connections[self.servers.index(node)].new_coordinator(self.address)
        except zerorpc.TimeoutExpired:
            print 'Timeout!'
            self.election()
            return

    for node in self.UP_node_set:
        try:
            self.connections[self.servers.index(node)].ready(self.address, self.define_task)
        except zerorpc.TimeoutExpired:
            print 'Timeout!'
            self.election()
            return

    self.status = "Normal"

    print "leader is : %s  " % self.coordinator
    self.check_greenlet = self.pool.spawn(self.check)


def recovery(self):
    print "start>>>>>>recovery"

    self.election()


def check(self):
    print "start>>>>>check"
    while True:
        gevent.sleep(2)
        if self.status == "Normal" and self.coordinator == self.address:
            for node in self.servers:
                if node != self.address:
                    try:
                        return_result = self.connections[self.servers.index(node)].are_you_normal()
                    except zerorpc.TimeoutExpired:
                        print 'Timeout!'
                        continue
                    if not return_result:
                        self.election()
        elif self.status == "Normal" and self.coordinator != self.address:
            try:
                self.connections[self.servers.index(self.coordinator)].are_you_there()
            except zerorpc.TimeoutExpired:
                print "Timeout!"
                self.time_out()


def time_out(self):
    print "start time_out"
    if self.status == "Normal" or self.status == "Reorganization":

        try:
            for server in self.servers:
                self.connections[self.servers.index(server)].are_you_there()
        except zerorpc.TimeoutExpired:
            print "Timeout!"
        self.election()


def start(self):
    print "start>>>>start"
    self.pool = gevent.pool.Group()
    self.recovery_greenlet = self.pool.spawn(self.recovery)


if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    addr = sys.argv[1]
    ne = BullyElection(addr)
    s = zerorpc.Server(ne)
    s.bind('tcp://' + addr)
    ne.start()
    # Start server
    logging.debug('[%s] Starting ZeroRPC Server' % addr)
    s.run()




