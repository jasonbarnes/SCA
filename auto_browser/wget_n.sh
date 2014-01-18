#!/bin/bash

infile=$1
outdir=$2
count=$3
email=$3

while [ $count -gt 0 ]
do
	mkdir $outdir.$count
	cat $infile | while read line
	do
		cd $outdir.$count
		sudo tcpdump -i eth0 -w $line &> /dev/null &
		td_pid=$!
		wget -T 10 -t 2 $line &> /dev/null
		rm index.html &> /dev/null
		kill $td_pid
		cd ..
	done
	count=$(($count - 1))
done

sendmail $2 < done.txt
