#!/bin/bash

#splits the first file 50/50 into the next two files

infile=$1
trainfile=$2
testfile=$3

lines=$(wc -l < $infile)
lines=$(expr $lines / 2)

head -n $lines $infile > $trainfile
tail -n $lines $infile > $testfile
