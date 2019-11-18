//CPSC 3600 - Jack Carroll
//A simple UDP echo server
//usage: udpserver <port>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFSIZE 1024

//error - wrapper for perror
void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char **argv) {
	srand(time(NULL));
	int sockfd; //socket
	int portno; //port number
	int clientlen; //client's address size
	struct sockaddr_in serveraddr; //server address
	struct sockaddr_in clientaddr; //client address
	struct hostent *hostp; //client host info
	char buf[BUFSIZE]; //message buffer
	char *hostaddrp; //dotted decimal host address string
	int optval; //flag value for setsockopt
	int n; //message size

	//check command line arguments
	if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);exit(1);
	}

	portno = atoi(argv[1]);

	//create the parent socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) { error("ERROR opening socket"); }

	//setsockopt: Handy debugging trick that lets us rerun the server immediately after we kill it, otherwise we have to wait about 20 secs. 
	//Eliminates "ERROR on binding: Address already in use" error
	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(const void *)&optval, sizeof(int));

	//build the server's Internet address
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)portno);

	//associate the parent socket with a port
	if (bind(sockfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) { error("ERROR on binding"); }

	//wait for a datagram, then echo it
	clientlen = sizeof(clientaddr);
	int lostPacket;
	while (1) {
		bzero(buf, BUFSIZE);
		n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &clientlen);
		if (n < 0) { error("ERROR in recvfrom"); }

		lostPacket = rand() % 100 + 1;
		if (lostPacket < 33) { printf("packet lost\n"); }
		else {
			//determine who sent the datagram
			hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,sizeof(clientaddr.sin_addr.s_addr), AF_INET);
			if (hostp == NULL) { error("ERROR on gethostbyaddr"); }
			hostaddrp = inet_ntoa(clientaddr.sin_addr);
			if (hostaddrp == NULL) { error("ERROR on inet_ntoa\n"); }
			printf("server received datagram from ip(port):%s(%u)\n",  hostaddrp, ntohs(clientaddr.sin_port));
			printf("server received %d/%d bytes: %s\n", strlen(buf), n, buf);

			//echo the input back to the client
			n = sendto(sockfd, buf, strlen(buf), 0,(struct sockaddr *) &clientaddr, clientlen);
			if (n < 0) { error("ERROR in sendto"); }
		}
	}

}