#ifndef __COUNTABLE_SET_H__
#define __COUNTABLE_SET_H__
/*
This structure is used in all of the generate... programs
and inside some of the feature extractors to indicate collections
of countable objects.  It is required for the feature extractors
from traffic-framework to work correctly, since the number
of features changes with each set of pcap files.

This structure is saved in the various *.dat files.
It is mmapped inside of the feature extraction functions so
that they can work properly.properly.
*/

struct countable_set{
	int num_elements;
	int *keys;
};

extern struct countable_set *cs_load_set(char *filename);
extern int cs_save_set(struct countable_set *set, char *filename);
extern struct countable_set *cs_init_set();
extern int cs_deinit_set(struct countable_set *set);
extern int cs_key_index(struct countable_set *set, int key);
extern int cs_add_key(struct countable_set *set, int key);
#endif
