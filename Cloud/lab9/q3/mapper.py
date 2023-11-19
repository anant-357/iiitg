#!/usr/bin/python
import sys
import math

cps = [
    [5.8, "Iris-setosa"],
    [6.1, "Iris-versicolor"],
    [6.3, "Iris-virginica"]
]

def ed(p1, p2):
    return abs(p1[0] - p2[0])

n = 0
for line in sys.stdin:
    n += 1
    line = line.strip()
    if line == "" or line == " " or line == None:
        continue
    dp = line.split(',')
    sl = float(dp[0])
    min_dis = 100000
    min_class = cps[0][1]
    for cp in cps:
        dis = ed([sl], [cp[0]])
        if dis < min_dis:
            min_class = cp[1]
            min_dis = dis

    print(f'{n},{sl},{min_class},1')
