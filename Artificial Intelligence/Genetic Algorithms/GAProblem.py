## An abstract base class to encapsulate all the problem-specific knowledge.
class GAProblem:
    def __init__(self, fitnessFn):
        self.fitnessFn = fitnessFn
        pass

    def makePopulation(self, popsize):
        raise NotImplementedError

    def evalFitness(self, pop):
        for p in pop:
            p.fitness = self.fitnessFn(p)

    def solved(self, pop):
        raise NotImplementedError

    def mutate(self, c):
        raise NotImplementedError


### abstract base class representing Chromosomes. WIll I need this?
class Chromosome:
    def __init__(self):
        self.fitness = 0

    def __repr__(self):
        raise NotImplementedError

    def __lt__(self, other):
        return self.fitness < other.fitness

    def __gt__(self, other):
        return self.fitness > other.fitness

    
