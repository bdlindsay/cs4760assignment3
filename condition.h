#include "semaphore.h"
#include <sys/shm.h>

typedef struct sharedInfo {
	int next_count; // shared int number waiting to enter monitor
} info_t;	

typedef struct condition {
	int num_waiting_procs;
	sem_t sem; // sem_t.val holds the sem_id of the semaphore
	//info_t *s_info;
	int next_count; 
	int sem_id; // sem_id for next and mutex
	//sem_t next; // shared semaphore index 0
	//sem_t mutex; // shared semaphore index 1
} cond_t;


cond_t* initcondition(int key_sem, int key_cond);
void initsharedsems(int key_sem); 
void wait_cond(cond_t *cond);
void signal_cond(cond_t *cond); 
void cleanupshared(int key_sem, int key_cond);
void cleanupcond(cond_t *cond);
