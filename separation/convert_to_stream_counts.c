#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "trip_trace.h"

int print_pkt(struct t_pkt *pkt, void *data){
	printf("%d,%d,%d,%d\n", pkt->dir, pkt->time, pkt->size, pkt->label);
	return EXIT_SUCCESS;
}

int print_stream_counts(struct t_trace *trace, void *data){
	printf(":%d\n", trace->num_labels);
	for_each_t_pkt(trace, print_pkt, NULL);
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	struct t_trace_list *list;
	list = load_t_trace_list(argv[1]);
	if(list == NULL){
		return EXIT_FAILURE;
	}
	for_each_t_trace(list, print_stream_counts, NULL);
	return EXIT_SUCCESS;
}
