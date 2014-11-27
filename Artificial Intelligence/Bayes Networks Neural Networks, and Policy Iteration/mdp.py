### code for representing/solving an MDP

import random


class State:
    def __init__(self):
        self.utility = 0
        self.reward = 0.0
        ### an action maps to a list of probability/state pairs
        self.transitions = {}
        self.actions = []
        self.policy = None
        self.coords = 0
        self.isGoal = False

    def computeEU(self, action):
        return sum([trans[0] * trans[1].utility
                    for trans in self.transitions[action]])

    def selectBestAction(self):
        best = max([(self.computeEU(a), a) for a in self.actions])
        return best


class Map:
    def __init__(self):
        self.states = {}
        self.error = 0.01
        self.gamma = 0.8

    ### you write this method
    def valueIteration(self):
        U_primary = dict([state, 0] for state in self.states)
        while True:
            U = U_primary.copy()
            delta = 0
            for state in self.states:
                if self.states[state].actions:
                    U_primary[state] = self.states[state].reward + self.gamma * self.states[state].selectBestAction()[0]

                    self.states[state].utility = U_primary[state]
                    delta = max(delta, abs(U_primary[state] - U[state]))
            if delta < self.error * (1 - self.gamma) / self.gamma:
                return

    ### you write this method
    def policyIteration(self):
        U_primary = dict([state, 0] for state in self.states)

        pi = {}
        for state in self.states:
            # print self.states[state].actions
            if len(self.states[state].actions) > 0:
                pi[state] = random.choice(self.states[state].actions)
            else:
                pi[state] = None
        # print pi
        while True:

            for state in self.states:
                if self.states[state].actions:
                    U_primary[state] = self.states[state].reward + self.gamma * self.states[state].selectBestAction()[0]
                    self.states[state].utility = U_primary[state]
            unchange = True
            for state in self.states:
                if self.states[state].actions:
                    if self.states[state].selectBestAction()[1] != pi[state]:
                        pi[state] = self.states[state].selectBestAction()[1]
                        unchange = False
            if unchange:
                for state in self.states:
                    self.states[state].policy = pi[state]
                return


### you shouldn't need to change this.
def makeRNProblem():
    m = Map()
    for i in range(12):
        m.states[i] = State()
        m.states[i].coords = i
    ## create goal states
    m.states[0].isGoal = True
    m.states[10].utility = 1.0
    m.states[10].isGoal = True
    m.states[11].utility = -1.0
    m.states[11].isGoal = True

    ## build nonGoal states
    for i in range(1, 10):
        m.states[i].reward = -0.04
        m.states[i].isGoal = False
        m.states[i].actions = ['left', 'right', 'up', 'down']
    m.states[1].transitions = {'left': [(0.9, m.states[1]),
                                        (0.1, m.states[4])],
                               'right': [(0.8, m.states[2]),
                                         (0.1, m.states[1]),
                                         (0.1, m.states[4])],
                               'up': [(0.8, m.states[1]),
                                      (0.1, m.states[1]),
                                      (0.1, m.states[2])],
                               'down': [(0.8, m.states[4]),
                                        (0.1, m.states[1]),
                                        (0.1, m.states[2])]}

    m.states[2].transitions = {'left': [(0.8, m.states[1]),
                                        (0.1, m.states[2]),
                                        (0.1, m.states[2])],
                               'right': [(0.8, m.states[3]),
                                         (0.1, m.states[2]),
                                         (0.1, m.states[2])],
                               'up': [(0.8, m.states[2]),
                                      (0.1, m.states[1]),
                                      (0.1, m.states[3])],
                               'down': [(0.8, m.states[2]),
                                        (0.1, m.states[1]),
                                        (0.1, m.states[3])]}
    m.states[3].transitions = {'left': [(0.8, m.states[2]),
                                        (0.1, m.states[5]),
                                        (0.1, m.states[3])],
                               'right': [(0.8, m.states[10]),
                                         (0.1, m.states[3]),
                                         (0.1, m.states[5])],
                               'up': [(0.8, m.states[3]),
                                      (0.1, m.states[2]),
                                      (0.1, m.states[10])],
                               'down': [(0.8, m.states[5]),
                                        (0.1, m.states[2]),
                                        (0.1, m.states[10])]}
    m.states[4].transitions = {'left': [(0.8, m.states[4]),
                                        (0.1, m.states[6]),
                                        (0.1, m.states[1])],
                               'right': [(0.8, m.states[4]),
                                         (0.1, m.states[1]),
                                         (0.1, m.states[6])],
                               'up': [(0.8, m.states[1]),
                                      (0.1, m.states[4]),
                                      (0.1, m.states[4])],
                               'down': [(0.8, m.states[6]),
                                        (0.1, m.states[4]),
                                        (0.1, m.states[4])]}
    m.states[5].transitions = {'left': [(0.8, m.states[5]),
                                        (0.1, m.states[8]),
                                        (0.1, m.states[3])],
                               'right': [(0.8, m.states[11]),
                                         (0.1, m.states[8]),
                                         (0.1, m.states[3])],
                               'up': [(0.8, m.states[3]),
                                      (0.1, m.states[5]),
                                      (0.1, m.states[11])],
                               'down': [(0.8, m.states[8]),
                                        (0.1, m.states[5]),
                                        (0.1, m.states[11])]}
    m.states[6].transitions = {'left': [(0.8, m.states[6]),
                                        (0.1, m.states[6]),
                                        (0.1, m.states[4])],
                               'right': [(0.8, m.states[7]),
                                         (0.1, m.states[4]),
                                         (0.1, m.states[6])],
                               'up': [(0.8, m.states[4]),
                                      (0.1, m.states[6]),
                                      (0.1, m.states[7])],
                               'down': [(0.8, m.states[6]),
                                        (0.1, m.states[6]),
                                        (0.1, m.states[7])]}
    m.states[7].transitions = {'left': [(0.8, m.states[6]),
                                        (0.1, m.states[7]),
                                        (0.1, m.states[7])],
                               'right': [(0.8, m.states[8]),
                                         (0.1, m.states[7]),
                                         (0.1, m.states[7])],
                               'up': [(0.8, m.states[7]),
                                      (0.1, m.states[6]),
                                      (0.1, m.states[8])],
                               'down': [(0.8, m.states[7]),
                                        (0.1, m.states[6]),
                                        (0.1, m.states[8])]}
    m.states[8].transitions = {'left': [(0.8, m.states[7]),
                                        (0.1, m.states[5]),
                                        (0.1, m.states[8])],
                               'right': [(0.8, m.states[9]),
                                         (0.1, m.states[5]),
                                         (0.1, m.states[8])],
                               'up': [(0.8, m.states[5]),
                                      (0.1, m.states[7]),
                                      (0.1, m.states[9])],
                               'down': [(0.8, m.states[8]),
                                        (0.1, m.states[7]),
                                        (0.1, m.states[9])]}
    m.states[9].transitions = {'left': [(0.8, m.states[8]),
                                        (0.1, m.states[9]),
                                        (0.1, m.states[11])],
                               'right': [(0.8, m.states[9]),
                                         (0.1, m.states[11]),
                                         (0.1, m.states[9])],
                               'up': [(0.8, m.states[11]),
                                      (0.1, m.states[8]),
                                      (0.1, m.states[9])],
                               'down': [(0.8, m.states[9]),
                                        (0.1, m.states[9]),
                                        (0.1, m.states[8])]}
    return m


