#include "master.h"

// master.c
// Brett Lindsay
// Project 3 CS4760

const int p_n = 19; // process number to send each process
const int n = 18; // its respective place in the flag array (1 less)
char *arg2; // to send execl process args
char *arg3; // to send execl process args
int pids[18] = { 0 };
bool timed_out = false;
void timeout();
void free_mem();
const int SIZE = 3;

main(int argc, char *argv[]) {
	char *arg1 = "slave"; // to send execl process argv[0]
	arg2 = malloc(sizeof(int)); // to send execl process args
	arg3 = malloc(sizeof(int)); // to send execl process args
	int pid;
	const int key_sem = 30; // key of shared semaphore set
	const int key_shm = 31; // key of shared int
	int act_procs = 0; // active process counter
	int i = 0; // index var

	// signal handling: timeout - 60s, on ctrl-c free memory allocated and quit
	signal(SIGALRM,timeout);
	signal(SIGINT,free_mem);
	
	if (argc > 1) { // user specified timeout period
		i = atoi(argv[1]); 
		fprintf(stderr,"Setting timeout for %d seconds\n",i);
		alarm(i);
	} else { // default
		fprintf(stderr,"Setting timeout for 60 seconds (default)\n");
		alarm(300);
	}
	// allocate shared memory/semaphore set
	initsharedsems(key_sem);

	// fork for each child process to create
	for(i = 1; i <= p_n; i++) { // 1 through 19
		sprintf(arg2,"%d", i); // var for process number for each process
		act_procs++; // increment each time a new process is created
		if (act_procs > 20) {
			fprintf(stderr,"Too many processes created. Fatal error.");
			raise(SIGINT);
		}
		pid = fork();
		if (pid < 0) { // error checking
			perror("fork:");
		}
		if (pid == 0) { // don't let children spawn more children
			break;        // for clarity, could just use execl at this point
		} else {
			pids[(i-1)] = pid; // save each process pid
		}
	}
	if (pid == 0) { // children process actions
		execl("slave", arg1, arg2, 0); // start a slave process
	}
	else if (pid > 0) { // parent process actions
		for(i = 0; i < n; i++) { // wait for children to finish
			wait(); 
			act_procs--;
		}
		printf("In master-finished tasks. Cleaning up and quiting.\n");

		// clean up shared sem set and int
		cleanupshared(key_sem, key_shm);

		// free argument memory process num transfer 
		free(arg2);
	} // end else for pid > 0 -> parent process actions
} // end main

void free_mem() {
	int sem_id;
	int i; // counter
	int key = 20; // key for semaphore set
	
	fprintf(stderr,"Received SIGINT. Cleaning up and quiting.\n");
	// kill each process if program timed out
	if (timed_out = true) {
		for(i = 0; i < 19; i++) { // 0-18
			kill(pids[i],SIGINT); // kill child process
			waitpid(pids[i],NULL,0);
		}
		// to be safe
		system("killall slave");
	}
	// get the sem_id of semaphore set 
	if((sem_id = semget(key,SIZE,0755)) == -1) {
		perror("semget:");
		exit(1);
	}
	// free allocated memory
	if((shmctl(sem_id, 0, IPC_RMID)) == -1) {
		perror("semctl:IPC_RMID");
		exit(1);
	}	
	free(arg2);

	signal(SIGINT,SIG_DFL); // restore default action to SIGINT
	raise(SIGKILL); // take normal action for SIGINT after cleanup
}

void timeout() {
	// timeout duration passed send SIGINT
	timed_out = true;
	fprintf(stderr,"Timeout duration reached.\n");
	raise(SIGINT);
}
