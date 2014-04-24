#!/usr/bin/python

import sys

fa = open(sys.argv[1])
fb = open(sys.argv[2])

a = fa.readlines()

for b in fb:
	sys.stdout.write(a[int(b)])
