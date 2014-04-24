#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <assert.h>
#include <math.h>

#include "round_double.h"
#include "countable_set.h"
#include "trip_trace.h"
/*
This program is designed to generate two things needed for
feature_extraction.c and fe_html_markers.c to work correctly:
The number of features expected by fe_html_markers.c
The occurring html marker sizes
Both of these are necessary since the original traffic-framework
code works by using variable length feature vectors that can 
change depending on the pcap files being used.  To replicate that
behavior, we must know beforehand how the entire set of pcap files
will impact the number of markers.
*/

struct generate_set_info{
	struct countable_set *size_up_set;
	struct countable_set *size_down_set;
};

void usage(){
	printf("usage:\ngenerate_histogram_marker <trip_file> <size_histogram_up.dat> <size_histogram_down.dat>\n");
}

/*
generate_set
Most of the work happens here, this function is applied to each packet in the pcap
file, finding the appropriate markers.
This code is based on traffic-analysis-framework/classifiers/PanchenkoClassifier.py
by K.P. Dyer
*/
int generate_set_pkt(struct t_pkt *pkt, void *data){
	struct generate_set_info *info = (struct generate_set_info *)data;
	if(pkt->dir == 0){
		cs_add_key(info->size_up_set, pkt->size);
	}
	else if(pkt->dir == 1){
		cs_add_key(info->size_down_set, pkt->size);
	}
	return EXIT_SUCCESS;
}
int generate_set_trace(struct t_trace *trace, void *data){
	struct generate_set_info *info = (struct generate_set_info *)data;
	for_each_t_pkt(trace, &generate_set_pkt, (void *)info);
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	struct t_trace_list *list;
	struct generate_set_info info;
	info.size_up_set = cs_init_set();
	info.size_down_set = cs_init_set();
	if(argc != 4){
		usage();
		return EXIT_FAILURE;
	}
	list = load_t_trace_list(argv[1]);
	for_each_t_trace(list, &generate_set_trace, (void *)&info);
	printf("Histogram Up Markers:\n");
	cs_print_set(info.size_up_set);
	cs_save_set(info.size_up_set, argv[2]);
	printf("Histogram Down Markers:\n");
	cs_print_set(info.size_down_set);
	cs_save_set(info.size_down_set, argv[3]);
	return EXIT_SUCCESS;
}
