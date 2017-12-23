#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	int initial;
	char expr[32];
	/* TODO: Add more members if necessary */


} WP;
WP* new_wp();
void free_wp(WP *wp);
WP *find_wp(int no);
void printf_wp();
bool check_wp();
#endif
