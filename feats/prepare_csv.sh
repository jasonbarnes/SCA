#!/bin/bash

maindir=$1
count=$2
outfile=$3
alexa=$4

while [ $count -gt 0 ]
do
	cd ../auto_browser/$maindir.$count
	for line in *
	do
		cp $line ../../feats/
		cd ../../feats/
		./generate_label $alexa $line > temp.txt
		label=$(cat temp.txt)
		./feature_extraction $line $label >> $outfile
		rm $line
		cd ~/sca/auto_browser/$maindir.$count
	done
	cd ../../feats/
	count=$(($count - 1))
done
