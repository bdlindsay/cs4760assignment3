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

typedef enum {false, true} bool;

void critical_section(int i);

#endif
