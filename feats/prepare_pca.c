#include <stdio.h>
#include <stdlib.h>
#include "trip_trace.h"

struct prepare_info{
	int **pca_csv;
	int max_packets;
	int num_traces;
	int cur_pkt;
};

void usage(){
	printf("usage:\nprepare_pca <filename> <max_packets>\n");
	return;
}

int print_csv(int **csv, int n, int m){
	int i, j;
	for(i=0 ; i < n ; i++){
		printf("%d", csv[i][0]);
		for(j=1 ; j < m ; j++){
			printf(",%d", csv[i][j]);
		}
		printf("\n");
	}
	return EXIT_SUCCESS;
}

int prepare_per_pkt(struct t_pkt *pkt, void *data){
	struct prepare_info *info = (struct prepare_info *)data;
	int i;
	if(info->cur_pkt >= info->max_packets){
		return EXIT_FAILURE;
	}
	info->pca_csv[info->num_traces-1][1+(info->cur_pkt * 3)] = pkt->dir;
	info->pca_csv[info->num_traces-1][1+(info->cur_pkt * 3)+1] = pkt->size;
	info->pca_csv[info->num_traces-1][1+(info->cur_pkt * 3)+2] = pkt->time;

	info->cur_pkt++;
	return EXIT_SUCCESS;
}

int prepare_per_trace(struct t_trace *trace, void *data){
	struct prepare_info *info = (struct prepare_info *)data;
	int i;
	info->num_traces++;
	info->pca_csv = (int **)realloc((void *)info->pca_csv, sizeof(int *)*(info->num_traces));
	info->pca_csv[info->num_traces-1] = (int *)malloc(sizeof(int) * (info->max_packets*3 + 1));
	for(i=0 ; i < info->max_packets*3+1 ; i++){
		info->pca_csv[info->num_traces-1][i] = 0;
	}
	info->pca_csv[info->num_traces-1][0] = trace->label_list[0];
	info->cur_pkt = 0;
	for_each_t_pkt(trace, &prepare_per_pkt, &info);
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	struct t_trace_list *list;
	struct prepare_info info;
	info.pca_csv == NULL;
	info.num_traces = 0;
	if(argc != 3){
		usage();
		return EXIT_FAILURE;
	}
	list = load_t_trace_list(argv[1]);
	info.max_packets = atoi(argv[2]);
	if(list == NULL){
		return EXIT_FAILURE;
	}
	for_each_t_trace(list, &prepare_per_trace, &info);
	print_csv(info.pca_csv, info.num_traces, info.max_packets*3 + 1);
	return EXIT_SUCCESS;
}
