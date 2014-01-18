#ifndef __PERCENT_DOWN_FE_H__
#define __PERCENT_DOWN_FE_H__

extern int percent_down_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic);
extern int percent_down_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_size, char *filename);

#endif
