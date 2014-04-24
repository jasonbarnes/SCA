#!/bin/bash

count=0
touch unshuf.var
rm unshuf.var
limit=$(wc -l < $1)
while [ $count -lt $limit ]
do
	echo $count >> unshuf.var
	count=$(( $count + 1 ))
done
cat unshuf.var | shuf
