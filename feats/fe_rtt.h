#ifndef __FE_RTT_H__
#define __FE_RTT_H__

extern int rtt_fe_num(struct fe_config_list *fe_list);
extern int rtt_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_size, struct t_trace *trace);

#endif
