#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	FILE *fpa;
	FILE *fpb;
	int i;
	int a,b;
	double match=0.0;
	double total=0.0;
	char ba[1000];
	char bb[1000];
	fpa = fopen(argv[1], "r");
	fpb = fopen(argv[2], "r");
	while(fgets(ba, sizeof(ba), fpa)){
		if(fgets(bb, sizeof(bb), fpb)){
		}
		else{
			break;
		}
		a = atoi(ba);
		b = atoi(bb);
		total = total + 1;
		if(a == b){
			match= match + 1;
		}
	}
	printf("%f\n", match/total);
	return 0;
}
