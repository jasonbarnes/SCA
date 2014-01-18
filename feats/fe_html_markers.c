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
#include "fe_html_markers.h"
#include "pcap_helper.h"
#include "countable_set.h"
/*
html_markers
*/

int total_pkt;

struct html_markers_dat{
	int num_packets;
	int done;
	int32_t src;
	int32_t dst;
	int counterUp;
	int counterDown;
	int htmlMarker;
	struct countable_set *set;
	double *list;
	int list_max;
};

extern int html_markers_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic){
	struct countable_set *set;
	int64_t *flag;
	int64_t *value;
	int ret_val;
	flag = get_fe_config_value(fe_list, "html_markers");
	if(flag == NULL){
		return 0;
	}
	if(*flag == 0){
		return 0;
	}
	//value = get_fe_config_value(fe_list, "max_html_markers");
	set = cs_load_set("html_marker.dat");
	if(set == NULL){
		return 0;
	}
	ret_val = set->num_elements;
	cs_deinit_set(set);
	return ret_val;
}

void html_markers_loop(const struct pcap_pkthdr *pkthdr, const u_char *packet, struct iphdr *ip, struct tcphdr *tcp, void *data){
	struct html_markers_dat *dat = (struct html_markers_dat *)data;
	int index;
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

extern int html_markers_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_start_index, char *filename){
	struct html_markers_dat dat;
	int index;
	dat.num_packets = 0;
	dat.done = 0;
	dat.src = 0;
	dat.dst = 0;
	dat.counterUp=0;
	dat.counterDown=0;
	dat.htmlMarker=0;
	dat.list = feature_vector + feature_vector_start_index;
	dat.list_max = html_markers_fe_num(fe_list, fe_basic);
	dat.set = cs_load_set("html_marker.dat");
	total_pkt = 0;
	if(pcap_helper_offline(filename, &html_markers_loop, (void *)&dat) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	index = cs_key_index(dat.set, (int)round_double((double)dat.htmlMarker, 600.0));
	if(index >= dat.list_max){
		return EXIT_FAILURE;
	}
	dat.list[index] += 1.0;
	return EXIT_SUCCESS;
}
