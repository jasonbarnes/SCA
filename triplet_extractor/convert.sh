#!/bin/bash

ab_min=1
ab_max=100
alexa_num=500
cur_dir=$(pwd)
count=0

rm -rf ~/ab_convert
mkdir ~/ab_convert
mkdir ~/ab_convert/pcap-logs
while read line
do
	cd $cur_dir
	rm valid_logs.txt
	cd ~/liberatore/pcap-logs/$line
	for a in *
	do
		python $cur_dir/max_site.py $a $alexa_num >> $cur_dir/valid_logs.txt
	done
	if [ $ab_min -gt $ab_max ]
	then
		ab_min=1
	fi
	mkdir ~/ab_convert/pcap-logs/$line
	cd ~/sca/auto_browser/results.$ab_min
	rm $cur_dir/ab_logs.txt
	for a in *
	do
		echo $a >> $cur_dir/ab_logs.txt
	done
	max_count=$(wc -l < $cur_dir/ab_logs.txt)
	count=1
	while [ "$count" -le "$max_count" ]
	do
		fileA=$(head -n $count $cur_dir/valid_logs.txt|tail -n 1)
		fileB=$(head -n $count $cur_dir/ab_logs.txt|tail -n 1)
		cp ~/sca/auto_browser/results.$ab_min/$fileB ~/ab_convert/pcap-logs/$line/$fileA
		count=$(($count + 1))
	done
done < $cur_dir/valid_dirs.txt
