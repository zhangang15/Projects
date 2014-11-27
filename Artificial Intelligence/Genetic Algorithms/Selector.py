import random

## more mixin here

class Selector:
    ### given a population, choose two parents
    def selectChromosomes(self, pop):
        raise NotImplementedError


class TournamentSelector(Selector):
    def selectChromosomes(self, pop):
        p1 = random.choice(pop)
        p2 = random.choice(pop)
        while p1 == p2:
            p2 = random.choice(pop)
        if p1.fitness > p2.fitness:
            good_p1 = p1
        else:
            good_p1 = p2

        p3 = random.choice(pop)
        p4 = random.choice(pop)
        while p3 == p4:
            p4 = random.choice(pop)
        if p3.fitness > p4.fitness:
            good_p2 = p3
        else:
            good_p2 = p4

        return good_p1, good_p2


### You do this part. The method should return two potential parents.
### For each parent, choose two potential parents and compare their
### fitnesses. The more fit parent is retained.

### choose two parents via roulette selection
class RouletteSelector(Selector):
    def selectChromosomes(self, pop):
        total = sum([x.fitness for x in pop])
        rval = random.randint(0, total)
        i = 0
        counter = pop[0].fitness
        while counter < rval and i < len(pop) - 1:
            counter += pop[i].fitness
            i += 1
        c1 = pop[i]
        rval = random.randint(0, total)
        i = 0
        counter = pop[0].fitness
        while counter < rval and i < len(pop) - 1:
            counter += pop[i].fitness
            i += 1
        c2 = pop[i]
        return c1, c2
