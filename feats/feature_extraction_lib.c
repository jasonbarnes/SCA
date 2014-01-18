#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <float.h>

#include <netinet/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <pcap/pcap.h>
#include "feature_extraction.h"
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

#define CEIL_VAL 600

static int label;

struct couplet{
	uint32_t src;
	uint32_t dst;
	int size;
};

struct couplet filter_list[100];
int cur_filter_list;

uint32_t con_src;
uint32_t con_dst;

int direction_done;

struct sca_sample final_sample;

int cur_direction;
struct timeval min_time;
struct timeval max_time;
int first_time_set;
int first_ngram;

int pkt_count;

int ceil_n(int val, int cap){
	if((val % cap) == 0){
		return val;
	}
	return ((val / cap)+1) * cap;
}

void usage(){
	printf("usage:\nfeature_extraction <pcap_file> <label #>\n");
	return;
}


void pcapHandle(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet){
	u8 *ptr;
	u64 stamp;
	int i;
	int ether_type;
	int ether_offset;
	struct iphdr *ip;
	struct tcphdr *tcp;
	ptr = (u8 *)packet;
	u8 temp;
	u32 src;
	u32 dst;
	u16 len;
	u8 flipped;
	ether_type = ((int)(ptr[12]) << 8) | (int)ptr[13];
	if(ether_type == 0x0800){
		ether_offset = 14;
	}
	else{
		ether_offset = 18;
	}
	ptr += ether_offset;
	ip = (struct iphdr *)ptr;
	//Filter out non-TCP
	if(ip->protocol != 6){
		return;
	}
	tcp = (struct tcphdr *)(ptr+4*(((ip->tos) << 4)>>4));
	//Add to our current counts
	for(i=0 ; i < 100 ; i++){
		if((filter_list[i].src == ip->saddr)&&(filter_list[i].dst = ip->daddr)){
			break;	
		}
	}
	if(i == 100){
		//We didn't find a match, let's add a new entry
		for(i=0 ; i < 100 ; i++){
			if((filter_list[i].src == 0)&&(filter_list[i].dst == 0)){
				break;
			}
		}
		if(i == 100){
			//No empty space, let's exit out.
			return;
		}
		filter_list[i].src = ip->saddr;
		filter_list[i].dst = ip->daddr;
	}
	filter_list[i].size += pkthdr->caplen;
	return;
}

void print_syn(uint32_t src, uint32_t dst){
	uint8_t *ptr;
	ptr = &src;
	int i;
	for(i=0 ; i < 4 ; i++){
		printf("%d ", *(ptr+i));
	}
	ptr = &dst;
	for(i=0 ; i < 4 ; i++){
		printf("%d ", *(ptr+i));
	}
	return;
}

void pcap_direction(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet){
	u8 *ptr;
	u64 stamp;
	int i;
	int ether_type;
	int ether_offset;
	struct iphdr *ip;
	struct tcphdr *tcp;
	ptr = (u8 *)packet;
	u8 temp;
	u32 src;
	u32 dst;
	u16 len;
	u8 flipped;
	u32 temp_switch;
	u32 ipa;
	u32 ipb;
	pkt_count++;
	if(direction_done){
		return;
	}
	ether_type = ((int)(ptr[12]) << 8) | (int)ptr[13];
	if(ether_type == 0x0800){
		ether_offset = 14;
	}
	else{
		ether_offset = 18;
	}
	ptr += ether_offset;
	ip = (struct iphdr *)ptr;
	//Filter out non-TCP
	if(ip->protocol != 6){
		return;
	}
	//tcp = (struct tcphdr *)(ptr+4*(((ip->tos) << 4)>>4));
	tcp = (struct tcphdr *)(ptr + sizeof(struct iphdr));
		ipa = ip->saddr;
		ipb = ip->daddr;
		if(ip->saddr == con_src){
			direction_done = 1;
			return;
		}
		else if(ip->saddr == con_dst){
			temp_switch = con_dst;
			con_dst = con_src;
			con_src = temp_switch;
			direction_done = 1;
			return;
		}
	return;
}

