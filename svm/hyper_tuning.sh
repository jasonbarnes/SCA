#!/bin/bash

log_c_start=$1
log_c_steps=$2
log_g_start=$3
log_g_steps=$4

count_c=0
max_accuracy=0
new_max_accuracy=0

while [ $count_c -lt $log_c_steps ]
do
	count_g=0
	while [ $count_g -lt $log_g_steps ]
	do
		cur_c=$(./log_calc $log_c_start $count_c)
		cur_g=$(./log_calc $log_g_start $count_g)
		cur_accuracy=$(./svm_accuracy.sh train.out test.out $cur_c $cur_g )
		./max_float $cur_accuracy $max_accuracy > float.temp
		new_max_accuracy=$(cat float.temp)
		if [ "$new_max_accuracy" != "$max_accuracy" ]
		then
			max_accuracy=$new_max_accuracy
			max_c=$cur_c
			max_g=$cur_g
		fi
		count_g=$(( $count_g + 1 ))
	done
	count_c=$(( $count_c + 1 ))
done

echo $max_c > best_c.txt
echo $max_g > best_g.txt
echo $max_accuracy > best_c_g_accuracy.txt
