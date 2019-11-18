//CPSC 3600 - Jack Carroll
// MPTCP Client Program

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define maxLength 4096 //maximum length of text on line

int main(int argc, char **argv) {

	int sockfd;
	struct sockaddr_in servaddr;
	char buf[maxLength];
	char receive[maxLength];

	//checks for argument errors
	if (argc != 4) {
		perror("Usage: ./mpTcpClient <ip> <port> <file>");
		exit(1);
	}

	//opens file from client and checks for file error
	FILE *fp = fopen(argv[3], "r");
	if (fp == NULL) {
		perror("File error:");
		exit(2);
	}

	//checks for error in creating socket
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
		perror("Problem in creating the socket");
		exit(3);
	}

	//creates socket address
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr= inet_addr(argv[1]);
	servaddr.sin_port =  htons(atoi(argv[2])); 

	//checks connection of client to server
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
		perror("Problem in connecting to the server");
		exit(4);
	}

	//sends file name to server
	write(sockfd, argv[3], strlen(argv[3]));

	//checks on the server side file name returned from server
	if (recv(sockfd, receive, maxLength, 0) == 0){
		perror("Server error retrieving file name\n");
		exit(5);
	}

	//prints out server side file name
	else {
		printf("Server's file: %s\n", receive);
		printf("file name received\n");
	}

	//go through file line by line, and send to server after receiving each ack
	while (fgets(buf, maxLength, fp) != NULL) {
		write(sockfd, buf, maxLength);
		if (recv(sockfd, receive, maxLength,0) == 0){
			perror("The server terminated prematurely");
			exit(6);
		}

		else if (strcmp(receive, "ack") != 0) {
			perror("Error reading from file\n");
			exit(7);
		}

		printf("Server: %s\n", receive);
	}

	//sends an ack to server that entire file has been sent
	write(sockfd, "done", maxLength);

	printf("File has been recieved by server\n");

	fclose(fp);
	exit(0);
}
