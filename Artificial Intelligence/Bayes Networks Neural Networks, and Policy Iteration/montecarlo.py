import sys
import random


class montecarlo(object):
    def __init__(self, inputfile=None):
        self.parent = {}
        self.linkmatrix = {}
        self.value = {}
        self.topoNumber = {}
        self.topoOrder = []
        self.frequencies = {}
        self.frequency = 0
        self.evidence = {}

        if inputfile != None:
            self.readfile(inputfile)

            ### Read in a single link matrix, set up the appropriate
            ### parent pointers, as well as all of the values in the
            ### link matrix

    def readMatrix(self, infile):
        header = infile.readline()
        childParentList = header[header.find("(") + 1:header.find(")")].split("|")
        if len(childParentList) == 1:
            child = childParentList[0]
            parents = []
        else:
            [child, parent] = childParentList
            parents = parent.split(",")
        self.parent[child] = parents
        if parents == []:
            self.linkmatrix[child] = [float(x) for x in infile.readline().split()]
        else:
            self.linkmatrix[child] = []
            for i in range(2 ** len(parents)):
                nl = infile.readline()
                self.linkmatrix[child].append([float(x) for x in nl.split()])

                ### Do a topological sort on the graph defined by the parent pointers
                ###  self.parent[], puting the result in self.topologicalOrder.  If
                ###  node 'a' appears 5th in a topological sort of the graph, then
                ###  self.topoloNumber['a'] == 5, and self.topoOrder[5] = 'a'.  Note that
                ###  topological ordering starts at 0

    def topoSort(self):
        outDegree = {}
        stack = []
        for x in self.parent:
            outDegree[x] = 0
        for x in self.parent:
            par = self.parent[x]
            for x in par:
                outDegree[x] = outDegree[x] + 1
        zeroOutdegree = [x for x in self.parent if outDegree[x] == 0]
        topoNum = len(self.parent) - 1
        self.topoOrder = [-1] * len(self.parent)
        while (zeroOutdegree != []):
            next = zeroOutdegree.pop()
            self.topoNumber[next] = topoNum
            self.topoOrder[topoNum] = next
            topoNum = topoNum - 1
            par = self.parent[next]
            for x in par:
                outDegree[x] = outDegree[x] - 1
                if outDegree[x] == 0:
                    zeroOutdegree.append(x)

                    ### Read in the graph & link matrices from a file

    def readfile(self, filename):
        infile = file(filename)
        numvars = int(infile.readline())
        for i in range(numvars):
            var = infile.readline().split()
        for i in range(numvars):
            self.readMatrix(infile)
        infile.readline()
        evidenceLine = infile.readline()
        while evidenceLine[0] != '-':
            [var, val] = evidenceLine.split()
            self.evidence[var] = int(val)
            evidenceLine = infile.readline()
        self.topoSort()


    ## FILL ME IN
    def doIterations(self, numIterations):
        for n in self.topoOrder:
            self.frequencies[n] = 0
        for num in range(numIterations):
            for node in self.topoOrder:

                if self.parent[node] == []:
                    p1 = random.random()
                    # print p1
                    if p1 < self.linkmatrix[node][0]:
                        self.value[node] = 0
                    else:
                        self.value[node] = 1
                else:
                    s = ""
                    for p in self.parent[node]:
                        s += str(self.value[p])

                    p2 = random.random()
                    # print p2
                    if p2 < self.linkmatrix[node][int(s, 2)][0]:
                        self.value[node] = 0
                    else:
                        self.value[node] = 1

            if self.value[self.topoOrder[-1]] == self.evidence[self.topoOrder[-1]]:

                self.frequency += 1
                for node in self.topoOrder:
                    if self.value[node] == 1:
                        self.frequencies[node] += 1


    ## FILL ME IN
    def printResults(self):
        print "Valid Trials:" + str(self.frequency)
        for node in self.topoOrder:
            a = float(self.frequencies[node]) / self.frequency
            s = "P(" + node + ") = "
            print   s + "%.3f" % a


if __name__ == '__main__':
    network = montecarlo(sys.argv[-1])
    network.doIterations(int(sys.argv[-2]))
    network.printResults()
