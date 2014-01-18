#!/bin/bash

maindir=$1
count=$2

while [ $count -gt 0 ]
do
	mkdir $maindir.$count
	cd ../auto_browser/$maindir.$count
	for line in *
	do
		../../prepare/prepare_pcap $line ../../prepare/$maindir.$count/$line
	done
	cd ../../prepare/
	count=$(($count - 1))
done
