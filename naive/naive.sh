#!/bin/bash

./prepare_data.sh

R CMD BATCH naive.R
rm naive.Rout

./match naive.out r_test_labels.csv > naive_results.txt
sendmail jaccountnumber2@gmail.com < done.txt
