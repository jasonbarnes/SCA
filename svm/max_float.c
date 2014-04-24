#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	double a = atof(argv[1]);
	double b = atof(argv[2]);
	if(a > b){
		printf("%f", a);
	}
	else{
		printf("%f", b);
	}
}
