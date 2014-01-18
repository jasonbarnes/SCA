#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <assert.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <math.h>

#include "round_double.h"
#include "countable_set.h"
#include "pcap_helper.h"
/*
This program is designed to generate two things needed for
feature_extraction.c and fe_num_markers.c to work correctly:
The number of features expected by fe_num_markers.c
The occurring html marker sizes
Both of these are necessary since the original traffic-framework
code works by using variable length feature vectors that can 
change depending on the pcap files being used.  To replicate that
behavior, we must know beforehand how the entire set of pcap files
will impact the number of markers.
*/

struct num_marker_dat{
	int num_packets;
	int done;
	int32_t src;
	int32_t dst;
	int directionCursor;
	int numCursor;
	struct countable_set *down_set;
	struct countable_set *up_set;
};

void usage(){
	printf("usage:\ngenerate_html_marker <pcap_file> <num_marker_up.dat> <num_marker_down.dat>\n");
}

/*
generate_set
Most of the work happens here, this function is applied to each packet in the pcap
file, finding the appropriate markers.
This code is based on traffic-analysis-framework/classifiers/PanchenkoClassifier.py
by K.P. Dyer
*/
void generate_set(const struct pcap_pkthdr *pkthdr, const u_char *packet, struct iphdr *ip, struct tcphdr *tcp, void *data){
	struct num_marker_dat *dat = (struct num_marker_dat *)data;
	if(dat->num_packets == 0){
		dat->src = ip->saddr;
		dat->dst = ip->daddr;
	}
	dat->num_packets++;
	if(dat->directionCursor == -1){
		if(dat->src == ip->saddr){
			dat->directionCursor = 0;
		}
		else{
			dat->directionCursor = 1;
		}
	}
	if(((dat->src == ip->saddr) && (dat->directionCursor == 1)) || ((dat->src != ip->saddr) && (dat->directionCursor == 0))){
		if(dat->directionCursor == 0){
			cs_add_key(dat->up_set, dat->numCursor);
		}
		else{
			cs_add_key(dat->down_set, dat->numCursor);
		}
		if(dat->src == ip->saddr){
			dat->directionCursor = 0;
		}
		else{
			dat->directionCursor = 1;
		}
		dat->numCursor = 0;
	}
	dat->numCursor++;
	return;
}

int main(int argc, char **argv){
	struct countable_set *down_set;
	struct countable_set *up_set;
	struct num_marker_dat *dat;
	int i;
	dat = (struct num_marker_dat *)malloc(sizeof(struct num_marker_dat));
	dat->num_packets = 0;
	dat->done = 0;
	dat->src = 0;
	dat->dst = 0;
	dat->directionCursor=-1;
	dat->numCursor=0;
	if(argc != 4){
		usage();
		return EXIT_FAILURE;
	}
	down_set = cs_load_set(argv[3]);
	up_set = cs_load_set(argv[2]);
	if(down_set == NULL || up_set == NULL){
		return EXIT_FAILURE;
	}
	if(down_set->num_elements == 0){
		//printf("No elements in down_set\n");
	}
	else{
		for(i=0 ; i < down_set->num_elements ; i++){
			//printf("%d\n", down_set->keys[i]);
		}
	}
	if(up_set->num_elements == 0){
		//printf("No elements in up_set\n");
	}
	else{
		for(i=0 ; i < up_set->num_elements ; i++){
			//printf("%d\n", up_set->keys[i]);
		}
	}
	dat->down_set = down_set;
	dat->up_set = up_set;
	if(pcap_helper_offline(argv[1], &generate_set, dat) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	if(dat->numCursor > 0){
		if(dat->directionCursor == 0){
			cs_add_key(up_set, dat->numCursor);
		}
		else{
			cs_add_key(down_set, dat->numCursor);
		}
	}
	if(cs_save_set(down_set, argv[3]) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	if(cs_save_set(up_set, argv[2]) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	cs_deinit_set(down_set);
	cs_deinit_set(up_set);
	return EXIT_SUCCESS;
}
