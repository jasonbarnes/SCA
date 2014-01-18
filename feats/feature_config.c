#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "feature_config.h"
/*
extern int load_fe_config_list(struct fe_config_list *fe_list, char *filename)
This function reads each line of the config file and stores its lines as key/value pairs.
It should only be called once, inside of feature_extraction.c to populate the fe_list
before any of the feature extraction functions are called.
*/
extern int load_fe_config_list(struct fe_config_list *fe_list, char *filename){
	FILE *fp;
	char buffer[2000];
	int i;
	int j;
	char *ptr;
	double temp_double;
	struct fe_config_entry temp_entry;
	fp = fopen(filename, "r");
	if(fp == NULL){
		perror("Config file open error:");
		return EXIT_FAILURE;
	}
	fe_list->size=0;
	fe_list->list=NULL;
	for(j=0 ; j < 2000 ; j++){
		buffer[j] = 0;
	}
	while(fgets(buffer, sizeof(buffer), fp)){
		if(buffer[0] == '#'){
			for(j=0 ; j < 2000 ; j++){
				buffer[j] = 0;
			}
			continue;
		}
		for(i=0 ; i < 100 ; i++){
			temp_entry.name[i] = 0;
			temp_entry.value = 0;
		}
		for(i=0 ; i < 99 ; i++){
			if(buffer[i] == ':'){
				i++;
				break;
			}
			temp_entry.name[i] = buffer[i];
		}
		ptr = buffer+i;
		//Check to see if the value happens to be floating point
		for(j=i ; j < 2000 ; j++){
			if(buffer[j] == '.'){
				break;
			}
		}
		
		if(j != 2000){
			temp_double = atof(ptr);
			if(sizeof(double) != sizeof(int64_t)){
				perror("Floating point config option found, but sizeof(int64_t) != sizeof(double)");
				return EXIT_FAILURE;
			}
			memcpy(&(temp_entry.value), &temp_double, sizeof(double));
		}
		else{
			temp_entry.value = atoi(ptr);
		}
		if(add_fe_config_entry(fe_list, &temp_entry)==EXIT_FAILURE){
			return EXIT_FAILURE;
		}
		for(j=0 ; j < 2000 ; j++){
			buffer[j] = 0;
		}
	}
	fclose(fp);
	return EXIT_SUCCESS;
}
extern int destroy_fe_config_list(struct fe_config_list *fe_list){
	free(fe_list->list);
	return EXIT_SUCCESS;
}
/*
extern int64_t *get_fe_config_value(struct fe_config_list *fe_list, char *name)
This function returns a pointer to the value associated with name.  If no value exists, NULL
is returned.
*/
extern int64_t *get_fe_config_value(struct fe_config_list *fe_list, char *name){
	int i;
	for(i=0 ; i < fe_list->size ; i++){
		if(strncmp(name, fe_list->list[i].name, 100) == 0){
			return &(fe_list->list[i].value);	
		}
	}
	return NULL;
}
/*
static int add_fe_config_entry(struct fe_config_list *fe_list, struct fe_config_entry *fe_entry);
This function adds a new entry to the fe_list.  It reallocates more memory to fe_list->list if
necessary, and allocates it to begin with if fe_list->list == NULL.
*/
static int add_fe_config_entry(struct fe_config_list *fe_list, struct fe_config_entry *fe_entry){
	fe_list->list = realloc(fe_list->list, (fe_list->size + 1)*sizeof(struct fe_config_entry));
	if(fe_list->list == NULL){
		perror("Error allocating memory for fe_list:");
		return EXIT_FAILURE;
	}
	memcpy(&(fe_list->list[fe_list->size]), fe_entry, sizeof(struct fe_config_entry));
	fe_list->size++;
	return EXIT_SUCCESS;
}

extern void print_config(struct fe_config_list *fe_list){
	int i;
	for(i=0 ; i < fe_list->size ; i++){
		printf("%s:%ld\n", fe_list->list[i].name, fe_list->list[i].value);
	}
	return;
}
