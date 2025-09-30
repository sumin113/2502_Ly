#include <stdio.h>
#include <unistd.h>

int main() {
	int pig;
	printf("[%d] Before fork()\n", getpid());
	pid = fork();
	printf("[%d] Process : return valuse = %d\n", getpid(), pid);
}
