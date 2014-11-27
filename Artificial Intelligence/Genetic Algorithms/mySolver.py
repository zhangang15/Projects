__author__ = 'ang'
import pygal
import GA
import Elitism
import Crossover
import Selector
import problemFactory
from buildGraph import graph
import TSPGAProblem
import sys


if __name__ == "__main__":
    # p = createNQueensProblem(4)
    #    p = createPatternMatchProblem('1101010011')
    number = sys.argv[1]

    if number == '1':

        class mySolver1(GA.GA, Elitism.DeterministicElites, Crossover.OnePointStringCrossover,
                        Selector.RouletteSelector):
            pass

        p = problemFactory.createPatternMatchProblem('1101010011')
        m = mySolver1(p)
        sol1 = []
        for j in range(20):
            m.run()
            data = m.run()
            count = 0
            for i in data:
                if i == 10:
                    count += 1
            sol1.append(count)


        class mySolver2(GA.GA, Elitism.DeterministicElites, Crossover.OnePointStringCrossover,
                        Selector.TournamentSelector):
            pass

        p = problemFactory.createPatternMatchProblem('1101010011')
        m = mySolver2(p)
        sol2 = []
        for k in range(20):
            m.run()
            data = m.run()
            count = 0
            for i in data:
                if i == 10:
                    count += 1
            sol2.append(count)

        line_chart = pygal.Line()
        line_chart.title = 'Pattern (in %)'

        line_chart.add('Roulette_Selector', sol1)
        line_chart.add('Tournament_Selector', sol2)

        line_chart.render_to_file('Same_Crossover_Different_Selector_Pattern_chart.svg')

    elif number == '2':

        #
        class mySolver3(GA.GA, Elitism.DeterministicElites, Crossover.OnePointStringCrossover,
                        Selector.RouletteSelector):
            pass

        p = problemFactory.createPatternMatchProblem('1101010011')
        m = mySolver3(p, mutationRate=0.06)
        sol1 = []
        for j in range(20):
            m.run()
            data = m.run()
            count = 0
            for i in data:
                if i == 10:
                    count += 1
            sol1.append(count)


        class mySolver4(GA.GA, Elitism.DeterministicElites, Crossover.TwoPointStringCrossover,
                        Selector.RouletteSelector):
            pass

        p = problemFactory.createPatternMatchProblem('1101010011')
        m = mySolver4(p, mutationRate=0.06)
        sol2 = []
        for j in range(20):
            m.run()
            data = m.run()
            count = 0
            for i in data:
                if i == 10:
                    count += 1
            sol2.append(count)

        line_chart = pygal.Line()
        line_chart.title = 'Pattern (in %)'

        line_chart.add('One_Point_Crossover', sol1)
        line_chart.add('two_Point_Crossover', sol2)

        line_chart.render_to_file('Same_Selector_Different_Crossover_Pattern_chart.svg')


    elif number == '3':

        #
        class mySolver5(GA.GA, Elitism.DeterministicElites, Crossover.OnePointStringCrossover,
                        Selector.RouletteSelector):
            pass

        p = problemFactory.createNQueensProblem(8)
        m = mySolver5(p)
        sol1 = []
        for i in range(20):
            m.run()
            data = m.run()
            count = 0
            for j in data:
                if j == 28:
                    count += 1
            sol1.append(count)

        class mySolver6(GA.GA, Elitism.DeterministicElites, Crossover.OnePointStringCrossover,
                        Selector.TournamentSelector):
            pass

        p = problemFactory.createNQueensProblem(8)
        m = mySolver6(p)
        sol2 = []
        for i in range(20):
            m.run()
            data = m.run()
            count = 0
            for j in data:
                if j == 28:
                    count += 1
            sol2.append(count)

        line_chart = pygal.Line()
        line_chart.title = '8_Queen (in %)'

        line_chart.add('Roulette_Selector', sol1)
        line_chart.add('Tournament_Selector', sol2)

        line_chart.render_to_file('Same_Crossover_Different_Selector_Queen_chart.svg')

    elif number == '4':

        #
        class mySolver7(GA.GA, Elitism.DeterministicElites, Crossover.OnePointStringCrossover,
                        Selector.TournamentSelector):
            pass

        p = problemFactory.createNQueensProblem(8)
        m = mySolver7(p)
        sol1 = []
        for i in range(20):
            m.run()
            data = m.run()
            count = 0
            for j in data:
                if j == 28:
                    count += 1
            sol1.append(count)

        class mySolver8(GA.GA, Elitism.DeterministicElites, Crossover.TwoPointStringCrossover,
                        Selector.TournamentSelector):
            pass

        p = problemFactory.createNQueensProblem(8)
        m = mySolver8(p)
        sol2 = []
        for i in range(20):
            m.run()
            data = m.run()
            count = 0
            for j in data:
                if j == 28:
                    count += 1
            sol2.append(count)

        line_chart = pygal.Line()
        line_chart.title = '8_Queen (in %)'

        line_chart.add('One_Point_Crossover', sol1)
        line_chart.add('Two_Point_Crossover', sol2)

        line_chart.render_to_file('Same_Selector_Different_Crossover_Queen_chart.svg')

    elif number == '5':

        #
        #
        class mySolver9(GA.GA, Elitism.DeterministicElites, Crossover.PermutationCrossover,
                        Selector.TournamentSelector):
            def setg(self, g):
                self.tspgraph = g

        g = graph('sfdata.txt')
        p = TSPGAProblem.TSPGAProblem(lambda x: -TSPGAProblem.computeTourCost(g, x), g)
        m = mySolver9(p)
        m.setg(g)
        sol1 = []
        for i in range(20):
            m.run()
            total = m.run()
            t = 0
            for i in total:
                t += i

            average = t / len(total)
            sol1.append(average)

        class mySolver10(GA.GA, Elitism.DeterministicElites, Crossover.GreedyCrossover, Selector.TournamentSelector):
            def setg(self, g):
                self.tspgraph = g

        g = graph('sfdata.txt')
        p = TSPGAProblem.TSPGAProblem(lambda x: -TSPGAProblem.computeTourCost(g, x), g)
        m = mySolver9(p)
        m.setg(g)
        sol2 = []
        for i in range(20):
            m.run()
            total = m.run()
            t = 0
            for i in total:
                t += i

            average = t / len(total)
            sol2.append(average)

        line_chart = pygal.Line()
        line_chart.title = 'TSP (in %)'

        line_chart.add('Permutation_Crossover', sol1)
        line_chart.add('Greedy_Crossover', sol2)

        line_chart.render_to_file('Same_Selector_Different_Crossover_TSP_chart.svg')


    elif number == '6':

        #
        class mySolver11(GA.GA, Elitism.DeterministicElites, Crossover.PermutationCrossover,
                         Selector.TournamentSelector):
            def setg(self, g):
                self.tspgraph = g

        g = graph('sfdata.txt')
        p = TSPGAProblem.TSPGAProblem(lambda x: -TSPGAProblem.computeTourCost(g, x), g)
        m = mySolver11(p, mutationRate=0.05)
        m.setg(g)
        sol1 = []
        for i in range(20):
            m.run()
            total = m.run()
            t = 0
            for i in total:
                t += i

            average = t / len(total)
            sol1.append(average)

        class mySolver12(GA.GA, Elitism.DeterministicElites, Crossover.PermutationCrossover,
                         Selector.TournamentSelector):
            def setg(self, g):
                self.tspgraph = g

        g = graph('sfdata.txt')
        p = TSPGAProblem.TSPGAProblem(lambda x: -TSPGAProblem.computeTourCost(g, x), g)
        m = mySolver12(p, mutationRate=0.08)
        m.setg(g)
        sol2 = []
        for i in range(20):
            m.run()
            total = m.run()
            t = 0
            for i in total:
                t += i

            average = t / len(total)
            sol2.append(average)

        line_chart = pygal.Line()
        line_chart.title = 'TSP (in %)'

        line_chart.add('Mutation_Rate=0.05', sol1)
        line_chart.add('Mutation_Rate=0.08', sol2)

        line_chart.render_to_file('Different_Mutation_Rate_TSP_chart.svg')

    else:
        print 'Please input a number between 1 to 6.'


