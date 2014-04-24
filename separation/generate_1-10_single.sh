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
echo "./interleave_seq $filename $(grep -c : $filename) $(grep -c : $filename) $count > result$count.trip"
./interleave_seq $filename $(grep -c : $filename) $(grep -c : $filename) $count > result$count.trip
count=$(( $count + 1 ))
done
