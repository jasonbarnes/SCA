#!/usr/bin/python
import sys

fa = open(sys.argv[1], "r")
fb = open(sys.argv[2], "r")

total=0
correct=0
for a in fa:
	b = fb.readline()
	sa = a.split(',')
	sb = b.split(',')
	int_a = []
	int_b = []
	for i in sa:
		int_a.append(int(i))
	for i in sb:
		int_b.append(int(i))
	set_a = set(int_a)
	set_b = set(int_b)
	result=set_a.intersection(set_b)
	total = total + 1
	if(len(result) > 0):
		correct = correct + 1

print float(correct)/float(total)
