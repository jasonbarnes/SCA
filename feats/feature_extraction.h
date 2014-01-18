#ifndef __FEATURE_EXTRACTION_H__
#define __FEATURE_EXTRACTION_H__

extern double round_double(double n, double base);

struct fe_basic_info {
	int32_t src;
	int32_t dst;
	int num_packets;
	int num_up_packets; //Number from src->dst
	int num_down_packets; //Number from src<-dst
	struct timeval start_time;
	struct timeval end_time;
	int total_size;
	int total_size_up;
	int total_size_down;
};

struct feature_extraction_config {
	//Panchenko Features
	int byte_ceil;
	int total_pkt_up;
	int total_pkt_down;
	int percent_up;
	int percent_down;
	int total_byte_up;
	int total_byte_down;
	int html_markers;
	int max_html_markers;
	int size_markers;
	int max_size_markers;
	int num_markers;
	int max_num_markers;
	int size_histogram;
	int size_histogram_bucket_size;//Bucket size, in bytes
	int max_size_histogram_bucket;//Max number of buckets
	//VNG++ Features
	//VNG is identical to Panchenko "int size_markers"
	//Total bandwidth is = to Panchenko "int total_byte_up/total_byte_down"
	int total_time;
	//Meta-PCA Features
	int raw_meta_info;
	int max_raw_meta_info;
	int component_vectors;
	int max_component_vectors;
	int two_d_space;
	int max_two_d_space;
	int one_d_space;
	int max_one_d_space;
};
#endif
