#include <stdio.h>

int main(int argc, char **argv){
	FILE *fp;
	int i;
	char b[1000];
	char *ptr;
	fp = fopen(argv[1], "r");
	while(fgets(b, sizeof(b), fp)){
		ptr = b;
		for(i=0 ; i < 1000 ; i++){
			if(b[i] == ','){
				break;
			}
		}
		ptr += i + 1;
		printf("%s", ptr);
	}
}
