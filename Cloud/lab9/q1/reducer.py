#!/usr/bin/python

import sys

curr_word = None

for line in sys.stdin:
    word = line.strip()
    if curr_word == None:
        curr_word = word
    elif curr_word == word:
        continue
    else:
        print(f'{curr_word}')
        curr_word = word
print(f'{curr_word}')
