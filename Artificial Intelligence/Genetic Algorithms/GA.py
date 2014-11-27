#!/usr/bin/env python

import random


class GA:
    def __init__(self, problem, popsize=100, elitismRate=0.2,
                 mutationRate=0.05, itersToRun=100):
        self.problem = problem
        self.popsize = popsize
        self.elitismRate = elitismRate
        self.mutationRate = mutationRate
        self.itersToRun = itersToRun

        self.population = problem.makePopulation(popsize)

    def run(self):
        i = 0
        while i < self.itersToRun and not self.problem.solved(self.population):
            i += 1
            self.problem.evalFitness(self.population)
            keepers = self.ApplyElitism(self.population,
                                        self.elitismRate)
            j = len(keepers)
            newcomers = []
            while j <= self.popsize:
                j += 2
                c1, c2 = self.selectChromosomes(self.population)
                c3, c4 = self.crossover(c1, c2)
                newcomers.extend([c3, c4])
            for c in newcomers:
                if random.random() < self.mutationRate:
                    self.problem.mutate(c)
            self.population = keepers + newcomers
            print "Iteration: %d" % i
        self.problem.evalFitness(self.population)
        print "Final results: "
        print [i.fitness for i in self.population]
        print self.population

        return [i.fitness for i in self.population]