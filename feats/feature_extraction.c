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
//Add feature extraction header files here:
#include "default_fe.h"
#include "fe_total_pkt_up.h"
#include "fe_total_pkt_down.h"
#include "fe_percent_up.h"
#include "fe_percent_down.h"
#include "fe_total_byte_up.h"
#include "fe_total_byte_down.h"
#include "fe_html_markers.h"
#include "fe_size_markers.h"
#include "fe_num_markers.h"
#include "fe_size_histogram.h"
#include "fe_total_time.h"
#include "fe_raw_meta_info.h"
#include "fe_component_vectors.h"
#include "fe_two_d_space.h"
#include "fe_one_d_space.h"
#include "fe_unique_up.h"
#include "fe_unique_down.h"
//This structure contains the information fead in by the configuration
//file.

/*const struct feature_extraction_config default_ex_con = {
	.byte_ceil = 600,
	.total_pkt_up = 1,
	.total_pkt_down = 1,
	.percent_up = 1,
	.percent_down = 1,
	.total_byte_up = 1,
	.total_byte_down = 1,
	.html_markers = 1,
	.max_html_markers = 20,
	.size_markers = 1,
	.max_size_markers = 20,
	.num_markers = 1,
	.max_num_markers = 20,
	.size_histogram = 1,
	.size_histogram_bucket_size = 600,
	.max_size_histogram_bucket = 20,
	.total_time = 1,
	.raw_meta_info = 1,
	.max_raw_meta_info = 20,
	.component_vectors = 1,
	.max_component_vectors = 1,
	.two_d_space = 1,
	.one_d_space = 1
};*/

//Modify these two lists to include your feature extraction function pointers
int (*fe_num_f_list[])(struct fe_config_list *, struct fe_basic_info *) = {
&default_fe_num, 
&total_pkt_up_fe_num, 
&total_pkt_down_fe_num,
&percent_up_fe_num,
&percent_down_fe_num,
&total_byte_up_fe_num,
&total_byte_down_fe_num,
&unique_up_fe_num,
&unique_down_fe_num,
&html_markers_fe_num,
&size_markers_fe_num,
&num_markers_fe_num,
&size_histogram_fe_num,
&total_time_fe_num,
&raw_meta_info_fe_num,
&component_vectors_fe_num,
&two_d_space_fe_num,
&one_d_space_fe_num
};
int (*fe_extract_f_list[])(struct fe_config_list *, struct fe_basic_info *, double *, int, char *) = {
&default_fe_extract, 
&total_pkt_up_fe_extract, 
&total_pkt_down_fe_extract,
&percent_up_fe_extract,
&percent_down_fe_extract,
&total_byte_up_fe_extract,
&total_byte_down_fe_extract,
&unique_up_fe_extract,
&unique_down_fe_extract,
&html_markers_fe_extract,
&size_markers_fe_extract,
&num_markers_fe_extract,
&size_histogram_fe_extract,
&total_time_fe_extract,
&raw_meta_info_fe_extract,
&component_vectors_fe_extract,
&two_d_space_fe_extract,
&one_d_space_fe_extract
};

extern double round_double(double n, double base){
	n = n / base;
	return round(n) * base;
}

void usage(){
	printf("usage:\nfeature_extraction <pcap input> <config file>\n");
	return;
}
int is_pcap_file_valid(char *filename){
	pcap_t *handle;
	char errbuf[PCAP_ERRBUF_SIZE];
	handle = pcap_open_offline(filename, errbuf);
	if(handle == NULL){
		perror("pcap input file error:");
		return EXIT_FAILURE;
	}
	pcap_close(handle);
	return EXIT_SUCCESS;
}
/*
This function populates fe_basic with the basic information that is shared across
feature extractors.  Things like the src and dst IP address, number of packets,
time, and more.  See the definition of fe_basic_info in feature_extraction.h for
more info on the different fields.
*/
void pcap_basic_info_loop(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet){
	struct fe_basic_info *fe_basic = (struct fe_basic_info *)userData;
	int i;
	int ether_type;
	int ether_offset;
	int32_t ip_a;
	int32_t ip_b;
	struct iphdr *ip;
	struct tcphdr *tcp;
	char *ptr;
	//Let's start by making sure we haven't encountered an error yet
	if(fe_basic->num_packets == -1){
		return;
	}
	//First few lines here are getting ptr to the start of the ip header
	ptr = (char *)packet;
	ether_type = ((int)(ptr[12]) << 8) | (int)ptr[13];
	if(ether_type == 0x0800){
		ether_offset = 14;
	}
	else{
		ether_offset = 18;
	}
	ptr += ether_offset;
	ip = (struct iphdr *)ptr;
	//Let's check to see if there are packets that don't belong:
	if(ip->protocol != 6){
		perror("pcap_basic_info_loop detected non-tcp packets:");
		fe_basic->num_packets = -1;
		return;
	}
	//Now we have the IP header, let's copy some things:
	if(fe_basic->num_packets == 0){
		//First packet, let's set src and dst
		fe_basic->src = ip->saddr;
		fe_basic->dst = ip->daddr;
		//Let's also get a starting point for time
		fe_basic->start_time.tv_sec = pkthdr->ts.tv_sec;
		fe_basic->start_time.tv_usec = pkthdr->ts.tv_usec;
		fe_basic->end_time.tv_sec = pkthdr->ts.tv_sec;
		fe_basic->end_time.tv_usec = pkthdr->ts.tv_usec;
	}
	//Let's check to see if there are packets that aren't from src or dst
	ip_a = ip->saddr;
	ip_b = ip->daddr;
	if(ip_a != fe_basic->src){
		if(ip_a != fe_basic->dst){
			perror("pcap_basic_info_loop found third party packets:");
			fe_basic->num_packets = -1;
			return;
		}
	}
	if(ip_b != fe_basic->src){
		if(ip_b != fe_basic->dst){
			perror("pcap_basic_info_loop found third party packets:");
			fe_basic->num_packets = -1;
			return;
		}
	}
	fe_basic->num_packets++;
	fe_basic->total_size += ntohs(ip->tot_len);
	if(ip_a == fe_basic->src){
		fe_basic->num_up_packets++;
		fe_basic->total_size_up += ntohs(ip->tot_len);
	}
	else{
		fe_basic->num_down_packets++;
		fe_basic->total_size_down += ntohs(ip->tot_len);
	}
	if(pkthdr->ts.tv_sec < fe_basic->start_time.tv_sec){
		fe_basic->start_time.tv_sec = pkthdr->ts.tv_sec;
		fe_basic->start_time.tv_usec = pkthdr->ts.tv_usec;
	}
	if(pkthdr->ts.tv_sec > fe_basic->end_time.tv_sec){
		fe_basic->end_time.tv_sec = pkthdr->ts.tv_sec;
		fe_basic->end_time.tv_usec = pkthdr->ts.tv_usec;
	}
	if(pkthdr->ts.tv_sec == fe_basic->start_time.tv_sec){
		if(pkthdr->ts.tv_usec < fe_basic->start_time.tv_usec){
			fe_basic->start_time.tv_sec = pkthdr->ts.tv_sec;
			fe_basic->start_time.tv_usec = pkthdr->ts.tv_usec;
		}
	}
	if(pkthdr->ts.tv_sec == fe_basic->end_time.tv_sec){
		if(pkthdr->ts.tv_usec > fe_basic->end_time.tv_usec){
			fe_basic->end_time.tv_sec = pkthdr->ts.tv_sec;
			fe_basic->end_time.tv_usec = pkthdr->ts.tv_usec;
		}
	}
}

