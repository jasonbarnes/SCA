#include "trip_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_unique_down.h"
#include "round_double.h"
#include "countable_set.h"

extern int unique_down_fe_num(struct fe_config_list *fe_list){
	int64_t *flag;
	flag = get_fe_config_value(fe_list, "unique_down");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	return 1;
}

static int generate_features_pkt(struct t_pkt *pkt, void *data){
	struct countable_set *set = (struct countable_set *)data;
	if(pkt->dir == 1){
		cs_add_key(set, pkt->size);
	}
	return EXIT_SUCCESS;
}

extern int unique_down_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_start_index, struct t_trace *trace){
	double *list;
	int list_max;
	int i;
	struct countable_set *set = cs_init_set();
	if(set == NULL){
		return EXIT_FAILURE;
	}
	list = feature_vector + feature_vector_start_index;
	list_max = unique_down_fe_num(fe_list);
	for_each_t_pkt(trace, &generate_features_pkt, (void *)set);
	list[0] = round_double((double)set->num_elements, 2.0);
	cs_deinit_set(set);
	return EXIT_SUCCESS;
}
