#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
	char b[1000];
	int i;
	int final_dash_pos;
	char *ptr;
	while(fgets(b, sizeof(b), stdin)){
		final_dash_pos = -1;
		for(i=0 ; i < 1000 ; i++){
			if(b[i] == '\n' || b[i] == '\0'){
				break;
			}
			if(b[i] == '-'){
				final_dash_pos = i;
			}
		}
		if(final_dash_pos == -1){
			continue;
		}
		ptr = b + final_dash_pos + 1;
		printf("%s", ptr);
	}
	return EXIT_SUCCESS;
}
