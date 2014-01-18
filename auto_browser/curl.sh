#!/bin/bash

cat $1 | while read line
do
	curl $line &> /dev/null
done
