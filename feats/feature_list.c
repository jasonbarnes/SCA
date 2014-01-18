#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "feature_config.h"
#include "feature_extraction.h"
/*
This file contains two lists of function pointers, fe_num_f_list and fe_extract_f_list.
fe_num_f_list entries point to a function that returns the number of features that an
extractor will generate based on the configuration info in fe_config_list.
fe_extract_f_list entries point to the functions that actually perform the extraction
and store it in the feature vector.
*/
const extern int (*fe_num_f_list[])(struct fe_config_list *, struct fe_basic_info *) = {NULL, NULL};
const extern int (*fe_extract_f_list[])(struct fe_config_list *, struct fe_basic_info *, int *, int) = {NULL, NULL, NULL};
