#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include "countable_set.h"

/*
This function creates a new countable_set and initializes it properly.
*/
extern struct countable_set *cs_init_set(){
	struct countable_set *set;
	set = (struct countable_set *)malloc(sizeof(struct countable_set));
	set->num_elements = 0;
	set->keys = NULL;
	return set;
}

/*
This function loads an existing countable_set from filename.
*/
extern struct countable_set *cs_load_set(char *filename){
	int fd;
	struct countable_set *set;
	void *ptr;
	struct stat sb;
	if((fd = open(filename, O_RDONLY)) < 0){
		//Check to see if we failed because of the file not existing...
		if(errno != ENOENT){
			perror("Error opening countable set .dat file"); 
			return NULL;
		}
		//File does not exist, create a new one...
		set = cs_init_set();
		if(cs_save_set(set, filename) == EXIT_FAILURE){
			perror("Error loading countable set .dat file");
			return NULL;
		}
		cs_deinit_set(set);
		if((fd = open(filename, O_RDONLY)) < 0){
			perror("Error opening countable set .dat file");
			return NULL;
		}
	}
	if(fstat(fd,&sb) < 0){
		perror("fstat error");
		return NULL;
	}
	ptr = mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
	set = cs_init_set();
	memcpy(set, ptr, sizeof(struct countable_set));
	set->keys = (int *)malloc(sizeof(int) * set->num_elements);
	memcpy(set->keys, ptr+sizeof(struct countable_set), sizeof(int)*set->num_elements);
	munmap(ptr, sb.st_size);
	close(fd);
	return set;
}
/*
This function writes a countable_set to disk
*/
extern int cs_save_set(struct countable_set *set, char *filename){
	FILE *fp;
	fp = fopen(filename, "wb");
	if(fp == NULL){
		perror("Error opening for writing");
		return EXIT_FAILURE;
	}
	fwrite(set, sizeof(struct countable_set), 1, fp);
	fwrite(set->keys, sizeof(int), set->num_elements, fp);
	fclose(fp);
	return EXIT_SUCCESS;
}
/*
This function frees up memory used by countable_set
*/
extern int cs_deinit_set(struct countable_set *set){
	free(set->keys);
	free(set);
	return EXIT_SUCCESS;
}

/*
This function adds a new element to countable_set, if it
is already not part of the set.
*/
extern int cs_add_key(struct countable_set *set, int key){
	int i;
	for(i=0 ; i < set->num_elements ; i++){
		if(set->keys[i] == key){
			return EXIT_SUCCESS;
		}
	}
	//Not found, let's increase the size of set->keys
	set->num_elements++;
	set->keys = (int *)realloc(set->keys, sizeof(int) * set->num_elements);
	set->keys[set->num_elements - 1] = key;
	return EXIT_SUCCESS;
}

/*
This function returns the index of set->keys where set->keys[index] == key,
or -1 if the key is not found.
*/
extern int cs_key_index(struct countable_set *set, int key){
	int i;
	for(i=0 ; i < set->num_elements ; i++){
		if(set->keys[i] == key){
			return i;
		}
	}
	return -1;
}


extern void cs_print_set(struct countable_set *set){
	int i;
	for(i=0 ; i < set->num_elements ; i++){
		printf("%d\n", set->keys[i]);
	}
	return;
}
