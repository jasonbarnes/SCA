#!/bin/bash

dir_prefix=$1
output=$2

rm -rf pcap-logs
mkdir pcap-logs
for a in $dir_prefix.*
do
	cp -r $a pcap-logs/
done
tar -czf $output pcap-logs
rm -rf pcap-logs
