#!/bin/bash

./strip_class train.out > r_train_labels.csv
./strip_data train.out > r_train_data.csv
./strip_class test.out > r_test_labels.csv
./strip_data test.out > r_test_data.csv
