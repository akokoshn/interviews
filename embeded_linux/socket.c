#include "stdio.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include<pthread.h>

static int count = 0;
static int connections[10];


static void* doWrite(void*)
{
    char sendBuff[10];
    while (1) {
        for (int i = 0; i < count; i++) {
            memset(sendBuff, 0, 10);
            snprintf(sendBuff, sizeof(sendBuff), "%d", i);
            write(connections[i], sendBuff, strlen(sendBuff));
        }
        printf("server write data for %d clients\n", count);
        sleep(1);
    }
    return NULL;
}

/*
* 1. create socket
* 2. create address and bind socket with address
* 3. set socket as listener
* 4. accept connection or wait for conection
*/

void server_run()
{
    int listenfd = 0;
    struct sockaddr_in serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);// IP
    serv_addr.sin_port = htons(5000);// port

    // associate socket with address
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd/*must be SOCK_STREAM or SOCK_SEQPACKET*/, 10/*connections queue length*/);

    printf("run server: %d:%d\n", serv_addr.sin_addr.s_addr, serv_addr.sin_port);

    pthread_t tid;
    pthread_create(&tid, NULL, &doWrite, NULL);
    printf("create writer thread\n");

    while (1)
    {
        // extract 1th connection from queue, block till smeone try to connect
        connections[count] = accept(listenfd/*must be SOCK_STREAM or SOCK_SEQPACKET*/, (struct sockaddr*)NULL, NULL);
        count++;
        sleep(1);
    }

    for (int i = 0; i < count; i++) {
        pthread_cancel(tid);
        close(connections[count]);
    }
}

/*
* 1. create socket
* 2. create address of server and connect socket to address
* 3. read data from socket
*/
void client_run()
{
    int sockfd = 0;
    int n = 0;
    char recvBuff[10];
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    printf("client %d connected\n", getpid());

    while (1)
    {
        while (n = read(sockfd, recvBuff, 10) > 0) {
            printf("  client %d read %d bytes from socket: %s\n", getpid(), n, recvBuff);
        }
        sleep(1);
    }
}

void main()
{
	printf("Hello!\n");

	pid_t p = fork();
	if (p < 0) {
		printf("Fail fork: %s\n", strerror(errno));
		return;
	}
	else if (p > 0) { // parent - server
        server_run();
	}
	else { // child - client
        pid_t pp = fork();
        if (pp < 0) {
            printf("Fail fork: %s\n", strerror(errno));
            return;
        }
        else if (pp > 0) {
            client_run();
        }
        else {
            client_run();
        }
	}
}
