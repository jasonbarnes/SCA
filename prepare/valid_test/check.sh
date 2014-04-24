#!/bin/bash

count=100

cur_dir=$(pwd)
rm -rf web_count
mkdir web_count
cd ..
for dir in results.*
do
	cd $dir
	ls -l|grep "     24" > $cur_dir/check.txt
	cd $cur_dir
	while read p
	do
		./last_arg $p > py.out
		target=$(cat py.out)
		cd web_count
		echo "Line" >> $target
		cd ..
	done < $cur_dir/check.txt
	cd ..
done
cd $cur_dir/web_count
rm $cur_dir/missing.txt
total=0
for a in *
do
	value=$(wc -l < $a)
	if [ $value -ge $count ]
	then
		echo $a >> $cur_dir/missing.txt
		total=1
	fi
done
#if [ $total -eq 0 ]
#then
#	echo "Test passed"
#fi
