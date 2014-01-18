#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_total_byte_up.h"

/*
This feature extractor counts the total number of packets that travel from src to dst.
*/

int total_pkt;

extern int total_byte_up_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic){
	int64_t *flag;
	flag = get_fe_config_value(fe_list, "total_byte_up");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	return 1;
}

extern int total_byte_up_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_start_index, char *filename){
	double *list;
	int list_max;
	int i;
	total_pkt = 0;
	list = feature_vector + feature_vector_start_index;
	list_max = total_byte_up_fe_num(fe_list, fe_basic);
	list[0] = (double)fe_basic->total_size_up;
	list[0] = round_double(list[0], 10000);
	return EXIT_SUCCESS;
}
