#ifndef __FE_TWO_D_SPACE_H__
#define __FE_TWO_D_SPACE_H__

extern int two_d_space_fe_num(struct fe_config_list *fe_list);
extern int two_d_space_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_size, struct t_trace *trace);

#endif
