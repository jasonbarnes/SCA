#ifndef __FE_RAW_META_INFO_H__
#define __FE_RAW_META_INFO_H__

extern int raw_meta_info_fe_num(struct fe_config_list *fe_list);
extern int raw_meta_info_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_size, struct t_trace *trace);

#endif
