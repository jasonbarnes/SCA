#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_total_time.h"

/*
This feature extractor counts the total number of packets that travel from src to dst.
*/

int total_pkt;

extern int total_time_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic){
	int64_t *flag;
	int64_t *value;
	flag = get_fe_config_value(fe_list, "total_time");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	return 1;
}

extern int total_time_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_start_index, char *filename){
	double *list;
	int list_max;
	int i;
	struct timeval total_time;
	double fp_time = 0.0;
	total_pkt = 0;
	list = feature_vector + feature_vector_start_index;
	list_max = total_time_fe_num(fe_list, fe_basic);
	total_time.tv_sec = fe_basic->end_time.tv_sec;
	total_time.tv_usec = fe_basic->end_time.tv_usec;
	total_time.tv_sec -= fe_basic->start_time.tv_sec;
	if(total_time.tv_usec >= fe_basic->start_time.tv_usec){
		total_time.tv_usec -= fe_basic->start_time.tv_usec;
	}
	else{
		total_time.tv_sec--;
		total_time.tv_usec -= fe_basic->start_time.tv_usec;
		total_time.tv_usec *= -1;
	}
	fp_time = (double)total_time.tv_sec;
	fp_time += (double)total_time.tv_usec / 1000000.0;
	list[0] = fp_time;
	return EXIT_SUCCESS;
}
