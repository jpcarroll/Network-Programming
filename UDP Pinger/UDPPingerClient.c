//CPSC 3600 - Jack Carroll
//A simple UDP client
//usage: udpclient <host> <port>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include <poll.h>
#define BUFSIZE 1024

//error - wrapper for perror
void error(char *msg) {
	perror(msg);
	exit(0);
}

//find time delay
double delay(struct timeval s, struct timeval e) {
	return (e.tv_sec - s.tv_sec);
}

int main(int argc, char **argv) {

	int sockfd, portno, n;
	int serverlen;
	struct sockaddr_in serveraddr;
	struct hostent *server;
	char *hostname;
	char buf[BUFSIZE] = { 'p', 'i', 'n', 'g', '\0' };
	struct timeval start, end;

	//check command line arguments
	if (argc != 3) {
		fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);exit(0);
	}
	hostname = argv[1];
	portno = atoi(argv[2]);

	//create the socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) { error("ERROR opening socket"); }

	//get the server's DNS entry
	server = gethostbyname(hostname);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host as %s\n", hostname);
		exit(0);
	}

	//move h_addr to ip_addr. This enables conversion to a form suitable for printing with the inet_ntoa() function
	struct in_addr ip_addr;
	ip_addr = *(struct in_addr *)(server->h_addr);
	printf("Hostname: %s, was resolved to: %s\n", hostname, inet_ntoa(ip_addr));

	//build the server's Internet address
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,  (char *)&serveraddr.sin_addr.s_addr, server->h_length);
	serveraddr.sin_port = htons(portno);

	//send the message to the server
	serverlen = sizeof(serveraddr);
	printf("ping %s\n", inet_ntoa(ip_addr));
	int i;
	struct pollfd fd;
	int res;

	for (i = 1; i <= 10; i++) {
		gettimeofday(&start, NULL);
		fd.fd = sockfd;
		fd.events = POLLIN;
		res = poll(&fd, 1, 10000); 

		n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
		if (n < 0)
			error("ERROR in sendto");
		if (res == 0) {
			printf("request timed out\n");
		}
		else {
			n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
			if (n < 0)
				error("ERROR in recvfrom");
			gettimeofday(&end, NULL);

			printf("%d bytes from %s: seq=%d rtt=%0.2lf ms\n", strlen(buf), hostname, i, delay(start, end));
		}
	}

	return 0;

}