#include <stdio.h>

void usage(){
	printf("usage:\ngenerate_label <filename> <label_text>\n");
	return;
}

int main(int argc, char **argv){
	FILE *fp;
	char ba[1000];
	char bb[1000];
	int i;
	int done=0;
	int count=1;
	if(argc != 3){
		usage();
		return 0;
	}
	for(i=0 ; i < 1000 ; i++){
		ba[i] = 0;
		bb[i] = 0;
	}
	for(i=0 ; i < 1000 ; i++){
		bb[i] = argv[2][i];
		if(argv[2][i] == '\0'){
			bb[i] = '\n';
			break;
		}
	}
	fp = fopen(argv[1], "r");
	while(fgets(ba, sizeof(ba), fp)){
		done = 0;
		for(i=0 ; i < 1000 ; i++){
			if(ba[i] != bb[i]){
				done = 0;
				break;
			}
			if(ba[i] == '\n'){
				done = 1;
				break;
			}
		}
		if(done){
			printf("%d", count);
			return 0;
		}
		count++;
	}
	printf("0");
	return 0;
}
