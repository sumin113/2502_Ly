#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* 부모 프로세스가 두 개의 자식 프로세스를 생성한다. */
int main(){
	int pid1, pid2;
	pid1 = fork();
	if(pid1==0){
		printf("[Child1] : Hello, World! pid=%d\n", getpid());
		exit(0);
	}
	pid2 = fork();
	if(pid2==0){
		printf("[Child2] :Hello, world ! pid =%d\n", getpid());
		exit(0);
	}
}
