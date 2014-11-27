from GAProblem import GAProblem, Chromosome
import random

### we'll assume that the "bitstring" is really a string of '0' and '1'. Not 
### as storage-efficient as it could be.

class BitStringGAProblem(GAProblem):
    def __init__(self, fitnessFn, strlength):
        GAProblem.__init__(self, fitnessFn)
        self.strlength = strlength

    def makePopulation(self, popsize):
        def getbit():
            if random.random() < 0.5:
                return '0'
            else:
                return '1'

        pop = []
        for i in range(popsize):
            pop.append(BitChromosome("".join([getbit() for x in range(self.strlength)])))
        return pop

    def evalFitness(self, pop):
        for p in pop:
            p.fitness = self.fitnessFn(p)

    def solved(self, pop):
        return False

    def mutate(self, c):
        def xor(s):
            if s == '1':
                return '0'
            else:
                return '1'

        mbit = random.randint(0, len(c.bitstring) - 1)
        c.bitstring = c.bitstring[:mbit] + xor(c.bitstring[mbit]) + c.bitstring[mbit + 1:]


class BitChromosome(Chromosome):
    def __init__(self, bitstring):
        Chromosome.__init__(self)
        self.bitstring = bitstring

    def __repr__(self):
        return self.bitstring

