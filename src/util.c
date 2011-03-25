#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"


void init_progress_bar(progress_bar_t *bar, const char *action_name) {
	
	bar->name = (char *) action_name;
	bar->val = 0;
}


void update_progress_bar(progress_bar_t *bar, float newProgress) {
	
	char tbar[64];
	float progr = 0.;
	
	if(newProgress>=100.)
		progr = 100.;
	else if(newProgress <= 0.)
		progr = 0.;
	else
		progr = newProgress;
	
	int barnum = (int) (progr/2.+.5);
	
	memset(tbar, 0, 64);
	memset(tbar, '=', barnum);
	memset(tbar+barnum, ' ', 50-barnum); 
	
	
	printf("\r%s: [%s] %.2f %%     ", bar->name, tbar, newProgress);
	fflush(stdout);

}