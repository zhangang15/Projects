import BitStringGAProblem
import math
import TSPGAProblem

## For demonstration purposes only, you should not actually use this function ...
import Crossover
import Elitism
import GA
import Selector
from buildGraph import graph


def allOnes(chr):
    return len([x for x in chr.bitstring if x == '1'])


def createPatternMatchProblem(pattern):
    ### Input: pattern is a string of 0's and 1's (like 1101101)
    ###
    ### returns BitStringGAProblem.BitStringGAProblem(fitnessFunc, len)
    ###   where fitnessFunc is a function that takes as input a chromosome,
    ###   and returns the number of bits that are the same as pattern.
    ###   len should be the length of the pattern
    ###
    ###   see the allOnes function as an example of a fitness function
    def fitnessFunc(chromosome, pattern):
        fitness = 0

        for i in range(len(pattern)):
            if chromosome.bitstring[i] == pattern[i]:
                fitness += 1
        return fitness


    return BitStringGAProblem.BitStringGAProblem(lambda x: fitnessFunc(x, pattern), len(pattern))


def createNQueensProblem(n):
    ### Input: Size of a chess board n (you may assume n is a power of 2
    ###
    ### returns BitStringGAProblem.BitStringGAProblem(fitnessFunc, len)
    ###   where fitnessFunc is a function that takes as input a chromosome
    ###   representing a chess board, and returns the number of *non-conflicts*
    ###   on the board (that is, higher numbers == fewer conflicts == good == more fit,
    ###   lower numbers == more conflicts == bad == less fit)
    ###
    ###   len should be n * lg n (since for a board of size n, we need lg n
    ###    bits to represent a single queen)
    ###
    ###  For example, for n = 4, a chromosome would consist of 8 bits.  The board:
    ###
    ###     Q...
    ###     ...Q
    ###     .Q..
    ###     ..Q.
    ###
    ###  Would have the representation 00110110 (since the queen in the first row is
    ###    in column 0 (00), the queen in the second row is in column 3 (11), the queen in the
    ###    third row is in column 1 (01) and the queen in the last row is in column 2 (10)
    ###    last queen is in row 11 (3)
    def isAttacking(row1, col1, row2, col2):
        return (row1 == row2 or (abs(row2 - row1) == abs(col2 - col1)))


    def fitnessFunc(chromosome, n):
        fitness = 0
        queenset = []
        bitPerQueen = int(math.log(n, 2))

        for i in range(0, len(chromosome.bitstring), bitPerQueen):
            queenset.append(int(chromosome.bitstring[i: i + bitPerQueen]))

        for j in range(0, n):
            for k in range(j, n):
                if (j != k):
                    if not (isAttacking(queenset[j], j, queenset[k], k)):
                        fitness += 1

        return fitness

    return BitStringGAProblem.BitStringGAProblem(lambda x: fitnessFunc(x, n), int(n * math.log(n, 2)))


