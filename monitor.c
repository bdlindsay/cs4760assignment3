#include "monitor.h"

// monitor takes a function pointer (function w/ int as parameter)
// and an int to run with the function pointer
void enter_monitor(int proc_num, void (*enter_cs)(int)) {
	time_t tcurrent;
	struct tm *timeinfo;
	int shm_id;
	int key_cond = 32;
	cond_t *cond;
	
	shm_id = shmget(key_cond,sizeof(cond_t*),0755);
	cond = shmat(shm_id,0,0);

	wait_sem(cond->sem_id,0); // mutex.wait() - wait for free critical section
	wait_cond(cond);

	time(&tcurrent);
	timeinfo = localtime(&tcurrent);
	fprintf(stderr, "Process %d entering critical section at %d:%02d:%02d\n",
		proc_num,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	
	// execute the function sent to the monitor
	(*enter_cs)(proc_num);
	
	time(&tcurrent);
	timeinfo = localtime(&tcurrent);
	fprintf(stderr, "Process %d exiting critical section at %d:%02d:%02d\n",
		proc_num,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	if (cond->next_count > 0) {
		signal_sem(cond->sem_id,1);	// next.signal();
		signal_cond(cond);
	} else {
		signal_cond(cond);
	}

	signal_sem(cond->sem_id,0); // done with critical section
	shmdt(cond);
}
