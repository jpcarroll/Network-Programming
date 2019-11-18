//CPSC 3600 - Jack Carroll
// MPTCP Server Program

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define maxLength 4096 //maximum length of text on line
#define listenQ 8 //maximum number of clients to listen to

//reverses a given string
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

int main (int argc, char **argv) {

int listenfd, connfd, n;
pid_t childpid;
socklen_t clilen;
char buf[maxLength];
struct sockaddr_in cliaddr, servaddr;
int client = 0;

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

	for ( ; ; ) {

		clilen = sizeof(cliaddr);
		client++;

		//accepts connection from client
		connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);

		printf("%s\n","Received client request...");

		//if it’s 0, it’s child process
		if ( (childpid = fork ()) == 0 ) {
			printf ("Client %d:\n", client);

			//close listening socket
			close (listenfd);

			//get file name from client, create server side file and send name back to client with ack
			n = read(connfd, buf, maxLength);
			char fp[maxLength];
			getName(fp, buf, client);
			send(connfd, fp, n, 0);
			send(connfd, "ack", n, 0);
			printf("writing to file %s\n", fp);
			FILE *input = fopen(fp, "w");

			//checks for file error
			if (fp == NULL) {
				perror("File error:\n");
				exit(3);
			}

			//gets file line by line and sends ack back for each line received
			while (read(connfd, buf, maxLength)) {
				fprintf(input, "%s", reverse(buf, strlen(buf)));
				send(connfd, "ack", n, 0);
				if (strcmp(buf, "done") == 0) { break; }
			}

			fclose(input);
			printf("Finished writing to file %s\n", fp);

			//checks for error reading
			if (n < 0) {
				printf("%s\n", "Read error");
				exit(4);
			}

			printf("%s\n","Waiting for clients to connect to server...");
			exit(0);
		}

		//closes socket
		close(connfd);
	}
}
