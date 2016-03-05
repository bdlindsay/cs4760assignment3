#include "condition.h"

// condition.c 
// Brett Lindsay
// cs4760 assignment3

void wait_cond(cond_t *cond) {
	cond->num_waiting_procs++; // # processes waiting on this condition
	if(cond->next_count > 0) { // is someone waiting inside monitor
		signal_sem(cond->sem_id,1); // next.signal() - wake it up
	} else {
		signal_sem(cond->sem_id,0); // mutex.signal() - No, free mutex so others enter
	}
	wait_sem(cond->sem.val,0); // sem.wait() - start waiting for condition
	cond->num_waiting_procs--; // wait over. decrement variable

}

void signal_cond(cond_t *cond) {
	if (cond->num_waiting_procs <= 0) // do nothing if no one waiting
		return;
	cond->next_count++; // # of ready proccesses inside monitor
	signal_sem(cond->sem.val,0); // sem.signal()
	wait_sem(cond->sem_id,1); // next.wait() - you wait. let signaled process run
	cond->next_count--; // one less process in monitor
}

// call for each cond_t with the same key_sem & key_shm
cond_t* initcondition() {
	int shm_id;
	cond_t *cond;
	// get and attach shared memory for cond_t *cond
	shm_id = shmget(IPC_PRIVATE, sizeof(cond_t*),IPC_CREAT | 0755);
	cond = (cond_t*) shmat(shm_id,0,0);
	cond->shm_id = shm_id;

	// init int vars	
	cond->num_waiting_procs = 0;
	cond->next_count = 0;

	// allocate mutex and next
	if ((cond->sem_id = semget(IPC_PRIVATE,2, IPC_CREAT | 0755)) == -1) {
		perror("semget");
		raise(SIGINT);
	}
	// init mutex to 1
	if((initelement(cond->sem_id,0,1)) != 0) {
		perror("semctl:initelement");
		raise(SIGINT);
	}
	// init next to 0
	if ((initelement(cond->sem_id,1,0)) != 0) {
		perror("semctl:initelemnt");
		raise(SIGINT);
	}
	// allocate sem_t sem
	if ((cond->sem.val = semget(IPC_PRIVATE,1,IPC_CREAT | 0755)) == -1) {
		perror("semget");
		raise(SIGINT);
	}	
	// init sem to 1
	if ((initelement(cond->sem.val,0,1)) != 0) {
		perror("semctl:initelement");
		raise(SIGINT);
	}

	return cond;
}

// cleanup condition allocations 
void cleanupcond(cond_t *cond) {
	int i;
	int id;

	// removes set, ignores semnum arg
	if((i = semctl(cond->sem_id,0,IPC_RMID)) != 0) { 
		perror("semctl:IPC_RMID");
	}

	// remove sem_t sem - semaphore set
	if((i = semctl(cond->sem.val,0,IPC_RMID)) != 0) {
		perror("semctl:IPC_RMID");
	}

	// save id of shared cond_t *cond 
	id = cond->shm_id;	
	// detatch cond_t *cond
	if((i = shmdt(cond)) == -1) {
		perror("shmdt");
	}
	// remove shared cond_t *cond
	if ((shmctl(id,IPC_RMID,NULL)) == -1) {
		perror("shmctl:IPC_RMID");
	}
}


