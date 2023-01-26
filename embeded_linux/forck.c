#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/*
* What will be printed?
* In the most cases: "Hello" and pid of children process.
* But actually undefined: if the children process finished before system library call return value from fork() - "Hello" and 0
*/

static pid_t p = 0;

void sig_handler(int signum) {
	printf("%d\n", p);
}

void main()
{
	int ret;
	printf("Hello!\n");
	signal(SIGCHLD, sig_handler); // set listener for finish child process
	p = fork();
	waitpid(p, &ret, 0); // wait for finish child process
}
