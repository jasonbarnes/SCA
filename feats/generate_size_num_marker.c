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
	struct countable_set *num_up_set;
	struct countable_set *num_down_set;
	int directionCursor;
	int dataCursor;
	int numberCursor;
};

void usage(){
	printf("usage:\ngenerate_size_num_marker <trip_file> <size_marker_up.dat> <size_marker_down.dat> <num_marker_up.dat> <num_marker_down.dat>\n");
}

int round_number_marker(int n){
	switch(n){
	case 4:
	case 5:
	return 3;
	case 7:
	case 8:
	return 6;
	case 10:
	case 11:
	case 12:
	case 13:
	return 9;
	default:
	return n;
	}
	return n;
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
	if(info->directionCursor == -1){
		info->directionCursor = pkt->dir;
	}
	if(pkt->dir != info->directionCursor){
		if(info->directionCursor == 0){
			cs_add_key(info->size_up_set, (int)round_double((double)info->dataCursor, 600.0));	
			cs_add_key(info->num_up_set, round_number_marker(info->numberCursor));	
		}
		else if(info->directionCursor == 1){
			cs_add_key(info->size_down_set, (int)round_double((double)info->dataCursor, 600.0));
			cs_add_key(info->num_down_set, round_number_marker(info->numberCursor));	
		}
		info->directionCursor = pkt->dir;
		info->dataCursor = 0;
		info->numberCursor = 0;
	}
	info->dataCursor += pkt->size;
	info->numberCursor++;
	return EXIT_SUCCESS;
}
int generate_set_trace(struct t_trace *trace, void *data){
	struct generate_set_info *info = (struct generate_set_info *)data;
	info->directionCursor=-1;
	info->dataCursor=0;
	info->numberCursor=0;
	for_each_t_pkt(trace, &generate_set_pkt, (void *)info);
	if(info->dataCursor > 0){
		if(info->directionCursor == 0){
			cs_add_key(info->size_up_set, (int)round_double((double)info->dataCursor, 600.0));	
		}
		else if(info->directionCursor == 1){
			cs_add_key(info->size_down_set, (int)round_double((double)info->dataCursor, 600.0));
		}
	}
	if(info->numberCursor > 0){
		if(info->directionCursor == 0){
			cs_add_key(info->num_up_set, round_number_marker(info->numberCursor));	
		}
		else if(info->directionCursor == 1){
			cs_add_key(info->num_down_set, round_number_marker(info->numberCursor));	
		}
	}
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	struct t_trace_list *list;
	struct generate_set_info info;
	info.size_up_set = cs_init_set();
	info.size_down_set = cs_init_set();
	info.num_up_set = cs_init_set();
	info.num_down_set = cs_init_set();
	info.directionCursor=-1;
	info.dataCursor=-1;
	info.numberCursor=-1;
	if(argc != 6){
		usage();
		return EXIT_FAILURE;
	}
	list = load_t_trace_list(argv[1]);
	for_each_t_trace(list, &generate_set_trace, (void *)&info);
	printf("Size Up Markers:\n");
	cs_print_set(info.size_up_set);
	cs_save_set(info.size_up_set, argv[2]);
	printf("Size Down Markers:\n");
	cs_print_set(info.size_down_set);
	cs_save_set(info.size_down_set, argv[3]);
	printf("Num Up Markers:\n");
	cs_print_set(info.num_up_set);
	cs_save_set(info.num_up_set, argv[4]);
	printf("Num Down Markers:\n");
	cs_print_set(info.num_down_set);
	cs_save_set(info.num_down_set, argv[5]);
	return EXIT_SUCCESS;
}
