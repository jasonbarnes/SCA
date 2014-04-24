#ifndef __DEFAULT_FE_H__
#define __DEFAULT_FE_H__

extern int default_fe_num(struct fe_config_list *fe_list);
extern int default_fe_extract(struct fe_config_list *fe_list, double *feature_vector, int feature_vector_size, char *filename);

#endif
