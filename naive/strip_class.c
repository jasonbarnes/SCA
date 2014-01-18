#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	FILE *fp;
	int i;
	int a;
	char buffer[1000];
	fp = fopen(argv[1], "r");
	while(fgets(buffer, sizeof(buffer), fp)){
		printf("%d\n", atoi(buffer));
	}
}
