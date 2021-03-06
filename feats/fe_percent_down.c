#include "trip_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_percent_down.h"
#include "round_double.h"

struct generate_set_info {
	int total;
	int total_up;
	int total_down;
};

extern int percent_down_fe_num(struct fe_config_list *fe_list){
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

static int generate_features_pkt(struct t_pkt *pkt, void *data){
	struct generate_set_info *info = (struct generate_set_info *)data;
	switch(pkt->dir){
	case 0:
		info->total_up += pkt->size;
		break;
	case 1:
		info->total_down += pkt->size;
		break;
	}
	info->total += pkt->size;
	return EXIT_SUCCESS;
}


extern int percent_down_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_start_index, struct t_trace *trace){
	double *list;
	int list_max;
	int i;
	struct generate_set_info info;
	info.total = 0;
	info.total_up = 0;
	info.total_down = 0;
	list = feature_vector + feature_vector_start_index;
	list_max = percent_down_fe_num(fe_list);
	for_each_t_pkt(trace, &generate_features_pkt, (void *)&info);
	list[0] = round_double(100.0 * (double)info.total_up/(double)info.total, 5.0);
	return EXIT_SUCCESS;
}
