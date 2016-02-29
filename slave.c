#include "master.h"

// slave.c
// Brett Lindsay
// Project 2 CS4760

const int p_n = 19;
const int n = 18;
int process_num; // global to inform user on SIGINT
char *msg; // global to release on SIGINT if necessary
FILE *fp; // global to close on SIGINT if necessary
const int SIZE = 3; // size of sem set

void intr_handler();

main (int argc, char *argv[]) {
	int sem_id;
	process_num = atoi(argv[1]); // process num sent from parent process
	int p_index = process_num - 1;
	const int key = 20; // key for semaphore set is 20 
	
	signal(SIGINT,intr_handler);

	printf("Slave Process: %d\n",process_num);	
	// access semaphore set
	if ((sem_id = semget(key, SIZE, 0755)) == -1) {
		perror("Failed to access semaphore set");
		exit(1);
	}

	// run process to write to file
	process();
}

void process() {
		time(&tcurrent);
		timeinfo = localtime(&tcurrent);
		fprintf(stderr, "Process %d entering critical section at %d:%02d:%02d\n", 
			(i+1), timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			
		critical_section(i+1);
		time(&tcurrent);
		timeinfo = localtime(&tcurrent);
		fprintf(stderr, "Process %d exiting critical section at %d:%02d:%02d\n",
			(i+1),timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

		// remainder_section();
} // end enterCriticalSection()

critical_section(int id) {
	time_t tcurrent;
	struct tm *timeinfo;
	int r;
	srandom(time(NULL));
	// open file	
	fp = fopen("cstest", "a");
	if (!fp) { // error checking on file open
		perror("fopen error");
		return -1;
	}	
	// sleep for 0-2 seconds
	r = random() % 3;
	sleep(r);

	// create/alloc memory for string to write
	time(&tcurrent);
	timeinfo = localtime(&tcurrent);
	asprintf(&msg, "File modified by process number %d at time %d:%02d:%02d\n",
		id, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	// write to file
	fprintf(fp, "%s", msg);

	// sleep for 0-2 seconds
	r = random() % 3;
	sleep(r);

	// clean up
	free(msg);
	msg = NULL;
	fclose(fp);
	fp = NULL;
}

void intr_handler() {
	int pid;
	info_t *tmp;
	signal(SIGINT,SIG_DFL); // make sure handler stays defined
	if (s_info != NULL)  { // if attached to shared memory, detach
		shmdt(tmp);
	}	
	if (msg != NULL) { // if allocated memory, free it
		free(msg);
	}
	if (fp != NULL) { // if file open, close it
		fclose(fp);
	}	
	fprintf(stderr,"Recieved SIGINT: Process %d detached s_info and dying.\n",
		process_num);
	// let it do default actions for SIGINT by resending now
	raise(SIGINT);
}
