#!/bin/bash

cat $1 | while read line
do
	cd results
	sudo tcpdump -i eth0 -w $line &
	td_pid=$!
	wget  $line
	rm index.html
	kill $td_pid
	cd ..
	break
done
