#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

;typedef struct semaphore {
	int val;
} sem_t;

void wait_sem(int sem_id, int sem_num);
void signal_sem(int sem_id, int sem_num);
int initelement(int id, int num, int val);

#endif
