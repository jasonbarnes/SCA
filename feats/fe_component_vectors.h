#ifndef __COMPONENT_VECTORS_FE_H__
#define __COMPONENT_VECTORS_FE_H__

extern int component_vectors_fe_num(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic);
extern int component_vectors_fe_extract(struct fe_config_list *fe_list, struct fe_basic_info *fe_basic, double *feature_vector, int feature_vector_size, char *filename);

#endif
