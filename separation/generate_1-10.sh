#!/bin/bash
touch result0.trip
rm result0.trip
touch result0.trip
count=1
max=10
filename="test_lib_nonempty.out"
while [ $count -le $max ]
do
prev=$(( $count - 1 ))
cp result$prev.trip result$count.trip
./interleave $filename 10000 $(grep -c : $filename) $count >> result$count.trip
count=$(( $count + 1 ))
done

