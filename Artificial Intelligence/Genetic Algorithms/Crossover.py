from BitStringGAProblem import BitChromosome
from TSPGAProblem import TSPChromosome
import random
from buildGraph import graph
### mixin for crossover.

class Crossover:
    def crossover(self, c1, c2):
        raise NotImplementedError


### single-point bitstring crossover.
class OnePointStringCrossover(Crossover):
    def crossover(self, c1, c2):
        locus = random.randint(0, len(c1.bitstring) - 1)
        child1 = BitChromosome(c1.bitstring[:locus] + c2.bitstring[locus:])
        child2 = BitChromosome(c2.bitstring[:locus] + c1.bitstring[locus:])
        return child1, child2


class TwoPointStringCrossover(Crossover):
    def crossover(self, c1, c2):
        l1 = random.randint(0, len(c1.bitstring) - 1)
        l2 = random.randint(0, len(c1.bitstring) - 1)
        if l1 > l2:
            l1, l2 = l2, l1

        child1 = BitChromosome(c1.bitstring[:l1] + c2.bitstring[l1:l2] + c1.bitstring[l2:])
        child2 = BitChromosome(c2.bitstring[:l1] + c1.bitstring[l1:l2] + c2.bitstring[l2:])
        return child1, child2


### works with Permutation Chrmosomes
class PermutationCrossover(Crossover):
    def crossover(self, c1, c2):
        l1 = random.randint(0, len(c1.citylist) - 1)
        c1list = c1.citylist[:l1]
        for item in c2.citylist:
            if item not in c1list:
                c1list.append(item)
        child1 = TSPChromosome(c1list)
        c2list = c2.citylist[:l1]
        for item in c1.citylist:
            if item not in c2list:
                c2list.append(item)
        child2 = TSPChromosome(c2list)
        return child1, child2


## works with TSP Chromosomes.
class GreedyCrossover(Crossover):
    def crossover(self, c1, c2):
        c1list = []
        c1list.append(c1.citylist[0])
        for i in range(1, len(c1.citylist)):
            elist = self.tspgraph.adjlist[c1list[i - 1]][1:]
            edge1 = None
            edge2 = None
            cost1 = 1000000
            cost2 = 1000000
            if c1.citylist[i] in c1list and c2.citylist[i] not in c1list:
                c1list.append(c2.citylist[i])
            elif c1.citylist[i] not in c1list and c2.citylist[i] in c1list:
                c1list.append(c1.citylist[i])
            elif c1.citylist[i] in c1list and c2.citylist[i] in c1list:
                templist1 = []
                for e in elist:
                    if e.dest not in c1list:
                        templist1.append(e.dest)
                c1list.append(random.choice(templist1))
            else:
                for e in elist:
                    if e.dest == c1.citylist[i]:
                        edge1 = e
                        cost1 = float(edge1.distance[:-2])
                    if e.dest == c2.citylist[i]:
                        edge2 = e
                        cost2 = float(edge2.distance[:-2])

                if cost1 >= cost2:
                    c1list.append(c2.citylist[i])
                else:
                    c1list.append((c1.citylist[i]))

        c2list = []
        c2list.append(c2.citylist[0])
        for i in range(1, len(c2.citylist)):
            elist = self.tspgraph.adjlist[c2list[i - 1]][1:]
            edge1 = None
            edge2 = None
            cost1 = 1000000
            cost2 = 1000000
            if c1.citylist[i] in c2list and c2.citylist[i] not in c2list:
                c2list.append(c2.citylist[i])
            elif c1.citylist[i] not in c2list and c2.citylist[i] in c2list:
                c2list.append(c1.citylist[i])
            elif c1.citylist[i] in c2list and c2.citylist[i] in c2list:
                templist2 = []
                for e in elist:
                    if e.dest not in c2list:
                        templist2.append(e.dest)
                c2list.append(random.choice(templist2))
            else:
                for e in elist:
                    if e.dest == c1.citylist[i]:
                        edge1 = e
                        cost1 = float(edge1.distance[:-2])
                    if e.dest == c2.citylist[i]:
                        edge2 = e
                        cost2 = float(edge2.distance[:-2])

                if cost1 >= cost2:
                    c2list.append(c2.citylist[i])
                else:
                    c2list.append((c1.citylist[i]))

        child1 = TSPChromosome(c1list)
        child2 = TSPChromosome(c2list)

        return child1, child2

        # if edge1 is None and edge2 is not None:

