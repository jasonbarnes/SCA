#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void usage(){
	printf("rand_draws <max> <num streams>\n");
	return;
}

int main(int argc, char **argv){
	int max;
	int num_streams;
	int draw;
	int i,j,k;
	if(argc != 3){
		usage();
		return EXIT_FAILURE;
	}
	max = atoi(argv[1]);
	num_streams = atoi(argv[2]);
	srand(time(NULL));
	draw = rand() % max;
	printf("%d", draw);
	for(j=1 ; j < num_streams ; j++){
		draw = rand() % max;
		printf(" %d", draw);
	}
	printf("\n");
	
	return EXIT_SUCCESS;
}