void pcap_extract(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet){
	u8 *ptr;
	u64 stamp;
	int i;
	int ether_type;
	int ether_offset;
	struct iphdr *ip;
	struct tcphdr *tcp;
	ptr = (u8 *)packet;
	u8 temp;
	u32 src;
	u32 dst;
	u16 len;
	u8 flipped;
	ether_type = ((int)(ptr[12]) << 8) | (int)ptr[13];
	if(ether_type == 0x0800){
		ether_offset = 14;
	}
	else{
		ether_offset = 18;
	}
	ptr += ether_offset;
	ip = (struct iphdr *)ptr;
	//Filter out non-TCP
	if(ip->protocol != 6){
		return;
	}
	tcp = (struct tcphdr *)(ptr + sizeof(struct iphdr));
	//Let's filter out everything that isn't our connection
	if((ip->saddr == con_src) || (ip->saddr == con_dst)){
	}
	else{
		return;
	}
	if((ip->daddr == con_src) || (ip->daddr == con_dst)){
	}
	else
	{
		return;
	}
	//We have the connection, now let's grab the time.
	if(first_time_set){
		first_time_set=0;
		min_time.tv_sec = pkthdr->ts.tv_sec;
		min_time.tv_usec = pkthdr->ts.tv_usec;
		max_time.tv_sec = pkthdr->ts.tv_sec;
		max_time.tv_usec = pkthdr->ts.tv_usec;
	}
	else{
		if(pkthdr->ts.tv_sec < min_time.tv_sec){
			min_time.tv_sec = pkthdr->ts.tv_sec;
			min_time.tv_usec = pkthdr->ts.tv_usec;
		}
		else if(pkthdr->ts.tv_sec == min_time.tv_sec){
			if(pkthdr->ts.tv_usec < min_time.tv_usec){
				min_time.tv_usec = pkthdr->ts.tv_usec;
			}
		}
		if(pkthdr->ts.tv_sec > max_time.tv_sec){
			max_time.tv_sec = pkthdr->ts.tv_sec;
			max_time.tv_usec = pkthdr->ts.tv_usec;
		}
		else if(pkthdr->ts.tv_sec == max_time.tv_sec){
			if(pkthdr->ts.tv_usec > max_time.tv_usec){
				max_time.tv_usec = pkthdr->ts.tv_usec;
			}
		}

	}
	//Now that time has been recorded (if necessary)...
	//Let's filter out anything with SYN or ACK
	if(tcp->syn){
		return;
	}
	/*if((ntohs(ip->tot_len) - sizeof(struct iphdr) - tcp->doff * 4) == 0){
		//No data
		return;
	}*/
	//Let's see if we've recorded anthing for n-gram direction yet...
	if(first_ngram){
		if(ip->saddr == con_src){
			cur_direction = 0;
		}
		else{
			cur_direction = 1;
		}
		first_ngram = 0;
	}
	else{
		if(cur_direction == 0){
			if(ip->saddr == con_src){
				//Keep everything the same
			}
			else{
				cur_direction = 1;
				final_sample.vng_size++;
			}
		}
		else{
			if(ip->saddr != con_src){
				//Keep everything the same
			}
			else{
				cur_direction = 0;
				final_sample.vng_size++;
			}
		}
	}
	//Now that we know the flow direction, let's add the sizes where they need to go.
	if(final_sample.vng_size < MAX_VNG){
		//final_sample.vng[final_sample.vng_size] += pkthdr->caplen;
		final_sample.vng[final_sample.vng_size] += ntohs(ip->tot_len);
		
	}
	if(cur_direction == 0){
		//final_sample.total_bandwidth_client += pkthdr->caplen;
		final_sample.total_bandwidth_client += ntohs(ip->tot_len);
	}
	else{
		//final_sample.total_bandwidth_server += pkthdr->caplen;
		final_sample.total_bandwidth_server += ntohs(ip->tot_len);
	}
	if(final_sample.num_raw >= MAX_RAW){
		return;
	}
	//final_sample.raw_size[final_sample.num_raw] = pkthdr->caplen;
	final_sample.raw_size[final_sample.num_raw] = ntohs(ip->tot_len);
	final_sample.raw_direction[final_sample.num_raw] = cur_direction;
	final_sample.raw_ts[final_sample.num_raw].tv_sec = pkthdr->ts.tv_sec;
	final_sample.raw_ts[final_sample.num_raw].tv_usec = pkthdr->ts.tv_usec;
	final_sample.num_raw++;	
	return;
}

void print_con(){
	uint32_t p_src;
	uint32_t p_dst;
	uint8_t *ptr;
	int i;
	p_src = ntohl(con_src);
	p_dst = ntohl(con_dst);
	ptr = &p_src;
	for(i=0 ; i < 4 ; i++){
		printf("%u ", *(ptr+(3-i)));
	}
	ptr = &p_dst;
	for(i=0 ; i < 4 ; i++){
		printf("%u ", *(ptr+(3-i)));
	}
	printf("\n");
}

