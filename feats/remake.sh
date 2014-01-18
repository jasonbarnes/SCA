#!/bin/bash

gcc feature_extraction.c fe_total_pkt_up.c fe_total_pkt_down.c fe_percent_up.c fe_percent_down.c fe_total_byte_down.c fe_total_byte_up.c fe_html_markers.c fe_size_markers.c fe_num_markers.c fe_size_histogram.c fe_total_time.c fe_raw_meta_info.c fe_component_vectors.c fe_two_d_space.c fe_one_d_space.c default_fe.c fe_unique_up.c fe_unique_down.c feature_config.c countable_set.c pcap_helper.c -o feature_extraction -lpcap -lm

gcc generate_size_marker.c pcap_helper.c countable_set.c round_double.c -o generate_size_marker -lpcap -lm
gcc generate_num_marker.c pcap_helper.c countable_set.c round_double.c -o generate_num_marker -lpcap -lm
gcc generate_size_histogram.c pcap_helper.c countable_set.c round_double.c -o generate_size_histogram -lpcap -lm
gcc generate_html_marker.c pcap_helper.c countable_set.c round_double.c -o generate_html_histogram -lpcap -lm

