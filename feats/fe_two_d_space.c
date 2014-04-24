//TODO PCA
#include "trip_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_two_d_space.h"

extern int two_d_space_fe_num(struct fe_config_list *fe_list){
	int64_t *flag;
	flag = get_fe_config_value(fe_list, "default");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	return 1;
}
extern int two_d_space_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_start_index, struct t_trace *trace){
	double *list;
	int list_max;
	int i;
	list = feature_vector + feature_vector_start_index;
	list_max = default_fe_num(fe_list);
	for(i=0 ; i < list_max ; i++){
		list[i] = 1.25;
	}
	return EXIT_SUCCESS;
}
