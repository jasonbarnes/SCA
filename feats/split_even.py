import sys

f = open(sys.argv[1], 'r')
state = 0
for line in f:
 if(state == 0):
  state = 1
 else:
  state = 0
 if(state == 0):
  sys.stdout.write(line)
