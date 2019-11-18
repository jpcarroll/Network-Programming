//CPSC 3600 - Jack Carroll
//Server Program

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define maxLength 4096 //maximum length of text on line
#define listenQ 8 //maximum number of clients to listen to

int listenfd, connfd, n;
socklen_t clilen;
char buf[maxLength];
struct sockaddr_in cliaddr, servaddr;
pthread_t child;
int client = 1;

//reverses a given string
//not my own code - taken from stack overflow
//https://stackoverflow.com/questions/34035169/fastest-way-to-reverse-a-string-in-c/34035474
char* reverse(char *str, int len)
{
	int i;
	for (i = len/2-1 ; i >= 0 ; --i) {
		char c = str[i];
		str[i] = str[len-i-1];
		str[len-i-1] = c;
	}
	return str;
}

//creates server side file name with file name from client
void getName(char *fp, char *buf, int client) {
	for (int i = 0; i < maxLength; i++) {
		if (buf[i] == '.') { break; }
		else { fp[i] = buf[i]; }
	}
	sprintf(fp + strlen(fp), "_");
	sprintf(fp + strlen(fp), "%d", client);
	sprintf(fp + strlen(fp), ".txt");
	return;
}

void* cliThread(void *arg) {

	//get file name from client, create server side file and send name back to client with ack
	n = read(connfd, buf, maxLength);

	//checks for error reading
	if (n < 0) {
		printf("%s\n", "Read error");
		exit(3);
	}

	char fp[maxLength];
	getName(fp, buf, client);
	send(connfd, fp, n, 0);
	send(connfd, "ack", n, 0);
	printf("writing to file %s\n", fp);
	FILE *input = fopen(fp, "w");

	//checks for file error
	if (fp == NULL) {
		perror("File error:\n");
		exit(4);
	}

	//gets file line by line and sends ack back for each line received
	while (read(connfd, buf, maxLength)) {
		fprintf(input, "%s", reverse(buf, strlen(buf)));
		send(connfd, "ack", n, 0);
		if (strcmp(buf, "done") == 0) { break; }
	}

	fclose(input);

	printf("Finished writing to file %s\n", fp);
	printf("%s\n","Waiting for clients to connect to server...");

	//clear fp buffer
	memset(fp, 0, sizeof(fp));
	client++;

	return 0;
}

int main (int argc, char **argv) {

	//checks for argument errors
	if (argc != 2) {
		perror("Usage: ./mpTcpServer <port>");
		exit(1);
	}

	//checks for error in creating socket
	if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
		perror("Problem in creating the socket");
		exit(2);
	}

	//creates socket address
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	//binds socket
	bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	//listens to the socket by creating a connection queue and waits for clients to connect
	listen (listenfd, listenQ);

	printf("%s\n","Waiting for clients to connect to server...");

	while (1) {
		clilen = sizeof(cliaddr);

		//accepts connection from client
		connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);

		printf("%s\n","Received client request...");

		printf ("Client %d:\n", client);

		//create thread and don't keep track of it
		pthread_create(&child, 0, cliThread, &connfd);
		pthread_detach(child);
	}

	//closes socket
	close(connfd);
}
