#ifndef __TRIP_TRACE_H__
#define __TRIP_TRACE_H__

struct t_pkt {
	int label;
	int dir;
	int size;
	int time;
};

struct t_trace {
	int num_elements;
	int *label_list;
	int num_labels;
	struct t_pkt **pkt_list;
};

struct t_trace_list {
	int num_elements;
	struct t_trace **trace_list;
};

extern struct t_trace_list *load_t_trace_list(char *filename);
extern int deinit_t_trace_list(struct t_trace_list *list);

//For each functions
extern int for_each_t_trace(struct t_trace_list *list, int (*func)(struct t_trace *, void *), void *func_data);
extern int for_each_t_pkt(struct t_trace *trace, int (*func)(struct t_pkt *, void *), void *func_data);
extern int for_all_t_pkt(struct t_trace_list *list, int (*func)(struct t_pkt *, void *), void *func_data);
#endif
