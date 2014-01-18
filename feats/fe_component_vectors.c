#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_component_vectors.h"

/*
This feature extractor counts the total number of packets that travel from src to dst.
*/

int total_pkt;

extern int component_vectors_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic){
	int64_t *flag;
	int64_t *value;
	flag = get_fe_config_value(fe_list, "component_vectors");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	value = get_fe_config_value(fe_list, "max_component_vectors");
	return (int)(*value);
}

extern int component_vectors_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_start_index, char *filename){
	double *list;
	int list_max;
	int i;
	total_pkt = 0;
	list = feature_vector + feature_vector_start_index;
	list_max = component_vectors_fe_num(fe_list, fe_basic);
	for(i=0 ; i < list_max ; i++){
		list[i] = 0.88;
	}
	return EXIT_SUCCESS;
}
