#include "trip_trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <pcap/pcap.h>

#include "round_double.h"
#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_size_markers.h"
#include "countable_set.h"

struct generate_set_info{
	struct countable_set *up_set;
	struct countable_set *down_set;
	double *list;
	int list_max;
	int directionCursor;
	int dataCursor;
	int numberCursor;
};

extern int size_markers_fe_num(struct fe_config_list *fe_list){
	int64_t *flag;
	int total;
	struct countable_set *up_set;
	struct countable_set *down_set;
	flag = get_fe_config_value(fe_list, "size_markers");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	up_set = cs_load_set("size_marker_up.dat");
	down_set = cs_load_set("size_marker_down.dat");
	if(up_set == NULL || down_set == NULL){
		return 0;
	}
	total = up_set->num_elements + down_set->num_elements;
	cs_deinit_set(up_set);
	cs_deinit_set(down_set);
	return total;
}

static int generate_features_pkt(struct t_pkt *pkt, void *data){
	struct generate_set_info *info = (struct generate_set_info *)data;
	int index;
	if(info->directionCursor == -1){
		info->directionCursor = pkt->dir;
	}
	if(pkt->dir != info->directionCursor){
		if(info->directionCursor == 0){
			index = cs_key_index(info->up_set, (int)round_double((double)info->dataCursor, 600.0));
			if(index < 0 || index > info->list_max){
				printf("Size Markers Failure: %d\n", index);
				return EXIT_FAILURE;
			}
			info->list[index] = info->list[index] + 1.0;
		}
		else if(info->directionCursor == 1){
			index = cs_key_index(info->down_set, (int)round_double((double)info->dataCursor, 600.0));
			if(index < 0 || index > info->list_max){
				printf("Size Markers Failure: %d\n", index);
				return EXIT_FAILURE;
			}
			index += info->up_set->num_elements;
			info->list[index] = info->list[index] + 1.0;
		}
		info->directionCursor = pkt->dir;
		info->dataCursor = 0;
	}
	info->dataCursor += pkt->size;
	return EXIT_SUCCESS;
}

static int generate_features(struct fe_config_list *fe_list, double *feature_vector, int max_features, struct t_trace *trace, struct countable_set *up_set, struct countable_set *down_set){
	struct generate_set_info info;
	int index;
	info.up_set = up_set;
	info.down_set = down_set;
	info.list = feature_vector;
	info.list_max = max_features;
	info.directionCursor=-1;
	info.dataCursor=0;
	for_each_t_pkt(trace, &generate_features_pkt, (void *)&info);
	if(info.dataCursor > 0){
		if(info.directionCursor == 0){
			index = cs_key_index(info.up_set, (int)round_double((double)info.dataCursor, 600.0));
			if(index < 0 || index > info.list_max){
				printf("Size Markers Failure: %d\n", index);
				return EXIT_FAILURE;
			}
			info.list[index] = info.list[index] + 1.0;
		}
		else if(info.directionCursor == 1){
			index = cs_key_index(info.down_set, (int)round_double((double)info.dataCursor, 600.0));
			if(index < 0 || index > info.list_max){
				printf("Size Markers Failure: %d\n", index);
				return EXIT_FAILURE;
			}
			index += info.up_set->num_elements;
			info.list[index] = info.list[index] + 1.0;
		}
	}
	return EXIT_SUCCESS;
}

extern int size_markers_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_start_index, struct t_trace *trace){
	double *list;
	int list_max;
	int i;
	struct countable_set *up_set;
	struct countable_set *down_set;
	list = feature_vector + feature_vector_start_index;
	list_max = size_markers_fe_num(fe_list);
	up_set = cs_load_set("size_marker_up.dat");
	down_set = cs_load_set("size_marker_down.dat");
	if(up_set == NULL || down_set == NULL){
		return EXIT_FAILURE;
	}
	if(generate_features(fe_list, list, list_max, trace, up_set, down_set) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	cs_deinit_set(up_set);
	cs_deinit_set(down_set);
	return EXIT_SUCCESS;
}
