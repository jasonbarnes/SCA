#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 20000

void clear_buffer(char *buffer){
	int i;
	for(i=0 ; i < BUF_SIZE ; i++){
		buffer[i] = 0;
	}
	return;
}

int main(int argc, char **argv){
	FILE *fpa;
	FILE *fpb;
	char ba[BUF_SIZE];
	char bb[BUF_SIZE];
	int a;
	int b;
	int num_match=0;
	int total=0;
	fpa = fopen(argv[1], "r");
	fpb = fopen(argv[2], "r");
	clear_buffer(ba);
	clear_buffer(bb);
	while(fgets(ba, sizeof(ba), fpa)){
		if(fgets(bb, sizeof(bb), fpb)){
		}
		else{
			break;
		}
		a = atoi(ba);
		b = atoi(bb);
		if(a == b){
			num_match++;
		}
		total++;
		clear_buffer(ba);
		clear_buffer(bb);
	}
	printf("%f\n", (double)num_match/(double)total);
	return EXIT_SUCCESS;
}
