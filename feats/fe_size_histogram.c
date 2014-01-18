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
#include "pcap_helper.h"
#include "countable_set.h"
#include "fe_size_histogram.h"

/*
This feature extractor counts the total number of packets that travel from src to dst.
*/

int total_pkt;

struct size_histogram_dat{
	int num_packets;
	int done;
	int32_t src;
	int32_t dst;
	struct countable_set *down_set;
	struct countable_set *up_set;
	double *list;
	int list_max;
};

extern int size_histogram_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic){
	struct countable_set *up_set;
	struct countable_set *down_set;
	int64_t *flag;
	int64_t *value;
	int ret_val;
	int i;
	flag = get_fe_config_value(fe_list, "size_histogram");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	//value = get_fe_config_value(fe_list, "max_html_histogram");
	up_set = cs_load_set("size_histogram_up.dat");
	down_set = cs_load_set("size_histogram_down.dat");
	
	if(up_set == NULL || down_set == NULL){
		return 0;
	}
	ret_val = up_set->num_elements + down_set->num_elements;
	cs_deinit_set(up_set);
	cs_deinit_set(down_set);
	return ret_val;
}


void size_histogram_loop(const struct pcap_pkthdr *pkthdr, const u_char *packet, struct iphdr *ip, struct tcphdr *tcp, void *data){
	struct size_histogram_dat *dat = (struct size_histogram_dat *)data;
	int index;
	if(dat->num_packets == 0){
		dat->src = ip->saddr;
		dat->dst = ip->daddr;
	}
	dat->num_packets++;

	if(dat->src == ip->saddr){
		//cs_add_key(dat->up_set, ntohs(ip->tot_len));
		index = cs_key_index(dat->up_set, ntohs(ip->tot_len));
		if(index < 0 || index >= dat->list_max){
			//Do nothing
		}
		else{
			dat->list[index] += 1.0;
		}
	}
	else{
		//cs_add_key(dat->down_set, ntohs(ip->tot_len));	
		index = cs_key_index(dat->down_set, ntohs(ip->tot_len));
		if(index < 0 || index >= dat->list_max){
			//Do nothing
		}
		else{
			dat->list[dat->up_set->num_elements + index] += 1.0;
		}
	}
	return;
}

extern int size_histogram_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_start_index, char *filename){
	struct size_histogram_dat dat;
	int index;
	dat.num_packets = 0;
	dat.done = 0;
	dat.src = 0;
	dat.dst = 0;
	dat.list = feature_vector + feature_vector_start_index;
	dat.list_max = size_histogram_fe_num(fe_list, fe_basic);
	dat.up_set = cs_load_set("size_histogram_up.dat");
	dat.down_set = cs_load_set("size_histogram_down.dat");
	total_pkt = 0;
	if(pcap_helper_offline(filename, &size_histogram_loop, (void *)&dat) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	cs_deinit_set(dat.up_set);
	cs_deinit_set(dat.down_set);
	return EXIT_SUCCESS;
}
