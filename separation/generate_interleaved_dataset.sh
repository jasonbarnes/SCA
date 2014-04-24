#!/bin/bash

max=$1
num_streams=$2
iterations=$3
csv_filename=$4

while [ $iterations -gt 0 ]
do
	./interleave $csv_filename $(./rand_draws $max $num_streams)
	iterations=$(( $iterations - 1 ))
done
