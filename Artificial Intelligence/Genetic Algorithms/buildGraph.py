import cPickle as pickle
import random

### build graph: 
### takes as input a file in the form:
## a b dist time
### where a and b are destinations, dist is the distance between them, and 
### time is the time needed to travel between them and constructs a graph.

### This graph should be represented as an adjacency list, and stored as a 
### dictionary, with the key in the dictionary being the source of an edge and 
### the value being a tuple containing the destination, distance, and cost.
### For example:
### g[a] = (b,dist,time)

class graph():
    def __init__(self, infile=None):
        self.adjlist = {}
        if infile:
            self.buildGraph(infile)


    ### method to print a graph.
    def __repr__(self):
        return '%s' % self.adjlist

    def createVertex(self, str):
        name, lat, long = str.split(" ", 2)
        lat = lat.split("=")[1]
        long = long.split("=")[1]
        return location(name, lat, long)

    def createEdges(self, str):
        src, dest, dist, time = str.split(" ", 4)
        dist = dist.split("=")[1]
        time = time.split("=")[1]
        e1 = edge(src, dest, dist, time)
        e2 = edge(dest, src, dist, time)
        return e1, e2

    ### method that takes as input a file name and constructs the graph described
    ### above.
    def buildGraph(self, infile):
        f = open(infile)
        line = f.readline()
        ## consume comments
        while line.startswith("#"):
            line = f.readline()
            ## create vertices
        while not line.startswith("#") and len(line) > 1:
            v = self.createVertex(line)
            ### adjlist is indexed my the string representing the
            ##location. The first object in the list is the vertex. 
            self.adjlist[v.name] = [v]
            line = f.readline()
            ## consume comments
        while line.startswith("#") or len(line) <= 1:
            line = f.readline()
            ## create edges
        while line and len(line) > 1:
            e1, e2 = self.createEdges(line)
            self.adjlist[e1.src].append(e1)
            self.adjlist[e2.src].append(e2)
            line = f.readline()


### this method should compute Prim's algorithm and
### return a graph representing the minimum spanning tree computed by Prim.


class location():
    def __init__(self, name, lat, longitude):
        self.name = name
        self.lat = lat
        self.longitude = longitude

    def __hash__(self):
        return hash(self.name)

    def __eq__(self, other):
        return self.name == other.name


class edge():
    def __init__(self, src, dest, distance, time):
        self.src = src
        self.dest = dest
        self.distance = distance
        self.numDist = float(distance[:-2])
        self.time = time

