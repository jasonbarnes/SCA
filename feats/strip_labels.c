#include <stdio.h>
#include <stdlib.h>
#include "trip_trace.h"

void usage(){
	printf("usage: ./strip_labels <trip_trace file>\n");
	return;
}

int print_labels(struct t_trace *trace, void *data){
	int i;
	printf("%d", trace->label_list[0]);
	for(i=1 ; i < trace->num_labels ; i++){
		printf(",%d", trace->label_list[i]);
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	struct t_trace_list *list;
	if(argc != 2){
		usage();
		return EXIT_FAILURE;
	}
	list = load_t_trace_list(argv[1]);
	for_each_t_trace(list, print_labels, NULL);
	return EXIT_SUCCESS;
}
