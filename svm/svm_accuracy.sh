#!/bin/bash

train_file=$1
test_file=$2
c=$3
g=$4

./svm-train -c $c -g $g -q $train_file svm.model
./svm-predict -q $test_file svm.model results.out
./accuracy results.out $test_file
