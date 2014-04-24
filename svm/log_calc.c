#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv){
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	double exp = (double)a+(double)b;
	printf("%f", pow(2.0, exp));
	return EXIT_SUCCESS;
}
