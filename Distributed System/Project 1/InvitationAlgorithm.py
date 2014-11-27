# The Invitation Election Algorithm
__author__ = 'ang'

import logging
import sys

import gevent
import zerorpc


class InvitationAlgorithm():
    def __init__(self, address, config_file='server_config'):

        self.address = address
        self.status = 'Election'
        self.coordinator = None
        self.define_task = None
        self.halted_address = None
        self.UP_node_set = []

        self.group_number = None
        self.counter = 0

        self.coordinator_set = []

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


    def check(self):
        print "start>>>>>>check()"

        while True:
            gevent.sleep(2)
            if self.status == "Normal" and self.coordinator == self.address:
                TempSet = []
                for node in self.servers:
                    if node != self.address:
                        answer = "No"
                        try:
                            # ang
                            answer = self.connections[self.servers.index(node)].are_you_coordinator()
                            logging.debug("%s is coordinator: %s" % (node, answer))
                        except zerorpc.TimeoutExpired:
                            print "%s Timeout!" % node
                            # ang
                            continue

                        if answer == "Yes":
                            TempSet.append(self.servers.index(node))
                print 'Tempset: ', TempSet
                if TempSet == []:
                    # ang
                    continue

                p = max(TempSet)

                if self.i < p:
                    gevent.sleep((p - self.i) * 2)

                self.merge(TempSet)

            elif self.status == "Normal" and self.coordinator != self.address:
                try:
                    answer = self.connections[self.servers.index(self.coordinator)].are_you_there(self.group_number,
                                                                                                  self.address)
                    logging.debug('%s is there: %s' % (self.coordinator, answer))
                except zerorpc.TimeoutExpired:

                    print "coordinator is dead!!!! %s" % self.coordinator
                    self.time_out()


    def time_out(self):
        MyCoord = self.coordinator
        MyGroup = self.group_number

        if MyCoord == self.address:
            return

        else:
            # ang
            answer = False
            try:
                answer = self.connections[self.servers.index(self.coordinator)].are_you_there(self.group_number,
                                                                                              self.address)
            except zerorpc.TimeoutExpired:
                self.recovery()

            if answer == "No":
                self.recovery()

    def merge(self, coord_set):
        logging.debug("merge()>>>>>>>>>>>>>")
        self.status = "Election"

        # CALL STOP

        self.counter += 1
        self.group_number = (self.address, self.counter)

        self.coordinator = self.address
        TempSet = self.UP_node_set
        self.UP_node_set = []

        for node in coord_set:
            try:
                self.connections[node].invitation(self.address, self.group_number)
            except zerorpc.TimeoutExpired:
                print "02 >>>>> merge timeout!"
                # ang
                continue

        for node in TempSet:
            try:
                print self.servers.index(node)
                self.connections[self.servers.index(node)].invitation(self.address, self.group_number)
            except zerorpc.TimeoutExpired:
                print "03>>>>>another___merge timeout!"

                continue

        gevent.sleep(2)

        self.status = "Reorganization"

        for node in self.UP_node_set:

            try:
                self.connections[self.servers.index(node)].ready(self.address, self.group_number, self.define_task)

            except zerorpc.TimeoutExpired:
                print "03>>>>>>>>>the third_merge timeout"

                self.recovery()

        self.status = "Normal"

    def ready(self, target_address, gn, x):
        logging.debug("ready() >>>>>>>>>>>>>")
        if self.status == "Reorganization" and self.group_number == gn:
            self.define_task = x
            self.status = "Normal"

    def are_you_coordinator(self):
        if self.status == "Normal" and self.coordinator == self.address:

            return "Yes"
        else:
            return "No"


    def are_you_there(self, gn, j):
        if self.group_number == gn and self.coordinator == self.address and j in self.UP_node_set:

            return "Yes"
        else:
            return "No"

    def invitation(self, j, gn):
        logging.debug("invitation >>>>>>>>>>>>>")
        if self.status != "Normal":
            return

            ## CALL  STOP
        temp = self.coordinator
        TempSet = self.UP_node_set
        self.status = "Election"
        self.coordinator = j
        self.group_number = gn

        if temp == self.address:

            for node in TempSet:
                try:
                    self.connections[self.servers.index(node)].invitation(j, gn)
                except zerorpc.TimeoutExpired:
                    print "04>>>>>>>>>>>invitation >>>>timeout!"
        try:
            self.connections[self.servers.index(j)].accept(self.address, gn)
        except zerorpc.TimeoutExpired:

            print "05>>>>>>>invitation >>>>>>timeout!"

            self.recovery()

        self.status = "Reorganization"

    def accept(self, j, gn):
        logging.debug("accept >>>>>>>>>>>>>")
        if self.status == "Election" and self.group_number == gn and self.coordinator == self.address:
            self.UP_node_set.append(j)

    def recovery(self):
        logging.debug('recovery  >>>>>>>>>>>>>>>>>')
        self.status = "Election"
        # CALL STOP

        self.counter += 1

        self.group_number = (self.address, self.counter)

        self.coordinator = self.address
        self.UP_node_set = []
        self.status = "Reorganization"

        self.status = "Normal"

        # ang
        if self.check_greenlet is None:
            self.check_greenlet = self.pool.spawn(self.check)

    def start(self):
        print "start>>>>start"
        self.pool = gevent.pool.Group()
        self.recovery_greenlet = self.pool.spawn(self.recovery)


if __name__ == '__main__':
    logging.basicConfig(level=logging.DEBUG)
    addr = sys.argv[1]
    ne = InvitationAlgorithm(addr)
    s = zerorpc.Server(ne)
    s.bind('tcp://' + addr)
    ne.start()
    # Start server
    logging.debug('[%s] Starting ZeroRPC Server' % addr)
    s.run()



















