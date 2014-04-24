#ifndef __FE_COMPONENT_VECTORS_H__
#define __FE_COMPONENT_VECTORS_H__

extern int component_vectors_fe_num(struct fe_config_list *fe_list);
extern int component_vectors_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_size, struct t_trace *trace);

#endif
