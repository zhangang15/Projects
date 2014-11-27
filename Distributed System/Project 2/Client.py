__author__ = 'ang'

import kazoo.client
from kazoo.protocol.states import EventType
import sys
import gevent
import zerorpc
import leveldb
import logging
import os


class Store():
    def __init__(self, address_list, config_file='server_config'):

        address_host = ",".join(address_list)
        self.zk = kazoo.client.KazooClient(hosts=address_host)
        self.zk.start()
        leader_address = self.zk.get("/Leader")[0]
        print ".dchecking client leader address " + leader_address

        self.servers = []
        file = open(config_file, 'r')
        for line in file.readlines():
            print line
            line = line.rstrip()
            self.servers.append(line)

        self.n = len(self.servers)

        self.connections = []

        for i, server in enumerate(self.servers):
            c = zerorpc.Client(timeout=10)
            c.connect('tcp://' + server)
            self.connections.append(c)

    def put(self, key, value):

        while True:
            leader_address = self.zk.get("/Leader")[0]
            try:
                t = self.connections[self.servers.index(leader_address)].request_put(key, value)
                if t:
                    return
            except zerorpc.TimeoutExpired:
                print 'store put Timeout!'

    def delete(self, key):

        leader_address = self.zk.get("/Leader")[0]

        try:
            while self.connections[self.servers.index(leader_address)].request_delete(key) is True:
                return
        except zerorpc.TimeoutExpired:
            print'delete Timeout!'

    def get(self, key):
        leader_address = self.zk.get("/Leader")[0]

        try:
            v = self.connections[self.servers.index(leader_address)].get(key)
            return v
        except zerorpc.TimeoutExpired:
            print 'get Timeout!'