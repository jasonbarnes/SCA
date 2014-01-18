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
feature_extraction.c and fe_html_markers.c to work correctly:
The number of features expected by fe_html_markers.c
The occurring html marker sizes
Both of these are necessary since the original traffic-framework
code works by using variable length feature vectors that can 
change depending on the pcap files being used.  To replicate that
behavior, we must know beforehand how the entire set of pcap files
will impact the number of markers.
*/

struct html_marker_dat{
	int num_packets;
	int done;
	int32_t src;
	int32_t dst;
	int counterUp;
	int counterDown;
	int htmlMarker;
	struct countable_set *set;
};

void usage(){
	printf("usage:\ngenerate_html_marker <pcap_file> <html_marker.dat>\n");
}

/*
generate_set
Most of the work happens here, this function is applied to each packet in the pcap
file, finding the appropriate markers.
This code is based on traffic-analysis-framework/classifiers/PanchenkoClassifier.py
by K.P. Dyer
*/
void generate_set(const struct pcap_pkthdr *pkthdr, const u_char *packet, struct iphdr *ip, struct tcphdr *tcp, void *data){
	struct html_marker_dat *dat = (struct html_marker_dat *)data;
	if(dat->done){
		return;
	}
	if(dat->num_packets == 0){
		dat->src = ip->saddr;
		dat->dst = ip->daddr;
	}
	dat->num_packets++;
	if(ip->saddr == dat->src){
		dat->counterUp++;
		if((dat->counterUp > 1) && (dat->counterDown > 0)){
			dat->done = 1;
			return;
		}
	}
	else{
		dat->counterDown++;
		dat->htmlMarker += ntohs(ip->tot_len);
	}
	return;
}

int main(int argc, char **argv){
	struct countable_set *set;
	struct html_marker_dat *dat;
	int i;
	dat = (struct html_marker_dat *)malloc(sizeof(struct html_marker_dat));
	dat->counterUp=0;
	dat->counterDown=0;
	dat->htmlMarker=0;
	dat->num_packets=0;
	dat->done = 0;
	if(argc != 3){
		usage();
		return EXIT_FAILURE;
	}
	set = cs_load_set(argv[2]);
	if(set == NULL){
		return EXIT_FAILURE;
	}
	if(set->num_elements == 0){
		//printf("No elements in set\n");
	}
	else{
		for(i=0 ; i < set->num_elements ; i++){
			//printf("%d\n", set->keys[i]);
		}
	}
	dat->set = set;
	if(pcap_helper_offline(argv[1], &generate_set, dat) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	cs_add_key(set, (int)round_double(dat->htmlMarker, 600));
	if(cs_save_set(set, argv[2]) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	cs_deinit_set(set);
	return EXIT_SUCCESS;
}