### you shouldn't need to change this either.
### problem setup - agent can move up, down, left, right. moves in the 
### intended direction 0.7, others 0.1 each.

def make2DProblem():
    def opposite(action):
        if action == 'left':
            return 'right'
        if action == 'right':
            return 'left'
        if action == 'up':
            return 'down'
        if action == 'down':
            return 'up'

    def left(action):
        if action == 'left':
            return 'down'
        if action == 'right':
            return 'up'
        if action == 'up':
            return 'left'
        if action == 'down':
            return 'right'

    def right(action):
        if action == 'right':
            return 'down'
        if action == 'left':
            return 'up'
        if action == 'up':
            return 'right'
        if action == 'down':
            return 'left'

    def getSuccessor(s, action):

        if action == 'up':
            return (s[0], s[1] - 1)
        elif action == 'down':
            return (s[0], s[1] + 1)
        elif action == 'left':
            return (s[0] - 1, s[1])
        else:
            return (s[0] + 1, s[1])


    walls = [(1, 1), (4, 1), (5, 1), (6, 1), (7, 1), (8, 1), (1, 2), (7, 2), (1, 3), (5, 3),
             (7, 3), (1, 4), (5, 4), (7, 4), (1, 5), (5, 5), (7, 5), (1, 6), (5, 6),
             (7, 6), (1, 7), (5, 7), (7, 7), (1, 8), (3, 8), (4, 8), (5, 8),
             (7, 8), (1, 9)]
    actions = ['left', 'right', 'up', 'down']

    def filterState(oldState, newState):
        if (newState[0] < 0 or newState[1] < 0 or newState[0] > 9 or
                    newState[1] > 9 or newState in walls):
            return oldState
        else:
            return newState

    m = Map()
    for i in range(10):
        for j in range(10):
            m.states[(i, j)] = State()
            m.states[(i, j)].coords = (i, j)
            m.states[(i, j)].isGoal = False
            m.states[(i, j)].actions = actions

    m.states[(0, 9)].isGoal = True
    m.states[(9, 9)].isGoal = True
    m.states[(9, 0)].isGoal = True

    m.states[(0, 9)].utility = 1.0
    m.states[(9, 9)].utility = -1.0
    m.states[(9, 0)].utility = 1.0

    ### fill in transition probabilities
    for s in m.states.items():
        for a in actions:
            s[1].transitions[a] = [(0.7, m.states[filterState(s[0], getSuccessor(s[0], a))]),
                                   (0.1, m.states[filterState(s[0], getSuccessor(s[0], opposite(a)))]),
                                   (0.1, m.states[filterState(s[0], getSuccessor(s[0], left(a)))]),
                                   (0.1, m.states[filterState(s[0], getSuccessor(s[0], right(a)))])]

    return m
        
            
            
