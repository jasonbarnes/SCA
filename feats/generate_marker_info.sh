#!/bin/bash

maindir=$1
count=$2
outfile=$3
alexa=$4

#Remove all existing marker info files
rm html_marker.dat
rm size_marker.dat
rm num_marker.dat
rm size_histogram.dat
rm unique_sizes.dat

while [ $count -gt 0 ]
do
	cd ../auto_browser/$maindir.$count
	for line in *
	do
		cp $line ../../feats/
		cd ../../feats/
		./generate_html_marker $line html_marker.dat
		./generate_size_marker $line size_marker_up.dat size_marker_down.dat
		./generate_num_marker $line num_marker_up.dat num_marker_down.dat
		./generate_size_histogram $line size_histogram_up.dat size_histogram_down.dat
		rm $line
		cd ~/sca/auto_browser/$maindir.$count
	done
	cd ../../feats/
	count=$(($count - 1))
done
