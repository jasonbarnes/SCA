#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <pcap/pcap.h>
#include <assert.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <math.h>

#include "feature_extraction.h"
#include "feature_config.h"
#include "fe_unique_down.h"
#include "pcap_helper.h"
#include "countable_set.h"
/*
This feature extractor counts the total number of unique packet sizes that occur
for src<-dst.  It works by using countable_set to maintain a set of sizes, then
checks the final size.
*/

int total_pkt;

struct unique_down_dat{
	int num_packets;
	int done;
	int32_t src;
	int32_t dst;
	struct countable_set *set;
};

extern int unique_down_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic){
	int64_t *flag;
	flag = get_fe_config_value(fe_list, "unique_down");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	return 1;
}

void unique_down_loop(const struct pcap_pkthdr *pkthdr, const u_char *packet, struct iphdr *ip, struct tcphdr *tcp, void *data){
	struct unique_down_dat *dat = (struct unique_down_dat *)data;
	if(dat->num_packets == 0){
		dat->src = ip->saddr;
		dat->dst = ip->daddr;
	}
	dat->num_packets++;
	if(dat->src != ip->saddr){
		cs_add_key(dat->set, ntohs(ip->tot_len));
	}
	return;
}

extern int unique_down_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_start_index, char *filename){
	struct countable_set *set;
	struct unique_down_dat dat;
	double *list;
	int list_max;
	int i;
	double a,b;
	total_pkt = 0;
	set = cs_init_set();
	dat.num_packets = 0;
	dat.done = 0;
	dat.src = 0;
	dat.dst = 0;
	dat.set = set;
	list = feature_vector + feature_vector_start_index;
	list_max = unique_down_fe_num(fe_list, fe_basic);
	if(pcap_helper_offline(filename, &unique_down_loop, (void *)&dat) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	list[0] = (double)round_double((double)dat.set->num_elements, 2.0);
	return EXIT_SUCCESS;
}
