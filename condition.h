#ifndef CONDITION_H
#define CONDITION_H

#include "semaphore.h"
#include <sys/shm.h>
#include <signal.h>

// condition.h
// Brett Lindsay
// cs4760 assignment3

typedef struct condition {
	int num_waiting_procs;
	sem_t sem; // sem.val holds the sem_id of the semaphore sem
	int next_count; 
	int sem_id; // sem_id for next and mutex
	//sem_t next; // shared semaphore index 0
	//sem_t mutex; // shared semaphore index 1
	int shm_id;
} cond_t;


cond_t* initcondition();
void cleanupcond(cond_t *cond);
void wait_cond(cond_t *cond);
void signal_cond(cond_t *cond); 

#endif
