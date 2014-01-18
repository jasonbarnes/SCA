#!/bin/bash

echo "Remaking"
./remake.sh
echo "Extracting data from $1"
./extract_data $1 > raw.out
echo "Sorting raw data"
sort raw.out > raw_sorted.out
echo "Splitting sorted data into flows"
./organize_data raw_sorted.out $2
echo "Reconstructing flows"
./sort.sh
echo "Calculating relative timestamp"
for f in sorted*
do
	./relative_timestamp $f > relative.$f
done
mkdir "processed_data"
mv relative.sorted* processed_data/
rm *.log
