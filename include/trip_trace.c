#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> 
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "trip_trace.h"

//Static function forward declarations
static struct t_trace_list *init_t_trace_list();
static struct t_trace *init_t_trace(int *labels, int num_labels);
static struct t_pkt *init_t_pkt(int label, int dir, int size, int time);
static int deinit_t_trace(struct t_trace *trace);
static int deinit_t_pkt(struct t_pkt *pkt);
static int add_t_trace(struct t_trace_list *list, struct t_trace *trace);
static int add_t_pkt(struct t_trace *trace, int label, int dir, int size, int time);

static struct t_trace_list *init_t_trace_list(){
	struct t_trace_list *list = (struct t_trace_list *)malloc(sizeof(struct t_trace_list));
	list->num_elements = 0;
	list->trace_list = NULL;
	return list;
}
static struct t_trace *init_t_trace(int *labels, int num_labels){
	struct t_trace *trace = (struct t_trace *)malloc(sizeof(struct t_trace));
	int i;
	trace->num_elements = 0;
	trace->label_list = (int *)malloc(sizeof(int) * num_labels);
	for(i=0 ; i < num_labels ; i++){
		trace->label_list[i] = labels[i];
	}
	trace->num_labels = num_labels;
	trace->pkt_list = NULL;
	return trace;
}

static struct t_pkt *init_t_pkt(int label, int dir, int size, int time){
	struct t_pkt *pkt = (struct t_pkt *)malloc(sizeof(struct t_pkt));
	pkt->label = label;
	pkt->dir = dir;
	pkt->size = size;
	pkt->time = time;
	return pkt;
}
extern int deinit_t_trace_list(struct t_trace_list *list){
	int i;
	if(list != NULL){
		for(i=0 ; i < list->num_elements ; i++){
			deinit_t_trace(list->trace_list[i]);
		}
		free(list);
	}
	return EXIT_SUCCESS;
}

static int deinit_t_trace(struct t_trace *trace){
	int i;
	if(trace != NULL){
		for(i=0 ; i < trace->num_elements ; i++){
			deinit_t_pkt(trace->pkt_list[i]);
		}
		free(trace);
	}
	return EXIT_SUCCESS;
}

static int deinit_t_pkt(struct t_pkt *pkt){
	if(pkt != NULL){
		free(pkt);
	}
	return EXIT_SUCCESS;
}

static int add_t_trace(struct t_trace_list *list, struct t_trace *trace){
	list->trace_list = (struct t_trace **)realloc((void *)list->trace_list, sizeof(struct t_trace *)*(list->num_elements + 1));
	if(list->trace_list == NULL){
		return EXIT_FAILURE;
	}
	list->trace_list[list->num_elements] = trace;
	list->num_elements++;
	return EXIT_SUCCESS;
}
static int add_t_pkt(struct t_trace *trace, int label, int dir, int size, int time){
	trace->pkt_list = (struct t_pkt **)realloc((void *)trace->pkt_list, sizeof(struct t_pkt *)*(trace->num_elements + 1));
	if(trace->pkt_list == NULL){
		return EXIT_FAILURE;
	}
	trace->pkt_list[trace->num_elements] = init_t_pkt(label, dir, size, time);
	trace->num_elements++;
	return EXIT_SUCCESS;
}

static void clear_buffer(char *buffer){
	int i;
	for(i=0 ; i < 2000 ; i++){
		buffer[i] = 0;
	}
	return;
}

static int parse_triplet(char *buffer, int *dir, int *time, int *size, int *label){
	int i;
	int triplet_part=0;
	char *ptr;
	*dir = -1;
	*time = -1;
	*size = -1;
	*label = -1;
	ptr = buffer;
	for(i=0 ; i < 2000 ; i++){
		if(triplet_part == 3){
			*label = atoi(ptr);
			break;
		}
		if(buffer[i] == ','){
			buffer[i] = '\0';
			switch(triplet_part){
			case 0:
				triplet_part = 1;
				*dir = atoi(ptr);
				break;
			case 1:
				triplet_part = 2;
				*time = atoi(ptr);
				break;
			case 2:
				triplet_part = 3;
				*size = atoi(ptr);
				break;
			}
			ptr = buffer + i + 1;
		}
	}
	if((*dir == -1) || (*time == -1) || (*size == -1) || (*label == -1)){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*
This function can be used externally to fully construct a t_trace_list using
only a filename.  The file should be of the format:
:label
direction,time,size
direction,time,size
...
direction,time,size
:label
direction,time,size
...
Where label, direction, time, and size are all ints.
*/
extern struct t_trace_list *load_t_trace_list(char *filename){
	FILE *fp;
	char buffer[2000];
	int *labels=NULL;
	int dir, time, size, pkt_label;
	struct t_trace_list *list = init_t_trace_list();
	struct t_trace *trace = NULL;
	int i;
	int count;
	char *ptr;
	clear_buffer(buffer);
	fp = fopen(filename, "r");
	if(fp == NULL){
		return NULL;
	}
	while(fgets(buffer, sizeof(buffer), fp)){
		if(buffer[0] == ':'){
			if(trace != NULL){
				if(add_t_trace(list, trace) == EXIT_FAILURE){
					return NULL;
				}
			}
			//We're starting a new packet trace here.
			count=0;
			for(i=0 ; i < sizeof(buffer) ; i++){
				if(buffer[i] == 0){
					break;
				}
				if(buffer[i] == ',' || buffer[i] == ':'){
					count++;
				}

			}
			labels = (int *)malloc(sizeof(int) * count);
			ptr = buffer + 1;
			for(i=0 ; i < count ; i++){
				labels[i] = atoi(ptr);
				while((ptr[0] < '0') || (ptr[0] > '9')){
					ptr++;
				}
				ptr++;
			}
			trace = init_t_trace(labels, count);
			free(labels);
			clear_buffer(buffer);
			continue;
		}
		//If we made it here, then we're at a packet triplet
		if(parse_triplet(buffer, &dir, &time, &size, &pkt_label) == EXIT_FAILURE){
			return NULL;
		}
		if(add_t_pkt(trace, pkt_label, dir, size, time) == EXIT_FAILURE){
		//if(add_t_pkt(trace, dir, size, time) == EXIT_FAILURE){
			return NULL;
		}
		clear_buffer(buffer);
	}
	if(trace != NULL){
		if(add_t_trace(list, trace) == EXIT_FAILURE){
			return NULL;
		}
	}
	return list;
}

extern int for_each_t_trace(struct t_trace_list *list, int (*func)(struct t_trace *, void *), void *func_data){
	int i;
	struct t_trace *trace;
	for(i=0 ; i < list->num_elements ; i++){
		trace = list->trace_list[i];
		if(func(trace, func_data) == EXIT_FAILURE){
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

extern int for_each_t_pkt(struct t_trace *trace, int (*func)(struct t_pkt *, void *), void *func_data){
	int i;
	struct t_pkt *pkt;
	for(i=0 ; i < trace->num_elements ; i++){
		pkt = trace->pkt_list[i];
		if(func(pkt, func_data) == EXIT_FAILURE){
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}

extern int for_all_t_pkt(struct t_trace_list *list, int (*func)(struct t_pkt *, void *), void *func_data){
	int i;
	struct t_trace *trace;
	for(i=0 ; i < list->num_elements ; i++){
		trace = list->trace_list[i];
		if(for_each_t_pkt(trace, func, func_data) == EXIT_FAILURE){
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
