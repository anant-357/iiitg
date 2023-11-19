#!/usr/bin/python

import sys

classes = {"Iris-setosa": [0, 0], "Iris-versicolor": [0, 0], "Iris-virginica": [0, 0]}

for line in sys.stdin:
    line = line.strip().split(',')
    n = line[0]
    sl = float(line[1])
    cl = line[2]
    count = int(line[3])
    
    classes[cl][1] = (classes[cl][1] * classes[cl][0] + sl) / (classes[cl][0] + 1)
    classes[cl][0] += count

print(classes)

