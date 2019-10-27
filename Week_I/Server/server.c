/*
	HoTen: Trinh Ngoc Huy
	MSSV: 16022440
	Content: SERVER
		- Server run, listen and reply message from client
*/

#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<strings.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {

	int port = 1998;
	int sockfd, newsockfd;
	struct sockaddr_in serv_addr, cli_addr;

	int nbytes;
	int bufferLength;
	int sendBuffer;
	char buffer[1024];

	// Create a socket to listen
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("Server socket error");
		return 1;
	}

	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
	serv_addr.sin_family = AF_INET;

	int bindCheck = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if (bindCheck < 0) {
		perror("ERROR on binding");
		return 1;
	}

	int listenCheck = listen(sockfd, 1024);
	if (listenCheck < 0) {
		perror("ERROR on listening");
		return 1;
	}

	printf("Server is listening at port %d\n", port);

	unsigned int addrLength = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &addrLength);
	if (newsockfd < 0) {
		perror("ERROR on accepting");
		return 1;
	}

	// Receive information

	char *clientIpAddr = inet_ntoa(cli_addr.sin_addr);
	int clientPort = ntohs(cli_addr.sin_port);

	printf("Client address: %s:%d\n", clientIpAddr, clientPort);

	nbytes = read(newsockfd, buffer, sizeof(buffer));
	if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}

	printf("Client: %s\n", buffer);

	// Send hello message
	char helloMessage[] = "Hello Client!";

	sendBuffer = htonl(sizeof helloMessage);
	nbytes = write(newsockfd,(char*)&sendBuffer, sizeof(sendBuffer));
	if(nbytes < 0){
		perror("ERROR on writing");
		return 1;
	}

	nbytes = write(newsockfd, helloMessage, sizeof(helloMessage));
	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}

	// Send message ask name
	char clientName[] = "What is your name?";

	sendBuffer = htonl(sizeof clientName);
	nbytes = write(newsockfd,(char*)&sendBuffer, sizeof(sendBuffer));
	if(nbytes < 0){
		perror("ERROR on writting");
		return 1;
	}	
	
	nbytes = write(newsockfd, clientName, sizeof(clientName));
    	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}

	// Receive name
	char nameBuffer[1024 * 10];

	nbytes = read(newsockfd,(char*)&bufferLength, sizeof(bufferLength));
	if(nbytes < 0){
		perror("ERROR on writing");
		return 1;
	}
	bufferLength = ntohl(bufferLength);
	nbytes = read(newsockfd, nameBuffer, bufferLength);
    	if (nbytes < 0) {
        	perror("ERROR on reading");
        	return 1;
    	}
	printf("Client: %s\n", nameBuffer);


	// Receive bye 

	char byeBuffer[1024];

	nbytes = read(newsockfd,(char*)&bufferLength, sizeof(bufferLength));
	if(nbytes < 0){
		perror("ERROR on writing");
		return 1;
	}
	bufferLength = ntohl(bufferLength);
	nbytes = read(newsockfd, byeBuffer, bufferLength);
	if (nbytes < 0) {
        perror("ERROR on reading");
        return 1;
    }
	printf("Client: %s\n", byeBuffer);

	close(newsockfd);
	close(sockfd);
	printf("Closed connection.\n");

	return 0;
}
