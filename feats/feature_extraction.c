#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "trip_trace.h"
#include "round_double.h"
#include "countable_set.h"
#include "feature_extraction.h"
#include "feature_config.h"
//Add feature extraction header files here:
//#include "default_fe.h"
#include "fe_total_pkt_up.h"
#include "fe_total_pkt_down.h"
#include "fe_percent_up.h"
#include "fe_percent_down.h"
#include "fe_total_byte_up.h"
#include "fe_total_byte_down.h"
#include "fe_html_markers.h"
#include "fe_size_markers.h"
#include "fe_num_markers.h"
#include "fe_size_histogram.h"
#include "fe_total_time.h"
#include "fe_raw_meta_info.h"
#include "fe_component_vectors.h"
#include "fe_two_d_space.h"
#include "fe_one_d_space.h"
#include "fe_unique_up.h"
#include "fe_unique_down.h"
#include "fe_rtt.h"

//Use this to print out the control path,
// to see where the control stops before
//segfaults.
#define DEBUG_PATH 0
#define INT_MODE 1
//Modify these two lists to include your feature extraction function pointers
int (*fe_num_f_list[])(struct fe_config_list *) = {
//&default_fe_num, 
&total_pkt_up_fe_num, 
&total_pkt_down_fe_num,
&percent_up_fe_num,
&percent_down_fe_num,
&total_byte_up_fe_num,
&total_byte_down_fe_num,
&unique_up_fe_num,
&unique_down_fe_num,
&html_markers_fe_num,
&size_markers_fe_num,
&num_markers_fe_num,
&size_histogram_fe_num,
&total_time_fe_num,
&raw_meta_info_fe_num,
&component_vectors_fe_num,
&two_d_space_fe_num,
&one_d_space_fe_num,
&rtt_fe_num
};
int (*fe_extract_f_list[])(struct fe_config_list *, double *, int, struct t_trace *) = {
//&default_fe_extract, 
&total_pkt_up_fe_extract, 
&total_pkt_down_fe_extract,
&percent_up_fe_extract,
&percent_down_fe_extract,
&total_byte_up_fe_extract,
&total_byte_down_fe_extract,
&unique_up_fe_extract,
&unique_down_fe_extract,
&html_markers_fe_extract,
&size_markers_fe_extract,
&num_markers_fe_extract,
&size_histogram_fe_extract,
&total_time_fe_extract,
&raw_meta_info_fe_extract,
&component_vectors_fe_extract,
&two_d_space_fe_extract,
&one_d_space_fe_extract,
&rtt_fe_extract
};

void usage(){
	printf("usage:\nfeature_extraction <trip_file> <feature.config>\n");
	return;
}

struct per_trace_info{
	struct feature_extraction_info *fe_info;
	struct fe_config_list *config;
	int (*func)(struct fe_config_list *, double *, int, struct t_trace *);
	int cur_trace;
	int cur_index;
};

int generate_feature_vector_size(struct fe_config_list *config){
	int (*func)(struct fe_config_list *);
	int i;
	int total=0;
	int max = sizeof(fe_num_f_list)/sizeof(func);
	for(i=0 ; i < max ; i++){
		func = fe_num_f_list[i];
		total += func(config);
	}
	return total+1;
}

int extract_per_trace(struct t_trace *trace, void *data){
	struct per_trace_info *t_info = (struct per_trace_info *)data;
	t_info->func(t_info->config, t_info->fe_info->csv[t_info->cur_trace], t_info->cur_index, trace);
	t_info->cur_trace++;
	return EXIT_SUCCESS;
}

int generate_labels(struct t_trace *trace, void *data){
	struct per_trace_info *t_info = (struct per_trace_info *)data;
	t_info->fe_info->csv[t_info->cur_trace][0] = trace->label_list[0];
	t_info->cur_trace++;
	return EXIT_SUCCESS;
}

int extract_features(struct feature_extraction_info *info, struct fe_config_list *config, struct t_trace_list *list){
	int i;
	int (*func)(struct fe_config_list *, double *, int, struct t_trace *);
	int max = sizeof(fe_extract_f_list)/sizeof(func);
	struct per_trace_info t_info;
	int add_index=0;
	int (*num_func)(struct fe_config_list *);
	t_info.fe_info = info;
	t_info.config = config;
	t_info.cur_index = 0;
	t_info.cur_trace = 0;
	for_each_t_trace(list, &generate_labels, (void *)&t_info);
	t_info.cur_trace=0;
	t_info.cur_index = 1;
	for(i=0 ; i < max ; i++){
		t_info.func = fe_extract_f_list[i];
		num_func = fe_num_f_list[i];
		t_info.cur_trace = 0;
		add_index = num_func(config);
		if(add_index == 0){
			continue;
		}
		#if DEBUG_PATH == 1
		printf("Made it to for_each_t_trace with extract_per_trace\n");
		printf("cur_trace=%d cur_index=%d\n", t_info.cur_trace, t_info.cur_index);
		#endif
		for_each_t_trace(list, &extract_per_trace, (void *)&t_info);
		#if DEBUG_PATH == 1
		printf("Made it past extract_per_trace\n");
		#endif
		t_info.cur_index += add_index;
	}
	#if DEBUG_PATH == 1
	printf("Made it past all traces\n");
	#endif
	return EXIT_SUCCESS;
}


int main(int argc, char **argv){
	struct feature_extraction_info info;
	struct fe_config_list config;
	struct t_trace_list *list;
	struct t_trace *trace;
	int i, j;
	info.n=0;
	info.m=0;
	info.csv=NULL;
	if(argc != 3){
		usage();
		return EXIT_FAILURE;
	}
	if(load_fe_config_list(&config, argv[2]) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	list = load_t_trace_list(argv[1]);
	if(list == NULL){
		return EXIT_FAILURE;
	}
	info.n=list->num_elements;
	info.m=generate_feature_vector_size(&config);
	info.csv = (double **)malloc(sizeof(double *)*info.n);
	if(info.csv == NULL){
		perror("Not enough memory:");
		return EXIT_FAILURE;
	}
	for(i=0 ; i < info.n ; i++){
		info.csv[i] = (double *)malloc(sizeof(double)*info.m);
		if(info.csv[i] == NULL){
			perror("Not enough memory:");
			return EXIT_FAILURE;
		}
		for(j=0 ; j < info.m ; j++){
			info.csv[i][j] = 0.0;
		}
	}
	#if DEBUG_PATH == 1
	printf("Made it to extract_features()\n");
	#endif
	if(extract_features(&info, &config, list)==EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	#if INT_MODE == 1
	for(i=0 ; i < info.n ; i++){
		printf("%d", (int)info.csv[i][0]);
		for(j=1 ; j < info.m ; j++){
			printf(",%d", (int)info.csv[i][j]);	
		}
		printf("\n");
	}
	#else
	for(i=0 ; i < info.n ; i++){
		printf("%f", (double)info.csv[i][0]);
		for(j=1 ; j < info.m ; j++){
			printf(",%f", (double)info.csv[i][j]);	
		}
		printf("\n");
	}
	#endif
	return EXIT_SUCCESS;
}
