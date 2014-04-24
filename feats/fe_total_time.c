#include "trip_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_total_time.h"
#include "countable_set.h"
#include "round_double.h"

extern int total_time_fe_num(struct fe_config_list *fe_list){
	int64_t *flag;
	flag = get_fe_config_value(fe_list, "total_time");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	return 1;
}

int find_total_time(struct t_pkt *pkt, void *data){
	int *total_time = (int *)data;
	if( pkt->time > total_time[0] ){
		total_time[0] = pkt->time;
	}
	return EXIT_SUCCESS;
}

extern int total_time_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_start_index, struct t_trace *trace){
	double *list;
	int list_max;
	int i;
	int total_time=0;
	list = feature_vector + feature_vector_start_index;
	list_max = total_time_fe_num(fe_list);
	for(i=0 ; i < list_max ; i++){
		for_each_t_pkt(trace, find_total_time, (void *)&total_time);
		list[i] = (double)total_time;
	}
	return EXIT_SUCCESS;
}
