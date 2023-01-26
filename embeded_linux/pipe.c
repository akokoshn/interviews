#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void main()
{
	int fd[2];
	printf("Hello!\n");
	if (pipe(fd) < 0) {
		printf("Fail to create pipe: %s\n", strerror(errno));
		return;
	}
	pid_t p = fork();
	if (p < 0) {
		printf("Fail fork: %s\n", strerror(errno));
		return;
	}
	else if (p > 0) { // parent
		pid_t p2 = fork();
		if (p2 < 0) {
			printf("Fail fork: %s\n", strerror(errno));
			return;
		}
		else if (p2 > 0) { // second child
			close(fd[1]);
			char buf[10];
			int count = 0;
			while (1) {
				int n = read(fd[0], &buf[count], 1);
				if (n < 0) {
					printf("Fail to read: %s\n", strerror(errno));
					return;
				}
				if (buf[count] == '\0') {
					break;
				}
				count += n;
			}
			close(fd[0]);
			printf("Recived: %s\n", buf);
		}
		else {
			close(fd[0]);
			char* buf = "246";
			for (int i = 0; i < 4; i++) {
				int n = write(fd[1], &buf[i], 1);
				if (n < 0) {
					printf("Fail to read: %s\n", strerror(errno));
					return;
				}
				sleep(1);
			}
			close(fd[1]);
			return;
		}
	}
	else { // child
		close(fd[0]);
		char* buf = "135";
		for (int i = 0; i < 4; i++) {
			int n = write(fd[1], &buf[i], 1);
			if (n < 0) {
				printf("Fail to read: %s\n", strerror(errno));
				return;
			}
			sleep(1);
		}
		close(fd[1]);
	}
}
