#ifndef __FE_SIZE_HISTOGRAM_H__
#define __FE_SIZE_HISTOGRAM_H__

extern int size_histogram_fe_num(struct fe_config_list *fe_list);
extern int size_histogram_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_size, struct t_trace *trace);

#endif
