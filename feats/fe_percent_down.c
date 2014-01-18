#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_percent_down.h"

/*
This feature extractor counts the total number of packets that travel from src to dst.
*/

int total_pkt;

extern int percent_down_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic){
	int64_t *flag;
	flag = get_fe_config_value(fe_list, "percent_down");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	return 1;
}

extern int percent_down_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_start_index, char *filename){
	double *list;
	int list_max;
	int i;
	double a,b;
	total_pkt = 0;
	list = feature_vector + feature_vector_start_index;
	list_max = percent_down_fe_num(fe_list, fe_basic);
	b = (double)fe_basic->num_packets;
	a = (double)fe_basic->num_down_packets;
	list[0] = (a/b)*100.0;
	list[0] = round_double(list[0], 5.0);
	return EXIT_SUCCESS;
}
