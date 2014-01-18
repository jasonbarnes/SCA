#!/bin/bash

count=0

while [ $count -lt 2000 ]
do
	echo $count
	count=$(($count + 1))
done
