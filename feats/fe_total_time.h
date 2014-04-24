#ifndef __FE_TOTAL_TIME_H__
#define __FE_TOTAL_TIME_H__

extern int total_time_fe_num(struct fe_config_list *fe_list);
extern int total_time_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_size, struct t_trace *trace);

#endif
