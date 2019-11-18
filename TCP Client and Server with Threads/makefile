SERV = mpTcpServer.c
CLI = mpTcpClient.c 
S = ./mpTcpServer 5000
C = ./mpTcpClient 130.127.49.11 5000 kicks.txt
EXE = mpTcpClient mpTcpServer

make: 
	gcc -Wall -pthread $(SERV) -o mpTcpServer
	gcc -Wall $(CLI) -o mpTcpClient

server:
	$(S)

client: 
	$(C)

clean:
	rm $(EXE)