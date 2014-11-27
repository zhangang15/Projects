import random
### we'll use the 'mixin' style here.

class Elites:
    def ApplyElitism(self, pop, erate):
        raise NotImplementedError


class DeterministicElites(Elites):
    def ApplyElitism(self, pop, erate):
        pop.sort()
        pop.reverse()
        return pop[0:int(erate * len(pop))]


class TournamentElites(Elites):
    def ApplyElitism(self, pop, erate):
        children = []
        for i in range(int(erate * len(pop))):
            c1 = random.choice(pop)
            c2 = random.choice(pop)
            if c1 > c2:
                children.append(c1)
            else:
                children.append(c2)
        return children


class RouletteElites(Elites):
    def ApplyElitism(self, pop, erate):

        total = sum([x.fitness for x in pop])
        children = []
        for j in range(int(erate * len(pop))):
            rval = random.randint(0, total)
            i = 0
            counter = pop[0].fitness
            while counter < rval and i < len(pop) - 1:
                counter += pop[i].fitness
                i += 1
            children.append(pop[i])
        return children


            
