#!/bin/bash

./prepare_data.sh

R CMD BATCH neural.R
cat neural.Rout
rm neural.Rout

#./match neural.out r_test_labels.csv > neural_results.txt
