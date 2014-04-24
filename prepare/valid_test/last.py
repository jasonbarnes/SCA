import sys

line = sys.argv[len(sys.argv)-1]
split_line = line.split(" ")
print split_line[len(split_line)-1]
