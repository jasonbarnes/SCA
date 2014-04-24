#include "trip_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_total_byte_down.h"
#include "countable_set.h"
#include "round_double.h"

struct generate_set_info{
	struct countable_set *set;
	double *list;
	int list_max;
	int total;
};
	
extern int total_byte_down_fe_num(struct fe_config_list *fe_list){
	int64_t *flag;
	struct countable_set *set;
	int total=0;
	flag = get_fe_config_value(fe_list, "total_byte_down");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	set = cs_load_set("size_total_down.dat");
	if(set == NULL){
		return 0;
	}
	total = set->num_elements;
	cs_deinit_set(set);
	return total;
}


static int generate_features_pkt(struct t_pkt *pkt, void *data){
	struct generate_set_info *info = (struct generate_set_info *)data;
	if(pkt->dir == 1){
		info->total += pkt->size;
	}
	return EXIT_SUCCESS;
}


static int generate_features(struct fe_config_list *fe_list, double *feature_vector, int max_features, struct t_trace *trace, struct countable_set *set){
	struct generate_set_info info;
	int index;
	info.set = set;
	info.list = feature_vector;
	info.list_max = max_features;
	info.total=0;
	for_each_t_pkt(trace, &generate_features_pkt, (void *)&info);
	info.total = (int)round_double((double)info.total, 10000.0);
	index = cs_key_index(info.set, info.total);
	if(index < 0 || index > info.list_max){
		return EXIT_FAILURE;
	}
	info.list[index] = info.list[index] + 1.0;
	return EXIT_SUCCESS;
}

extern int total_byte_down_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_start_index, struct t_trace *trace){
	double *list;
	int list_max;
	int i;
	struct countable_set *set;
	list = feature_vector + feature_vector_start_index;
	list_max = total_byte_down_fe_num(fe_list);
	set = cs_load_set("size_total_down.dat");
	if(set == NULL){
		return EXIT_FAILURE;
	}
	if(generate_features(fe_list, list, list_max, trace, set) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
