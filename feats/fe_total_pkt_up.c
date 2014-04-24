#include "trip_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_total_pkt_up.h"
#include "round_double.h"

struct generate_set_info {
	int total;
	int total_up;
	int total_down;
};

extern int total_pkt_up_fe_num(struct fe_config_list *fe_list){
	int64_t *flag;
	flag = get_fe_config_value(fe_list, "total_pkt_up");
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
	if(pkt->dir == 0){
		info->total++;
	}
	return EXIT_SUCCESS;
}


extern int total_pkt_up_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_start_index, struct t_trace *trace){
	double *list;
	int list_max;
	int i;
	struct generate_set_info info;
	info.total = 0;
	list = feature_vector + feature_vector_start_index;
	list_max = total_pkt_up_fe_num(fe_list);
	for_each_t_pkt(trace, &generate_features_pkt, (void *)&info);
	list[0] = round_double((double)info.total, 15.0);
	return EXIT_SUCCESS;
}
