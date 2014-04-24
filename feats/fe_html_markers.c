#include "trip_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_html_markers.h"
#include "countable_set.h"
#include "round_double.h"

struct generate_set_info{
	struct countable_set *set;
	double *list;
	int list_max;
	int counterUP;
	int counterDOWN;
	int htmlMarker;
};

int html_markers_fe_num(struct fe_config_list *fe_list){
	int64_t *flag;
	struct countable_set *set;
	int total;
	flag = get_fe_config_value(fe_list, "html_markers");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	set = cs_load_set("html_marker.dat");
	if(set == NULL){
		return 0;
	}
	total = set->num_elements;
	cs_deinit_set(set);
	return total;
}

static int generate_features_pkt(struct t_pkt *pkt, void *data){
	struct generate_set_info *info = (struct generate_set_info *)data;
	if(pkt->dir == 0){
		info->counterUP++;
		if((info->counterUP > 1) && (info->counterDOWN > 0)){
			return EXIT_FAILURE;//This should cause the loop to end here instead of continuing on
		}
	}
	else if(pkt->dir == 1){
		info->counterDOWN++;
		info->htmlMarker += pkt->size;
	}
	return EXIT_SUCCESS;
}

static int generate_features(struct fe_config_list *fe_list, double *feature_vector, int max_features, struct t_trace *trace, struct countable_set *set){
	struct generate_set_info info;
	int index;
	info.set = set;
	info.list = feature_vector;
	info.list_max = max_features;
	info.counterUP=0;
	info.counterDOWN=0;
	info.htmlMarker=0;
	for_each_t_pkt(trace, &generate_features_pkt, (void *)&info);
	info.htmlMarker = (int)round_double((double)info.htmlMarker, 600.0);
	index = cs_key_index(info.set, info.htmlMarker);
	if(index < 0 || index > info.list_max){
		return EXIT_FAILURE;
	}
	info.list[index] = info.list[index] + 1.0;
	return EXIT_SUCCESS;
}

extern int html_markers_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_start_index, struct t_trace *trace){
	double *list;
	int list_max;
	int i;
	struct countable_set *set;
	list = feature_vector + feature_vector_start_index;
	list_max = html_markers_fe_num(fe_list);
	set = cs_load_set("html_marker.dat");
	if(set == NULL){
		return EXIT_FAILURE;
	}
	if(generate_features(fe_list, list, list_max, trace, set) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
