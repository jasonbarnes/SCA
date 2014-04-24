#include <stdio.h>
#include <stdlib.h>
#include "trip_trace.h"

int pad_pkt(struct t_pkt *pkt, void *data){
	int n = *((int *)data);
	if(pkt->size > n){
		printf("%d,%d,%d\n", pkt->dir, pkt->time, pkt->size);
	}
	else{
		printf("%d,%d,%d\n", pkt->dir, pkt->time, n);
	}
	return EXIT_SUCCESS;
}

int apply_countermeasure(struct t_trace *trace, void *data){
	printf(":%d\n", trace->label_list[0]);
	for_each_t_pkt(trace, pad_pkt, data);
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){	
	struct t_trace_list *list;
	int n = atoi(argv[2]);
	list = load_t_trace_list(argv[1]);
	if(list == NULL){
		return EXIT_FAILURE;
	}
	for_each_t_trace(list, apply_countermeasure, (void *)(&n));
	return EXIT_SUCCESS;
}
