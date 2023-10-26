#!/usr/bin/python
import sys
import math

cps=[
[5.8,4.0,"Iris-setosa"],
[6.1,2.8,"Iris-versicolor"],
[6.3,2.7,"Iris-virginica"]
]

def ed(p1,p2):
    return math.sqrt((p1[0]-p2[0])**2+(p1[1]-p2[1])**2)
n = 0
for line in sys.stdin:
    n+=1
    line = line.strip()
    if line == "" or line == " " or line == None:
        continue
    dp = line.split(',')
    sl = float(dp[0])
    sw = float(dp[1])
    min_dis = 100000
    min_class = cps[0][2]
    for cp in cps:
        dis = ed([sl,sw],[cp[0], cp[1]])
        if dis < min_dis:
            min_class = cp[2]
            min_dis = dis

    print(f'{n},{sl},{sw},{min_class},1')
