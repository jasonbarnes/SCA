#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "default_fe.h"

extern int default_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic){
	int64_t *flag;
	flag = get_fe_config_value(fe_list, "default");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	return 4;
}
extern int default_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_start_index, char *filename){
	double *list;
	int list_max;
	int i;
	list = feature_vector + feature_vector_start_index;
	list_max = default_fe_num(fe_list, fe_basic);
	for(i=0 ; i < list_max ; i++){
		list[i] = 1.5;
	}
	return EXIT_SUCCESS;
}
