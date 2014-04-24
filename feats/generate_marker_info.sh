#!/bin/bash

#Remove all existing marker info files
rm html_marker.dat
rm size_marker_up.dat
rm size_marker_down.dat
rm num_marker_up.dat
rm num_marker_down.dat
rm size_histogram_up.dat
rm size_histogram_down.dat
rm size_total_up.dat
rm size_total_down.dat

line=$1
./generate_html_marker $line html_marker.dat
./generate_size_num_marker $line size_marker_up.dat size_marker_down.dat num_marker_up.dat num_marker_down.dat
./generate_size_histogram $line size_histogram_up.dat size_histogram_down.dat
./generate_total_marker $line size_total_up.dat size_total_down.dat
