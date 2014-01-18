#ifndef __RAW_META_INFO_FE_H__
#define __RAW_META_INFO_FE_H__

extern int raw_meta_info_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic);
extern int raw_meta_info_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_size, char *filename);

#endif