void init_basic_info(struct fe_basic_info *fe_basic){
	memset((void *)fe_basic, 0, sizeof(struct fe_basic_info));
	return;
}

int generate_basic_info(struct fe_basic_info *fe_basic, char *filename){
	pcap_t *handle;
	char errbuf[PCAP_ERRBUF_SIZE];
	init_basic_info(fe_basic);
	handle = pcap_open_offline(filename, errbuf);
	if(handle == NULL){
		perror("pcap_open_offline error in generate_basic_info");
		return EXIT_FAILURE;
	}
	fe_basic->num_packets = 0;
	if(pcap_loop(handle, 0, pcap_basic_info_loop, (u_char *)fe_basic)){
	}
	pcap_close(handle);
	if(fe_basic->num_packets == 0){
		perror("pcap file is empty");
		return EXIT_FAILURE;
	}
	if(fe_basic->num_packets == -1){
		perror("Ran into error in pcap_basic_info_loop:");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void print_features(double *list, int size){
	int i;
	for(i=0 ; i < size ; i++){
		printf("%f", list[i]);
		if(i == size - 1){
			printf("\n");
		}
		else{
			printf(",");
		}
	}
	return;
}

/*
This function constructs the feature vector.  It also calls generate_basic_info on
the pcap file to get values that are commonly used across all of the different feature
extractors.  Given the options in fe_list, run
goes through each function listed above and constructs a 0 initialized
feature vector, and then extracts the features using each function.
*/
int run(struct fe_config_list *fe_list, char *in_filename){
	//Declarations
	struct fe_basic_info fe_basic;
	int total_features = 0;
	int cur_feature = 0;
	double *feature_vector;
	int i,j;
	//num_f is a pointer to a function that gives how many features will be
	//generated based on the config and basic info
	int (*num_f)(struct fe_config_list *, struct fe_basic_info *);
	//extract_f is a pointer to a function that extracts the features and
	//stores them in the feature vector allocated here in run()
	int (*extract_f)(struct fe_config_list *, struct fe_basic_info *, int *, int);
	//Check to see if the input file is a valid pcap file
	if(is_pcap_file_valid(in_filename) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	//Check file and set "generic" variables
	if(generate_basic_info(&fe_basic, in_filename) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	//Allocate feature vector
	for(i=0 ; i < sizeof(fe_num_f_list)/sizeof(fe_num_f_list[0]);i++){
		if(fe_num_f_list[i] == NULL){
			break;
		}
		total_features += fe_num_f_list[i](fe_list, &fe_basic);
	}
	feature_vector = (double *)malloc(total_features * sizeof(double));
	for(i=0 ; i < total_features ; i++){
		feature_vector[i] = 0.0;
	}
	//Generate features
	for(i=0 ; i < sizeof(fe_num_f_list)/sizeof(fe_num_f_list[0]);i++){
		if(fe_num_f_list[i] == NULL){
			break;
		}
		if(fe_num_f_list[i](fe_list, &fe_basic) == 0){
			continue;
		}
		if(fe_extract_f_list[i](fe_list, &fe_basic, feature_vector, cur_feature, in_filename) == EXIT_FAILURE){
			return EXIT_FAILURE;
		}
		cur_feature += fe_num_f_list[i](fe_list, &fe_basic);
	}
	print_features(feature_vector, total_features);
	return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	struct fe_config_list fe_list;
	if(sizeof(fe_num_f_list)/sizeof(fe_num_f_list[0]) != sizeof(fe_extract_f_list)/sizeof(fe_extract_f_list[0])){
		perror("Function pointers in fe_num_f_list and fe_extract_f_list do not match");
		return EXIT_FAILURE;
	}
	if(argc != 3){
		usage();
		return EXIT_FAILURE;
	}
	if(load_fe_config_list(&fe_list, argv[2]) == EXIT_FAILURE){
		return EXIT_FAILURE;
	}
	return run(&fe_list, argv[1]);
	
}
