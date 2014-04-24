#!/bin/bash

line=$1


for file in fe_*.c
do
	echo $line > temp
	cat $file >> temp
	mv temp $file
done
