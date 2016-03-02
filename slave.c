#include "master.h"

// slave.c
// Brett Lindsay
// Project 2 CS4760

const int p_n = 19;
const int n = 18;
int process_num; // global to inform user on SIGINT
char *msg; // global to release on SIGINT if necessary
FILE *fp; // global to close on SIGINT if necessary
cond_t *cond; // condition for this process

void intr_handler();
void process(int i);

main (int argc, char *argv[]) {
	process_num = atoi(argv[1]); // process num sent from parent process
	int p_index = process_num - 1;
	const int key_sem = 30; // key for semaphore set is 20 
	const int key_shm = 31; // key for shared ints

	signal(SIGINT,intr_handler);

	printf("Slave Process: %d\n",process_num);	
	// init condition for this process
	cond = (cond_t*)initcondition(key_sem,key_shm);
		
	// send a function ptr of this to enter_monitor(funcptr) TODO
	// try to enter monitor 3 times
	// run process to write to file
	process(p_index+1);

	// cleanup
	cleanupcond(cond);	
}

void process(int i) {
	time_t tcurrent;
	struct tm *timeinfo;
	
	time(&tcurrent);
	timeinfo = localtime(&tcurrent);
	fprintf(stderr, "Process %d entering critical section at %d:%02d:%02d\n", 
		i, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		
	critical_section(i);
	time(&tcurrent);
	timeinfo = localtime(&tcurrent);
	fprintf(stderr, "Process %d exiting critical section at %d:%02d:%02d\n",
		i,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	// remainder_section();
} // end process()

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

// interupt handler
void intr_handler() {
	signal(SIGINT,SIG_DFL); // make sure handler stays defined

	// cleanup
	cleanupcond(cond);	

	if (msg != NULL) { // if allocated memory, free it
		free(msg);
	}
	if (fp != NULL) { // if file open, close it
		fclose(fp);
	}	
	fprintf(stderr,"Recieved SIGINT: Process %d cleaned up and dying.\n",
		process_num);
	// let it do default actions for SIGINT by resending now
	raise(SIGINT);
}
