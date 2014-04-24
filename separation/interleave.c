#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "trip_trace.h"

int set_delta(int *delta, int *select, int *select_sizes, int size, struct t_trace_list *list){
	int i;
	int remaining=0;
	int min=-1;
	int max=-1;
	int rand_choice;
	for(i=0 ; i < size; i++){
		if(delta[i] == -1){
			remaining++;
			continue;
		}
	}
	if(remaining == 0){
		return EXIT_SUCCESS;
	}
	rand_choice = rand() % size;
	while(delta[rand_choice] != -1){
		rand_choice = rand() % size;
	}
	if(remaining == size){
		delta[rand_choice] = 0;
		return set_delta(delta, select, select_sizes, size, list);
	}
	for(i=0 ; i < size ; i++){
		if(delta[i] == -1){
			continue;
		}
		if((min == -1) || (max == -1)){
			min = delta[i];
			max = list->trace_list[select[i]]->pkt_list[list->trace_list[select[i]]->num_elements-1]->time;
			continue;
		}
		if(delta[i] < min){
			min = delta[i];
		}
		if(list->trace_list[select[i]]->pkt_list[list->trace_list[select[i]]->num_elements-1]->time > max){
			max = list->trace_list[select[i]]->pkt_list[list->trace_list[select[i]]->num_elements-1]->time;
		}
	}
	if((max-min) <= 0){
		delta[rand_choice] = min;
	}
	else{
		delta[rand_choice] = (rand() % (max - min))+min;
	}
	return set_delta(delta, select, select_sizes, size, list);
}

int print_next_pkt(struct t_trace_list *list, int *select, int *select_sizes, int size, int *delta){
	int i;
	int min=-1;
	int num_match=0;
	int *matches=NULL;
	int rand_choice;
	struct t_pkt *pkt;
	for(i=0 ; i < size ; i++){
		if(select_sizes[i] == 0){
			continue;
		}
		if((min == -1) || (list->trace_list[select[i]]->pkt_list[list->trace_list[select[i]]->num_elements-select_sizes[i]]->time+delta[i] < min)){
			min = list->trace_list[select[i]]->pkt_list[list->trace_list[select[i]]->num_elements-select_sizes[i]]->time+delta[i];
		}
	}
	for(i=0 ; i < size ; i++){
		if(select_sizes[i] == 0){
			continue;
		}
		if(list->trace_list[select[i]]->pkt_list[list->trace_list[select[i]]->num_elements-select_sizes[i]]->time+delta[i] == min){
			num_match++;
			matches = (int *)realloc((void *)matches, sizeof(int)*num_match);
			matches[num_match-1] = i;
		}
	}
	rand_choice = rand() % num_match;
	pkt = list->trace_list[select[matches[rand_choice]]]->pkt_list[list->trace_list[select[matches[rand_choice]]]->num_elements-select_sizes[matches[rand_choice]]];
	printf("%d,%d,%d,%d\n", pkt->dir, pkt->time + delta[matches[rand_choice]], pkt->size, matches[rand_choice]);
	select_sizes[matches[rand_choice]]--;
	return EXIT_SUCCESS;
}

int remaining_work(int *select_sizes, int size){
	int i;
	for(i=0 ; i < size ; i++){
		if(select_sizes[i] != 0){
			return 1;
		}
	}
	return 0;
}

int main(int argc, char **argv){	
	struct t_trace_list *list;
	int i,j,k;
	int size;
	int *select;
	int *select_sizes;
	int *delta;
	int *label_list;
	int num_labels;
	int iterations;
	int max;
	list = load_t_trace_list(argv[1]);
	if(list == NULL){
		return EXIT_FAILURE;
	}
	size = argc - 3;
	if(size <= 0){
		return EXIT_FAILURE;
	}
	iterations = atoi(argv[2]);
	max = atoi(argv[3]);
	size = atoi(argv[4]);
	srand(time(NULL));
	for( ; iterations > 0 ; iterations--){
		select = (int *)malloc(sizeof(int) * size);
		select_sizes = (int *)malloc(sizeof(int) * size);
		delta = (int *)malloc(sizeof(int) * size);
		num_labels = 0;
		for(i=0 ; i < size ; i++){
			select[i] = rand() % max;
			select_sizes[i] = list->trace_list[select[i]]->num_elements;
			delta[i] = -1;
			num_labels += list->trace_list[select[i]]->num_labels;
		}
		label_list = (int *)malloc(sizeof(int) * num_labels);
		k=0;
		for(i=0 ; i < size ; i++){
			for(j=0 ; j < list->trace_list[select[i]]->num_labels ; j++){
				label_list[k++] = list->trace_list[select[i]]->label_list[j];
			}
		}
		set_delta(delta, select, select_sizes, size, list);
		printf(":%d", label_list[0]);
		for(i=1 ; i < num_labels ; i++){
			printf(",%d", label_list[i]);
		}
		printf("\n");
		free(label_list);
		while(remaining_work(select_sizes, size)){
			print_next_pkt(list, select, select_sizes, size, delta);
		}
		free(select);
		free(select_sizes);
		free(delta);
	}
	/*select = (int *)malloc(sizeof(int) * size);
	select_sizes = (int *)malloc(sizeof(int) * size);
	delta = (int *)malloc(sizeof(int) * size);
	num_labels = 0;
	for(i=0 ; i < size ; i++){
		select[i] = atoi(argv[i+3]);
		select_sizes[i] = list->trace_list[select[i]]->num_elements;
		delta[i] = -1;
		num_labels += list->trace_list[select[i]]->num_labels;
	}
	label_list = (int *)malloc(sizeof(int) * num_labels);
	k=0;
	for(i=0 ; i < size ; i++){
		for(j=0 ; j < list->trace_list[select[i]]->num_labels ; j++){
			label_list[k++] = list->trace_list[select[i]]->label_list[j];
		}
	}
	srand(time(NULL));
	set_delta(delta, select, select_sizes, size, list);
	//printf(":%d\n", atoi(argv[2]));
	printf(":%d", label_list[0]);
	for(i=1 ; i < num_labels ; i++){
		printf(",%d", label_list[i]);
	}
	printf("\n");
	free(label_list);
	while(remaining_work(select_sizes, size)){
		print_next_pkt(list, select, select_sizes, size, delta);
	}*/
	return EXIT_SUCCESS;
}
