#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

/*
* 1. parallel acces from different processes
* 2. parallel acces from different threads
*/

void main()
{
	printf("Hello!\n");
	FILE* fd = fopen("f.txt", "w");

	int res = fprintf(fd, "main\n");
	if (res < 0) {
		printf("Fail main write: %s\n", strerror(errno));
		return;
	}
	fflush(fd);

	FILE* fd2 = fopen("f.txt", "r");

	pid_t p = fork();
	if (p < 0) {
		printf("Fail fork: %s\n", strerror(errno));
		return;
	}
	else if (p > 0) { // parent
		int res_p = fprintf(fd, "parent\n");
		if (res_p < 0) {
			printf("Fail parent write: %s\n", strerror(errno));
			return;
		}
		printf("parent write %d\n", res_p);
		fflush(fd);

		wait(NULL);

		char buf[10];
		fscanf(fd2, "%s", buf);
		printf("parent read: %s\n", buf);
	}
	else { // child
		int res_c = fprintf(fd, "child\n");
		if (res_c < 0) {
			printf("Fail child write: %s\n", strerror(errno));
			return;
		}
		printf("child write %d\n", res_c);
		fflush(fd);
		char buf[10];
		fscanf(fd2, "%s", buf);
		printf("child read: %s\n", buf);
		fclose(fd2);
	}
}