void process_file(char *filename){
	pcap_t *handle;
	int i;
	int max=0;
	int max_index=0;
	char errbuf[PCAP_ERRBUF_SIZE];
	handle = pcap_open_offline(filename, errbuf);
	if(handle==NULL){
		printf("pcap Open error\n");
		return;
	}
	if(pcap_loop(handle, 0, pcapHandle, NULL) < 0){
	}
	pcap_close(handle);
	//The highest size is the one we're looking for.
	for(i=0 ; i < 100 ; i++){
		if(filter_list[i].size > max){
			max = filter_list[i].size;
			max_index = i;
		}
	}
	con_src = filter_list[max_index].src;
	con_dst = filter_list[max_index].dst;
	handle = pcap_open_offline(filename, errbuf);
	if(handle==NULL){
		printf("pcap open error 2\n");
		return;
	}
	direction_done = 0;
	pkt_count = 0;
	if(pcap_loop(handle, 0, pcap_direction, NULL) < 0){
	}
	pcap_close(handle);
	if(direction_done == 0){
		printf("src direction not found\n");
		return;
	}
	first_time_set = 1;
	handle = pcap_open_offline(filename, errbuf);
	if(handle==NULL){
		printf("pcap open error 3\n");
		return;
	}
	if(pcap_loop(handle, 0, pcap_extract, NULL) < 0){
	}
	pcap_close(handle);
	//Now, to calculate the total time...
	max_time.tv_sec -= min_time.tv_sec;
	min_time.tv_sec = 0;
	if(max_time.tv_usec >= min_time.tv_usec){
		max_time.tv_usec -= min_time.tv_usec;
	}
	else{
		max_time.tv_sec -= 1;
		max_time.tv_usec = min_time.tv_usec - max_time.tv_usec;
	}
	//max_time now contains the total time, let's convert it to a double
	final_sample.total_time += max_time.tv_sec;
	final_sample.total_time += (max_time.tv_usec)/1000000.0;
	//600 cap modification
	for(i=0 ; i < MAX_VNG ; i++){
		final_sample.vng[i] = ceil_n(final_sample.vng[i], CEIL_VAL);
	}
	printf("%d", final_sample.label);
	#if VNG_FEATS == 1
	printf(",%f,%d,%d", final_sample.total_time, final_sample.total_bandwidth_client, final_sample.total_bandwidth_server);
	for(i=0 ; i < MAX_VNG ; i++){
		printf(",%d", final_sample.vng[i]);
	}
	printf(",%d\n", final_sample.vng_size);
	#endif
	#if P_FEATS == 1
	#if VNG_FEATS == 0
	for(i=0 ; i < MAX_VNG ; i++){
		printf(",%d", final_sample.vng[i]);
	}
	printf(",%d\n", final_sample.vng_size);
	#endif
	#endif
	#if RAW_FEATS == 1
	printf(",%d", final_sample.num_raw);
	for(i=0 ; i < MAX_RAW ; i++){
		printf(",%f,%d,%d", final_sample.raw_time[i], final_sample.raw_size[i], final_sample.raw_direction[i]);
	}
	#endif
	printf("\n");
	return;
}

int main(int argc, char **argv){
	int i;
	if(argc != 3){
		usage();
		return 0;
	}
	cur_filter_list = 0;
	for(i=0 ; i < 100 ; i++){
		filter_list[i].src = 0;
		filter_list[i].dst = 0;
		filter_list[i].size = 0;
	}
	label = atoi(argv[2]);
	final_sample.label = label;
	final_sample.total_time = 0.0;
	final_sample.total_bandwidth_client = 0;
	final_sample.total_bandwidth_server = 0;
	for(i=0 ; i < MAX_VNG ; i++){
		final_sample.vng[i] = 0;
	}
	final_sample.vng_size = 0;
	for(i=0 ; i < MAX_RAW ; i++){
		final_sample.num_raw = 0;
		final_sample.raw_ts[i].tv_sec = 0;
		final_sample.raw_ts[i].tv_usec = 0;
		final_sample.raw_time[i] = 0;
		final_sample.raw_size[i] = 0;
		final_sample.raw_direction[i] = 0;
	}
	process_file(argv[1]);
	return 0;
}
