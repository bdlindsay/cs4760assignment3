#include "semaphore.h"


void wait_sem(int sem_id, int sem_num) {
	struct sembuf sops[1];
	sops[0].sem_num = sem_num; // which semaphore in set sem_id
	sops[0].sem_op = -1; // decrement by 1
	sops[0].sem_flg = 0; // no flag
	// wait
	//while((semctl(sem_id, sem_num, GETVAL)) <= 0);
	// then decrement
	while((semop(sem_id, sops, 1)) == -1) {
		perror("semop:wait");
		if (errno != EINTR)
			return;
	}	
}

void signal_sem(int sem_id, int sem_num) {
	struct sembuf sops[1];
	sops[0].sem_num = sem_num; // which semaphore in set sem_id
	sops[0].sem_op = 1; // increment by 1
	sops[0].sem_flg = 0; // no flag
	// increment by 1
	if((semop(sem_id, sops, 1)) == -1) {
		perror("semop:signal");
		exit(1);
	}
}

int initelement(int id, int num, int val) {
	union semun {
		int val;
		struct semid_ds *buf;
		unsigned short *array;
	} arg;

	arg.val = val;

	return semctl(id, num, SETVAL, arg);
}



