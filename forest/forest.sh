#!/bin/bash

./prepare_data.sh

R CMD BATCH forest.R
cat forest.Rout
rm forest.Rout

#./match forest.out r_test_labels.csv > neural_results.txt
