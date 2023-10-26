#!/usr/bin/python
import sys

punc = '''!()-[]{};:'"\,<>./?@#$%^&*_~'''
for line in sys.stdin:
    line = line.strip()
    words = line.split()
    for word in words:
        for char in word:
            if char in punc:
                word = word.replace(char, "")
        word = word.lower()
        print(f'{word}')
