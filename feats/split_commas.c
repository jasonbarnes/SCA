#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	FILE *fp;
	char b[10000];
	int i;
	fp = fopen(argv[1], "r");
	fgets(b, sizeof(b), fp);
	for(i=0 ; b[i] != '\n' ; i++){
		if(b[i] == ','){
			printf("\n");
		}
		else{
			printf("%c", b[i]);
		}
	}
}
