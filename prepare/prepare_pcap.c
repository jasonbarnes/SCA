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
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

void usage(){
	printf("usage:\nprepare_pcap <pcap input> <pcap output>\n");
	return;
}


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
	if(tcp->syn){
		if(tcp->ack){
			return;
		}
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
	}
	return;
}

void prepare_infile(char *filename){
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
	return;
}

void pcap_final_write(u_char *userData, const struct pcap_pkthdr* pkthdr, const u_char* packet){
	pcap_dumper_t *p_dump = (pcap_dumper_t *)userData;
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
	//Filter out packets without data (SYN, SYN/ACK, ACK)
	//We don't have to check the flags since all of them lack data segments
	if((ntohs(ip->tot_len) - sizeof(struct iphdr) - tcp->doff * 4) == 0){
		return;
	}
	if(ip->saddr == con_src || ip->saddr == con_dst){
		if(ip->daddr == con_src || ip->daddr == con_dst){
			//We know we have the right packet, let's change sport to 22 for the sake of traffic-framework if the packet is down
			if(ip->saddr != con_src){
				tcp->source = htons(22);
			}
			pcap_dump((u_char *)p_dump, pkthdr, packet);
		}
	}
	return;
}

void write_pcap(char *infile, char *outfile){
	pcap_t *in_handle;
	pcap_t *out_handle;
	pcap_dumper_t *p_dump;
	int i;
	char errbuf[PCAP_ERRBUF_SIZE];
	in_handle = pcap_open_offline(infile, errbuf);
	out_handle = pcap_open_dead(DLT_EN10MB, 65535);
	if(out_handle == NULL){
		perror("Out handle");
		return;
	}
	p_dump = pcap_dump_open(out_handle, outfile);
	if(p_dump == NULL){
		perror("p_dump");
		return;
	}
	if(pcap_loop(in_handle, 0, pcap_final_write, (u_char *)p_dump) < 0){
	}
	pcap_close(in_handle);
	pcap_dump_close(p_dump);
	return;
}

int main(int argc, char **argv){
	pcap_t *handle;
	if(argc != 3){
		usage();
		return EXIT_FAILURE;
	}
	prepare_infile(argv[1]);
	write_pcap(argv[1], argv[2]);
}
