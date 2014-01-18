#!/bin/bash

cp test.out svm_train.txt
cp train.out svm_test.txt

python svm.py > svm_results.txt
