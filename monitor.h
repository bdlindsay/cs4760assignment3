#ifndef MONITOR_H
#define MONITOR_H

#include "condition.h"
#include <time.h>

// monitor.h
// Brett Lindsay
// cs4760 assignment3

void enter_monitor(int proc_num, int shm_id, void (*enter_cs)(int));

#endif
