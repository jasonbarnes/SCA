import sys

a = sys.argv[1].split('-')
b = int(a[len(a)-1])
if b <= int(sys.argv[2]):
	print sys.argv[1]
