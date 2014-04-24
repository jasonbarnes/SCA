#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	FILE *fp;
	char b[1000];
	int state=0;
	int total=0;
	fp = fopen(argv[1], "r");
	while(fgets(b, sizeof(b), fp)){
		if(b[0] == ':'){
			state = 0;
			continue;
		}
		if(state){
			continue;
		}
		total++;
		state = 1;
	}
	printf("%d\n", total);
	return 0;
}
