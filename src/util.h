#ifndef __UTIL_H
#define __UTIL_H

typedef struct _progress_bar_t {
	char *name;
	float val;
} progress_bar_t;


void init_progress_bar(progress_bar_t *bar, const char *action_name);

void update_progress_bar(progress_bar_t *bar, float newProgress);



#endif /*__UTIL_H */