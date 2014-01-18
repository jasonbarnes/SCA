#include <stdio.h>

char output_header[1000];

void usage(){
	printf("usage:\n");
	printf("organize_data <input> <output_header>\n");
}

void get_header(char *name){
	int i;
	for(i=0 ; i < 1000 ; i++){
		output_header[i] = name[i];
		if(name[i] == '\0'){
			break;
		}
	}
	output_header[1000] = '\0';
	return;
}

void process_file(char *filename){
	/*
	At this point in the processing, the file is sorted by string.
	This function separates the file into src,dst pairs.
	It also changes the data format to be more friendly to PCA.
	Namely, we go from src,dst,time,size,flipped to:
	time,size,direction
	src and dst are stored in the filename.
	*/
	FILE *ifp;
	FILE *ofp;
	int i;
	int j;
	char output_file[1000];
	char buffer[1000];
	unsigned int src;
	unsigned int dst;
	unsigned int temp;
	unsigned long int time;
	unsigned int len;
	unsigned int direction;
	ifp = fopen(filename, "r");
	while(fgets(buffer, sizeof(buffer), ifp)){
		sscanf(buffer, "%u,%u,%lu,%u,%u\n", &src, &dst, &time, &len, &direction);
		if(direction){
			sprintf(buffer, ".%u.%u.log\0", src,dst);
		}
		else{
			sprintf(buffer, ".%u.%u.log\0", dst,src);
		}
		for(i=0 ; i < 1000 ; i++){
			if(output_header[i] == '\0'){
				break;
			}
			output_file[i] = output_header[i];
		}
		j=0;
		for( ; i < 1000 ; i++){
			output_file[i] = buffer[j++];
			if(output_file[i] == '\0'){
				break;
			}
		}
		ofp = fopen(output_file, "a");
		fprintf(ofp, "%lu,%u,%u\n", time, len, direction);
		fclose(ofp);
	}
	return;
}

int main(int argc, char **argv){
	if(argc != 3){
		usage();
		return 0;
	}
	get_header(argv[2]);
	process_file(argv[1]);
	return 0;
}
