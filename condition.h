#ifndef CONDITION_H
#define CONDITION_H

#include "semaphore.h"
#include <sys/shm.h>
#include <signal.h>

typedef struct condition {
	int num_waiting_procs;
	sem_t sem; // sem.val holds the sem_id of the semaphore
	int next_count; 
	int sem_id; // sem_id for next and mutex
	//sem_t next; // shared semaphore index 0
	//sem_t mutex; // shared semaphore index 1
} cond_t;


cond_t* initcondition(int key_sem, int key_cond);
void initsharedsems(int key_sem); 
void wait_cond(cond_t *cond);
void signal_cond(cond_t *cond); 
void cleanupcond(int key_sem, int key_cond, cond_t *cond);

#endif
