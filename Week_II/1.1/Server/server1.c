/*
Ho_ten: Trinh Ngoc Huy
MSSV: 16022440

Content: Server

-	Server receive file from Client
	
*/
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<unistd.h>

int main() {

	int port = 1998;
	int sockfd, newsockfd;
	struct sockaddr_in serv_addr, cli_addr;
	int option = 1;
	int nbytes;
	char buffer[1024];

	// Create a socket to listen
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
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

	printf("Server is listening at port  %d\n", port);

	unsigned int addr512 = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &addr512);
	if (newsockfd < 0) {
		perror("ERROR on accepting");
		return 1;
	}

	// Receive information
	char *clientIpAddr = inet_ntoa(cli_addr.sin_addr);
	int clientPort = ntohs(cli_addr.sin_port);
	printf("Client address: %s:%d\n", clientIpAddr, clientPort);

	// Server read hello message of client
	int size_Receive_Message;
	char helloMessageFromClient[1024];

	nbytes = read(newsockfd, (char *)&size_Receive_Message, sizeof(size_Receive_Message));
	if(nbytes < 0){
		perror("ERROR on reading");
		return 1;
	}
	size_Receive_Message = ntohl(size_Receive_Message);
	nbytes = read(newsockfd, helloMessageFromClient, size_Receive_Message);
	if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}
	printf("Client: %s\n", helloMessageFromClient);

	// Send hello message
	char helloMessageToClient[] = "Hello Client! OK I'm ready !";
	int size_Send_Message;
	size_Send_Message = htonl(sizeof(helloMessageToClient));
	nbytes = write(newsockfd, (char *)&size_Send_Message, sizeof(size_Send_Message));
	if(nbytes <0){
		perror("ERROR on writing");
		return 1;
	}

	nbytes = write(newsockfd, helloMessageToClient, sizeof(helloMessageToClient));
	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}

	// Send message choose file
	char choosefileMessage[] = "Please choose a file: ";
	size_Send_Message = htonl(sizeof(choosefileMessage));
	nbytes = write(newsockfd, (char *)&size_Send_Message, sizeof(size_Send_Message));
	if(nbytes < 0){
		perror("ERROR on writing");
		return 1;
	}

	nbytes = write(newsockfd, choosefileMessage, sizeof(choosefileMessage));
	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}

	// Receive file name
	nbytes = read(newsockfd, (char *)&size_Receive_Message, sizeof(size_Receive_Message));
	if(nbytes < 0 ){
		perror("ERROR on reading");
		return 1;
	}
	size_Receive_Message = ntohl(size_Receive_Message);
	nbytes = read(newsockfd, buffer, size_Receive_Message);
    if (nbytes < 0) {
		perror("Read error");
		return 1;
	}

	// Check file exist
	int fileExist;
	if(access(buffer,0) != -1){
		fileExist = 1;
		nbytes = write(newsockfd, &fileExist, sizeof(int));
			if (nbytes < 0) {
				perror("ERROR on writing");
				return 1;
			}
		close(newsockfd);
		close(sockfd);
		printf("Closed connection.\n");
		return 0;
	}
	else{
		fileExist = 0;
		nbytes = write(newsockfd, &fileExist, sizeof(int));
			if (nbytes < 0) {
				perror("ERROR on writing");
				return 1;
			}
	}
	// Receive file size
	unsigned int fileSize = 0;
    nbytes = read(newsockfd, &fileSize, sizeof(fileSize));
    if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}


	//Receive file
	char receiveBuffer[1024];
    unsigned int receivedSize = 0;
	FILE *freceive = fopen(buffer, "wb");
	while (receivedSize < fileSize) {
		int currentReceivedSize = read(newsockfd, receiveBuffer, 1024);
		receivedSize += currentReceivedSize;
		fwrite(receiveBuffer, 1, currentReceivedSize, freceive);
    }
    printf("Server: Receive Successfully \n");

	close(newsockfd);
	close(sockfd);
	printf("Closed connection.\n");

	return 0;
}