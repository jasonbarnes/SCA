from random import shuffle
import sys

f = open(sys.argv[1], 'r')
x = f.readlines()
shuffle(x)
for a in x:
    sys.stdout.write(a)
