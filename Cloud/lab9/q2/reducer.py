#!/usr/bin/python

import sys

curr_word = None
curr_count = 0

for line in sys.stdin:
    line=line.strip().split(',')
    word,count = line[0],int(line[1])
    if curr_word == None:
        curr_word = word
        curr_count = count
    elif curr_word == word:
        curr_count += count
    else:
        print(f'{curr_word}, {curr_count}')
        curr_word = word
        curr_count = count
print(f'{curr_word}, {curr_count}')
