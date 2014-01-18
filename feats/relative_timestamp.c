#include <stdio.h>

void print_new_ts(unsigned long int offset, unsigned long int time, unsigned int a, unsigned int b){
	printf("%lu,%u,%u\n", time-offset, a, b);
	return;
}

void convert_to_relative(char *filename){
	FILE *fp;
	char buffer[1000];
	char out[1000];
	unsigned long int offset;
	unsigned long int time;
	unsigned int a,b;
	fp = fopen(filename, "r");
	fgets(buffer, sizeof(buffer), fp);
	sscanf(buffer, "%lu,%u,%u\n", &offset, &a, &b);
	print_new_ts(offset, offset, a, b);
	while(fgets(buffer, sizeof(buffer), fp)){
		sscanf(buffer, "%lu,%u,%u\n", &time, &a, &b);
		print_new_ts(offset, time, a, b);
	}
	return;
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Takes 1 arg, a csv file starting with a timestamp\n");
		return 0;
	}
	convert_to_relative(argv[1]);
	return 0;
}