##                print 'edge 1 None'
# c1list.append(edge2.dest)
##                edge1 = edge2
#            elif edge2 is None and edge1 is not None:
##                print 'edge 2 None'
#                c1list.append(edge1.dest)
##                edge2 = edge1
#            elif edge1 is None and edge2 is None:
##                print 'edge 1 2 None', c1.citylist[i], c2.citylist[i], e.dest
#                while True:
#                    edge1 = random.choice(elist)
#                    if edge1.dest not in c1list:
#                        break
##                edge2 = random.choice(elist)
#            else:
#                templist1 = []
#
#    #            print 'edges', edge1, edge2
#    #            if edge1 is None or edge2 is None:
#    #                print elist[0].src, [i.dest for i in elist]
#                if edge1.distance > edge2.distance:
#                    if edge2.dest in c1list:
#
#                        if edge1.dest in c1list:
#                            for e in elist:
#                                if e.dest not in c1list:
#                                    templist1.append(e.dest)
#                                    c1list.append(random.choice(templist1))
#                        else:
#                            c1list.append(edge1.dest)
#                    else:
#                        c1list.append(edge2.dest)
#                elif edge1.distance <= edge2.distance:
#                    if edge1.dest in c1list:
#
#                        if edge2.dest in c1list:
#                            for e in elist:
#                                if e.dest not in c1list:
#                                    templist1.append(e.dest)
#                                    c1list.append(random.choice(templist1))
#                        else:
#                            c1list.append(edge2.dest)
#                    else:
#                        c1list.append(edge1.dest)
#
#        child1 = TSPChromosome(c1list)
#
#        c2list = []
#        c2list.append(c2.citylist[0])
#        print len(c2.citylist)
#        for i in range(1,len(c2.citylist)):
#            print 'c2list length: ', len(c1list)
##            print "=========================" + c2list[i-1]
##            print self.tspgraph.adjlist[c2list[i-1]]
#            elist = self.tspgraph.adjlist[c2list[i-1]][1:]
#            edge1 = None
#            edge2 = None
##            print child1
#            for e in elist:
#                if e.dest == c1.citylist[i]:
#                    edge1 = e
#
#            for e in elist:
#                if e.dest == c2.citylist[i]:
#                    edge2 = e
#
##                if e.dest == c2.citylist[i]:
##                    edge2 = e
#            if edge1 is None and edge2 is not None:
##                print 'edge 1 None'
#                c2list.append(edge2.dest)
#            #                edge1 = edge2
#            elif edge2 is None and edge1 is not None:
##                print 'edge 2 None'
#                c2list.append(edge1.dest)
#            #                edge2 = edge1
#            elif edge1 is None and edge2 is None:
##                print 'edge 1 2 None', c1.citylist[i], c2.citylist[i], e.dest
#                while True:
#                    edge1 = random.choice(elist)
#                    if edge1.dest not in c2list:
#                        break
#                    #                edge2 = random.choice(elist)
#            else:
#                templist2 = []
#                if edge1.distance > edge2.distance:
#                    if edge2.dest in c2list:
#
#                        if edge1.dest in c2list:
#                            for e in elist:
#                                if e.dest not in c2list:
#                                    templist2.append(e.dest)
#                                    c2list.append(random.choice(templist2))
#
#                        else:
#                            c2list.append(edge1.dest)
#                    else:
#                        c2list.append(edge2.dest)
#                elif edge1.distance <= edge2.distance:
#                    if edge1.dest in c2list:
#
#                        if edge2.dest in c2list:
#                            for e in elist:
#                                if e.dest not in c2list:
#                                    templist2.append(e.dest)
#                                    c2list.append(random.choice(templist2))
#
#
#                        else:
#                            c2list.append(edge2.dest)
#                    else:
#                        c2list.append(edge1.dest)
#
#        child2 = TSPChromosome(c2list)
#
#


### you do this one.
### The algorithm is as follows:
### Child1[0] == parent1[0]
### if parent1[1] is closer to Child1[0] than parent2[1], then Child1[1] = parent1[1]
### Otherwise, Child1[1] = parent2[1].
### If the chosen node is already in Child1, use the other.
### If both nodes are already in the child, select a random unused node.
### Repeat this for each subsequent node - for child[i], choose the closer parent node.
### Child2 starts with the first node of parent2 and proceeds similarly.

### To get distances, you will need a copy of the TSP graph. you can either attach it to the instance of the 
### mixin class after creating it, like so:
### class mySolver(GA.GA, Elitism.DeterministicElites, Crossover.GreedyCrossover, Selector.TournamentSelector) :
###      pass
### m = mySolver
### g = graph.Graph()
### g.makeTSPProblem(10)
### m.tspgraph = g
###
### or, you can write a method that attaches it.

        
        
        

