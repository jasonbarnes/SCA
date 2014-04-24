#ifndef __FE_PERCENT_UP_H__
#define __FE_PERCENT_UP_H__

extern int percent_up_fe_num(struct fe_config_list *fe_list);
extern int percent_up_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_size, struct t_trace *trace);

#endif
