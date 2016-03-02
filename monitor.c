#include "monitor.h"

// monitor takes a function pointer (function w/ int as parameter)
// and an int to run with the function pointer
void enter_monitor(int proc_num, cond_t *cond) {
		
	//wait_cond(cond);
	wait_sem(cond->sem_id,0);

	printf("\"Running\" process %d\n", proc_num);
	sleep(3);
	signal_sem(cond->sem_id,0);
	/*if (cond->s_info->next_count > 0) {
		printf("next_count > 0 true\n");
		signal_cond(cond);
	} else {
		printf("next_count > 0 false\n");
		signal_cond(cond);
	}*/
}
