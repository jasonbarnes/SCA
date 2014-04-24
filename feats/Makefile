all: generate_html_marker prepare_pca generate_size_num_marker generate_size_histogram generate_total_marker feature_extraction strip_labels
CC=gcc

generate_headers = round_double.h countable_set.h trip_trace.h
generate_objects = round_double.o countable_set.o trip_trace.o

fe_objects = fe_total_pkt_up.o fe_total_pkt_down.o fe_percent_up.o fe_percent_down.o fe_total_byte_down.o fe_total_byte_up.o fe_html_markers.o fe_size_markers.o fe_num_markers.o fe_size_histogram.o fe_total_time.o fe_raw_meta_info.o fe_component_vectors.o fe_two_d_space.o fe_one_d_space.o default_fe.o fe_unique_up.o fe_unique_down.o feature_config.o countable_set.o trip_trace.o round_double.o fe_rtt.o

fe_headers = feature_extraction.h feature_config.h trip_trace.h round_double.h countable_set.h

strip_labels: strip_labels.o trip_trace.o
	$(CC) strip_labels.o trip_trace.o -o strip_labels
strip_labels.o: strip_labels.c trip_trace.h
	$(CC) -c strip_labels.c -o strip_labels.o

prepare_pca: prepare_pca.o trip_trace.o
	$(CC) prepare_pca.o trip_trace.o -o prepare_pca -lm
prepare_pca.o: prepare_pca.c trip_trace.h
	$(CC) -c prepare_pca.c -o prepare_pca.o

trip_trace.o: trip_trace.c trip_trace.h
	$(CC) -c trip_trace.c -o trip_trace.o
countable_set.o: countable_set.c countable_set.h
	$(CC) -c countable_set.c -o countable_set.o
round_double.o: round_double.c round_double.h
	$(CC) -c round_double.c -o round_double.o

generate_html_marker: $(generate_objects) generate_html_marker.o
	$(CC) generate_html_marker.o $(generate_objects) -o generate_html_marker -lpcap -lm
generate_html_marker.o: $(generate_headers)
	$(CC) -c generate_html_marker.c -o generate_html_marker.o

generate_size_num_marker: $(generate_objects) generate_size_num_marker.o
	$(CC) generate_size_num_marker.o $(generate_objects) -o generate_size_num_marker -lpcap -lm
generate_size_num_marker.o: $(generate_headers)
	$(CC) -c generate_size_num_marker.c -o generate_size_num_marker.o

generate_total_marker: $(generate_objects) generate_total_marker.o
	$(CC) generate_total_marker.o $(generate_objects) -o generate_total_marker -lpcap -lm
generate_total_marker.o: $(generate_headers)
	$(CC) -c generate_total_marker.c -o generate_total_marker.o

generate_size_histogram: $(generate_objects) generate_size_histogram.o
	$(CC) generate_size_histogram.o $(generate_objects) -o generate_size_histogram -lpcap -lm
generate_size_histogram.o: $(generate_headers)
	$(CC) -c generate_size_histogram.c -o generate_size_histogram.o

feature_extraction: $(fe_objects) feature_extraction.o
	$(CC) $(fe_objects) feature_extraction.o -o feature_extraction -lpcap -lm
feature_extraction.o: $(fe_headers) feature_extraction.c
	$(CC) -c feature_extraction.c -o feature_extraction.o

fe_rtt.o: $(fe_headers) fe_rtt.c
	$(CC) -c fe_rtt.c -o fe_rtt.o

fe_total_pkt_up.o: $(fe_headers) fe_total_pkt_up.c
	$(CC) -c fe_total_pkt_up.c -o fe_total_pkt_up.o

fe_total_pkt_down.o: $(fe_headers) fe_total_pkt_down.c
	$(CC) -c fe_total_pkt_down.c -o fe_total_pkt_down.o

fe_percent_up.o: $(fe_headers) fe_percent_up.c
	$(CC) -c fe_percent_up.c -o fe_percent_up.o

fe_percent_down.o: $(fe_headers) fe_percent_down.c
	$(CC) -c fe_percent_down.c -o fe_percent_down.o

fe_total_byte_down.o: $(fe_headers) fe_total_byte_down.c
	$(CC) -c fe_total_byte_down.c -o fe_total_byte_down.o

fe_total_byte_up.o: $(fe_headers) fe_total_byte_up.c
	$(CC) -c fe_total_byte_up.c -o fe_total_byte_up.o

fe_html_markers.o: $(fe_headers) fe_html_markers.c
	$(CC) -c fe_html_markers.c -o fe_html_markers.o

fe_size_markers.o: $(fe_headers) fe_size_markers.c
	$(CC) -c fe_size_markers.c -o fe_size_markers.o

fe_num_markers.o: $(fe_headers) fe_num_markers.c
	$(CC) -c fe_num_markers.c -o fe_num_markers.o

fe_size_histogram.o: $(fe_headers) fe_size_histogram.c
	$(CC) -c fe_size_histogram.c -o fe_size_histogram.o

fe_total_time.o: $(fe_headers) fe_total_time.c
	$(CC) -c fe_total_time.c -o fe_total_time.o

fe_raw_meta_info.o: $(fe_headers) fe_raw_meta_info.c
	$(CC) -c fe_raw_meta_info.c -o fe_raw_meta_info.o

fe_component_vectors.o: $(fe_headers) fe_component_vectors.c
	$(CC) -c fe_component_vectors.c -o fe_component_vectors.o

fe_two_d_space.o: $(fe_headers) fe_two_d_space.c
	$(CC) -c fe_two_d_space.c -o fe_two_d_space.o

fe_one_d_space.o: $(fe_headers) fe_one_d_space.c
	$(CC) -c fe_one_d_space.c -o fe_one_d_space.o

default_fe.o: $(fe_headers) default_fe.c
	$(CC) -c default_fe.c -o default_fe.o

fe_unique_up.o: $(fe_headers) fe_unique_up.c
	$(CC) -c fe_unique_up.c -o fe_unique_up.o

fe_unique_down.o: $(fe_headers) fe_unique_down.c
	$(CC) -c fe_unique_down.c -o fe_unique_down.o

feature_config.o: $(fe_headers) feature_config.c
	$(CC) -c feature_config.c -o feature_config.o

clean:
	rm -rf *.o generate_html_marker prepare_pca generate_size_num_marker generate_size_histogram generate_total_marker
