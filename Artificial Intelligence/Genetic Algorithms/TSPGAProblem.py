from GAProblem import GAProblem, Chromosome
import random

### How to use this:
### 1. Create a graph. For example, here I'm using the sfdata. You might want to start with a smaller one to begin.
### import buildGraph, TSPGAProblem
### g = buildGraph.graph()
### g.buildGraph("sfdata")

### 2. create a fitness function. It needs to take a chromosome as input and return a scalar.
### def f(c) :
###      return TSPGAProblem.computeTourCost(g,c)
### 3. Create a TSPGAProblem.
### newProblem = TSPGAProblem(f, g)

### 4. Solve as before by creating a solver.


class TSPGAProblem(GAProblem):
    ### g is a graph that will be used to represent the TSP
    def __init__(self, fitnessFn, g):
        GAProblem.__init__(self, fitnessFn)
        self.g = g


    def makePopulation(self, popsize):
        pop = []
        for i in range(popsize):
            clist = self.g.adjlist.keys()
            random.shuffle(clist)
            pop.append(TSPChromosome(clist))
        return pop

    def evalFitness(self, pop):
        for p in pop:
            p.fitness = self.fitnessFn(p)

    def solved(self, pop):
        return False

    def mutate(self, c):
        c1 = random.randint(0, len(c.citylist) - 1)
        c2 = random.randint(0, len(c.citylist) - 1)
        newc = []
        for i in range(len(c.citylist)):
            if i == c1:
                newc.append(c.citylist[c2])
            elif i == c2:
                newc.append(c.citylist[c1])
            else:
                newc.append(c.citylist[i])
        c.citylist = newc


class TSPChromosome(Chromosome):
    def __init__(self, citylist):
        Chromosome.__init__(self)
        self.citylist = citylist

    def __repr__(self):
        return "%s %d" % (self.citylist, self.fitness)


def computeTourCost(g, c):
    path = c.citylist
    cost = 0
    for i in range(len(path) - 1):
        elist = g.adjlist[path[i]][1:]
        e = [e for e in elist if e.dest == path[i + 1]][0]
        if e is None:
            cost = 100000000
        else:
            cost += float(e.distance[:-2])
    elist = g.adjlist[path[-1]][1:]
    e = [e for e in elist if e.dest == path[0]][0]
    if e is None:
        cost = 100000000
    else:
        cost += float(e.distance[:-2])

    return cost




