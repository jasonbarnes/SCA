#include <stdio.h>
#include <stdlib.h>
#include "trip_trace.h"

int print_pkts(struct t_pkt *pkt, void *data){
	printf("%d,%d,%d,%d", pkt->dir, pkt->time, pkt->size, pkt->label);
	printf("\n");
	return EXIT_SUCCESS;
}

int print_nonempty(struct t_trace *trace, void *data){
	int i;
	if(trace->num_elements){
		printf(":%d", trace->label_list[0]);
		for(i=1 ; i < trace->num_labels ; i++){
			printf(",%d", trace->label_list[i]);
		}
		printf("\n");
		for_each_t_pkt(trace, print_pkts, NULL);
	}
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	struct t_trace_list *list;
	list = load_t_trace_list(argv[1]);
	for_each_t_trace(list, print_nonempty, NULL);
	return EXIT_SUCCESS;
}
