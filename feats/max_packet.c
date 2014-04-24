#include <stdio.h>
#include <stdlib.h>
#include "trip_trace.h"

void usage(){
	printf("usage:\nmax_packet filename\n");
	return;
}

int max_packet(struct t_trace *trace, void *data){
	int *max = (int *)data;
	if(trace->num_elements > *max){
		*max = trace->num_elements;
	}
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	struct t_trace_list *list;
	int max=0;
	if(argc != 2){
		usage();
		return EXIT_FAILURE;
	}
	list = load_t_trace_list(argv[1]);
	if(list == NULL){
		perror("Error loading trace list");
		return EXIT_FAILURE;
	}
	for_each_t_trace(list, &max_packet, &max);
	printf("%d", max);
	return EXIT_SUCCESS;
}
