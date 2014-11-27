__author__ = 'anglies'

import sys
import random


def run():
    options = ["BAR", "BELL", "LEMON", "CHERRY"]
    coin = 10
    time = 0


    while coin > 0:
        item1 = random.choice(options)
        item2 = random.choice(options)
        item3 = random.choice(options)
        time += 1
        coin = coin - 1
        if item1 == item2 == item3 == "BAR":
            coin += 20
        elif item1 == item2 == item3 == "BELL":
            coin += 15
        elif item1 == item2 == item3 == "LEMON":
            coin += 5
        elif item1 == item2 == item3 == "CHERRY":
            coin += 3
        elif item1 == item2 == "CHERRY":
            coin += 2
        elif item1 == "CHERRY":
            coin += 1

    return time

if __name__ == '__main__':
    if len(sys.argv) < 1:
        print "Please input running times!"
        exit(1)
    times = int(sys.argv[1])
    total = 0
    total_Times = []
    for i in range(0, times):
        t = run()
        total_Times.append(t)
        total += t

    print "Mean is :", str(total/times)

    total_Times.sort()
    print "Median is :", str(total_Times[times/2])


