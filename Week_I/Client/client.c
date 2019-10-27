/*
	HoTen: Trinh Ngoc Huy
	MSSV: 16022440
	Content: CLIENT
		- Client run, user insert ip for server and after insert name of client when Server ask.
*/

#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<strings.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(){

	int sockfd;
	int port = 1998;
	struct sockaddr_in serv_addr;

	char* serverIpAddr = (char *) malloc(100 * sizeof(char *));
	
	int nbytes;
	int bufferLength;
	int sendBuffer;


	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("ERROR socket server");
		return 1;
	}

	bzero(&serv_addr, sizeof(serv_addr));

	printf("IP Server: ");
	scanf("%s", serverIpAddr);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serverIpAddr);
	serv_addr.sin_port = htons(port);


	// Send connect
	int checkConnect = connect (sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
	if (checkConnect < 0) {
		perror("ERROR on connecting");
		return 1;
	}

	// Send hello message

	char helloMessage[] = "Hello Server!";
	nbytes = write(sockfd, helloMessage, sizeof(helloMessage));
	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}

	char receiveBuffer[1024];

	// Receive hello message

	nbytes = read(sockfd, (char*)&bufferLength, sizeof(bufferLength));
	if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}
	bufferLength = ntohl(bufferLength);
	nbytes = read(sockfd, receiveBuffer, bufferLength);
    	if (nbytes < 0) {
        	perror("ERROR on reading");
        	return 1;
    	}
    	printf("Server: %s\n", receiveBuffer);
	
	//Receive ask name message

	nbytes = read(sockfd, (char*)&bufferLength, sizeof(bufferLength));
	if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}
	bufferLength = ntohl(bufferLength);
	nbytes = read(sockfd, receiveBuffer, bufferLength);
    	if (nbytes < 0) {
        	perror("ERROR on reading");
        	return 1;
    	}
    	printf("Server: %s\n", receiveBuffer);

    // Send name

    	char name[1024];
	char clientName[1024 * 10];

	printf("Client: ");
    	scanf("%s", name);
	strcpy(clientName, "My name is ");
	strcat(strcat(clientName, name), "! Nice to meet you!");



	sendBuffer = htonl(sizeof(clientName));
	nbytes = write(sockfd, (char *)&sendBuffer, sizeof(sendBuffer));
	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}

	nbytes = write(sockfd, clientName, sizeof(clientName));
	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}


	// Send bye message
	char byeMessage[] = "Bye";

	sendBuffer = htonl(sizeof(byeMessage));
	nbytes = write(sockfd, (char *)&sendBuffer, sizeof(sendBuffer));
	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}

	nbytes = write(sockfd, byeMessage, sizeof(byeMessage));
	if (nbytes < 0) {
		perror("ERROR on write");
		return 1;
	}


	// Close connection
	close(sockfd);
	printf("Closed connection.\n");

	return 0;

}
