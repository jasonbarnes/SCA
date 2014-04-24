#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 20000

void clear_buffer(char *buffer){
	int i;
	for(i=0 ; i < BUF_SIZE ; i++){
		buffer[i] = 0;
	}
}

int next_number(char *ptr){
	int i;
	for(i=0 ; (ptr[i] != ',') && (ptr[i] != '\n') && (ptr[i] != '\0') ; i++){
	}
	if(ptr[i] == ','){
		return i+1;
	}
	else{
		return -1;
	}
}

int print_sparse(char *buffer){
	char *ptr = buffer;
	int i;
	int next;
	int value;
	//First we print the label:
	printf("%d", atoi(ptr));
	next = next_number(ptr);
	for(i=1 ; next != -1 ; i++){
		ptr = ptr + next;
		value = atoi(ptr);
		if(value){
			printf(" %d:%d", i, value);
		}
		next = next_number(ptr);
	}
	printf("\n");
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	char buffer[BUF_SIZE];
	clear_buffer(buffer);
	while(fgets(buffer, sizeof(buffer), stdin)){
		print_sparse(buffer);
		clear_buffer(buffer);
	}
	return EXIT_SUCCESS;
}
