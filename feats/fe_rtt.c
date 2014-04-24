#include "trip_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_rtt.h"
#include "countable_set.h"
#include "round_double.h"

extern int rtt_fe_num(struct fe_config_list *fe_list){
	int64_t *flag;
	flag = get_fe_config_value(fe_list, "rtt");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	return 1;
}

int find_rtt(struct t_pkt *pkt, void *data){
	int *rtt=(int*)data;
	if(pkt->dir == 1){
		rtt[0] = pkt->time;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

extern int rtt_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_start_index, struct t_trace *trace){
	double *list;
	int list_max;
	int i;
	int rtt=0;
	list = feature_vector + feature_vector_start_index;
	list_max = rtt_fe_num(fe_list);
	for(i=0 ; i < list_max ; i++){
		for_each_t_pkt(trace, find_rtt, (void *)&rtt);
		list[i] = (double)rtt;
	}
	return EXIT_SUCCESS;
}
