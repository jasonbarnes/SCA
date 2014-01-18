#!/bin/bash

cat $1 | while read line
do
	cd results
	sudo tcpdump -i eth0 -w $line &> /dev/null &
	td_pid=$!
	wget -T 10 -t 2 $line &> /dev/null
	rm index.html
	kill $td_pid
	cd ..
done

sendmail $2 < done.txt
