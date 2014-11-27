__author__ = 'anglies'


import mdp
m = mdp.makeRNProblem()
m.valueIteration()
print [(s.coords, s.utility) for s in m.states.values()]
m.policyIteration()
print [(s.coords, s.utility, s.policy) for s in m.states.values()]