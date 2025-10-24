#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int S[100], A[100], B[100];
int cnt = 0;

void *TaskCode(void *argument){

	int tid;
	tid = *((int*)argument);
	for(int i=tid*25000; i<(tid+1)*25000; i++){
		cnt += 1;
	}

	return NULL;
}

int main(int argc, char *argv[]){
	pthread_t threads[4];
	int args[4];
	int i;

	for(i=0; i<100000 ;i++){
		A[i] = i;
		B[i] = i;
	}

	/* creat all thread */
	for(i=0; i<4; i++){
		args[i] = i;
		pthread_create(&threads[i],NULL,TaskCode,(void*)&args[i]);
	}

	/* wait for all threads to complete */
	for(i=0; i<4; i++){
		pthread_join(threads[i], NULL);
	}

	printf("%d\n", cnt);
	return 0;
}
