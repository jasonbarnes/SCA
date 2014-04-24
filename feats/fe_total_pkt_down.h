#ifndef __FE_TOTAL_PKT_DOWN_H__
#define __FE_TOTAL_PKT_DOWN_H__

extern int total_pkt_down_fe_num(struct fe_config_list *fe_list);
extern int total_pkt_down_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_size, struct t_trace *trace);

#endif
