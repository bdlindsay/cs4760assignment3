#include "master.h"

// slave.c
// Brett Lindsay
// Project 2 CS4760

const int p_n = 19;
const int n = 18;
int process_num; // global to inform user on SIGINT
char *msg; // global to release on SIGINT if necessary
FILE *fp; // global to close on SIGINT if necessary
cond_t *cond = NULL; // condition for this process

void intr_handler();
void process(int i);

main (int argc, char *argv[]) {
	process_num = atoi(argv[1]); // process num sent from parent process
	int p_index = process_num - 1;
	const int key_sem = 30; // key for semaphore set is 20 
	//const int key_shm = 31; // key for shared ints
	const int key_cond = 32;
	time_t tcurrent; // for time reporting
	struct tm *timeinfo; // for time reporting
	int write_count = 0;
	
	signal(SIGINT,intr_handler);

	printf("Slave Process: %d\n",process_num);	

	// try to enter monitor 3 times to write to file
	while (write_count < 3) {
		time(&tcurrent);
		timeinfo = localtime(&tcurrent);
		fprintf(stderr, "Process %d attempt to enter monitor  at %d:%02d:%02d\n", 
			process_num, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

		enter_monitor(process_num, &critical_section);

		/*time(&tcurrent);
		timeinfo = localtime(&tcurrent);
		fprintf(stderr, "Process %d attempt to enter monitor at %d:%02d:%02d\n",
			process_num,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);*/
		write_count++;
	}
	// cleanup
	//cleanupcond(cond);	
}

void critical_section(int id) {
	time_t tcurrent;
	struct tm *timeinfo;
	int r;
	srandom(time(NULL));
	// open file	
	fp = fopen("cstest", "a");
	if (!fp) { // error checking on file open
		perror("fopen error");
		return;
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
	signal(SIGINT,SIG_DFL); // change SIGINT back to default handling

	// cleanup
	//cleanupcond(cond);	

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
