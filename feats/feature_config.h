#ifndef __FEATURE_CONFIG_H__
#define __FEATURE_CONFIG_H__
/*
feature_config.h
This program is designed to provide functions to populate the feature configuration
options for feature_extraction.c and it's associated feature extracting programs.
The list should be populated in features.config, and loaded by feature_extraction.c.
Inside of feature extraction programs, use:
int *value;
value = get_fe_config_value(fe_list, name);
To get the value of the configuration options you need.
*/
struct fe_config_list {
	int size;
	struct fe_config_entry *list;
};

struct fe_config_entry {
	char name[100];
	int64_t value; //This is 8 bytes in case someone (?) wants to use it as a double
};

extern int load_fe_config_list(struct fe_config_list *fe_list, char *filename);
extern int destroy_fe_config_list(struct fe_config_list *fe_list);
extern int64_t *get_fe_config_value(struct fe_config_list *fe_list, char *name);
static int add_fe_config_entry(struct fe_config_list *fe_list, struct fe_config_entry *fe_entry);

//For debugging purposes:
extern void print_config(struct fe_config_list *fe_list);

#endif
