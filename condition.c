#include "condition.h"

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
cond_t* initcondition(int key_sem, int key_cond) {
	int shm_id;
	cond_t *cond;
	shm_id = shmget(key_cond, sizeof(cond_t*),IPC_CREAT | 0755);
	cond = (cond_t*) shmat(shm_id,0,0);

	// init shared semaphores is called separately
	// save sem_id
	if ((cond->sem_id = semget(key_sem,2,IPC_CREAT | 0755)) == -1) {
		perror("semget");
		exit(1);
	}
	// init sem_t sem
	if ((cond->sem.val = semget(IPC_PRIVATE,1,IPC_CREAT | 0755)) == -1) {
		perror("semget");
		exit(1);
	}	
	// actual init
	if ((initelement(cond->sem.val,0,1)) != 0) {
		perror("semctl:initelement");
		exit(1);
	}
	// init int vars	
	cond->num_waiting_procs = 0;
	cond->next_count = 0;
	//shm_id = shmget(key_shm, sizeof(info_t*),IPC_CREAT | 0755);
	//cond->s_info = (info_t*)shmat(shm_id,0,0);
	//cond->s_info->next_count = 0;

	return cond;
}

// call once to init shared semaphores
void initsharedsems(int key_sem) {
	int sem_id;

	// 0 is mutex; 1 is next   
	sem_id = semget(key_sem,2,IPC_CREAT | 0755); // 2 semaphores 
	// init mutex to 1
	if((initelement(sem_id,0,1)) != 0) {
		perror("semctl:initelement");
		exit(1);
	}
	// init next to 0
	if ((initelement(sem_id,1,0)) != 0) {
			perror("semctl:initelemnt");
			exit(1);
	}
}

// call to clean up non-shared part of cond_t
void cleanupcond(cond_t *cond) {
	int i;
	int id;
	/*info_t *tmp = cond->s_info;
	// detach shared int
	if((i = shmdt(tmp)) == -1) {
		perror("shmdt");
	}*/
	if((i = shmdt(cond)) == -1) {
		perror("shmdt");
	}
	// remove sem_t sem - semaphore set
	id = cond->sem.val;	
	if((i = semctl(id,0,IPC_RMID)) != 0) {
		perror("semctl:IPC_RMID");
	}
	//free(cond);
}

// call only after cleanupcond for each cond_t being used
void cleanupshared(int key_sem, int key_cond) {
	int i;
	int id;
	// clean up mutex and next semaphores, will only confirm perms if exists	
	if ((id = semget(key_sem,2,IPC_CREAT | 0755)) == -1) {
		perror("semget");
		exit(1);
	}
	if((i = semctl(id,0,IPC_RMID)) != 0) { // removes set, ignores semnum arg
		perror("semctl:IPC_RMID");
		exit(1);
	}
	// remove shared next_count int	
	if ((id = shmget(key_cond,sizeof(cond_t*),IPC_CREAT | 0755)) == -1) {
		perror("shmget");
		exit(1);
	}
	if ((shmctl(id,IPC_RMID,NULL)) == -1) {
		perror("shmctl:IPC_RMID");
		exit(1);
	}
}


