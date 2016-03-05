#ifndef MASTER_H
#define MASTER_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "monitor.h"

// master.h
// Brett Lindsay
// cs4760 assignment3

typedef enum {false, true} bool;

void critical_section(int i);
const int p_n = 19; // process number to send each process
const int n = 18; // its respective place in the flag array (1 less)

#endif
