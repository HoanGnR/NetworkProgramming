/*
Ho_ten: Trinh Ngoc Huy
MSSV: 16022440

Content: Server

-	Server send file to Client
	
*/
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>

int main() {


	int port = 1998;
	int sockfd, newsockfd;
	struct sockaddr_in serv_addr, cli_addr;
	int option = 1;
	int nbytes;
	int bufferLength;
	char fileName[100];

	time_t start_time;
	time_t end_time;

  	struct timeval start, stop;
	double secs = 0;

	printf("Set size of buffer: ");
	scanf("%d",&bufferLength);

	char buffer[bufferLength];
	
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
	printf("Waiting for request from Client... \n");

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
	char choosefileMessage[] = "Please choose a file name you want download: ";
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
	nbytes = read(newsockfd, fileName, size_Receive_Message);
    if (nbytes < 0) {
		perror("Read error");
		return 1;
	}

	// Check file exist
	int fileExist;
	if(access(fileName,0) == -1){
		fileExist = 0;
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
		// send status file
		fileExist = 1;
		nbytes = write(newsockfd, &fileExist, sizeof(int));
			if (nbytes < 0) {
				perror("ERROR on writing");
				return 1;
			}

		// Send file size
		int size;
		FILE *fsend = fopen(fileName,"rb");
		fseek(fsend, 0, SEEK_END);
    	size = ftell(fsend);
    	fseek(fsend, 0, SEEK_SET);
    	nbytes = write(newsockfd,(void *) &size, sizeof(int));
		if(nbytes < 0){
			perror("ERROR on writing");
			return 1;
		}

		// Get time start send
		time( &start_time );
		gettimeofday(&start, NULL);

		while (!feof(fsend)) {
		    int readSize = fread(buffer, 1, sizeof(buffer) - 1, fsend);
		    write(newsockfd, buffer, readSize);
		    bzero(buffer, sizeof(buffer));
		}
    	printf("Server: Send Successfully!\n");
	}

	// Get time finish send
	time (&end_time );
	gettimeofday(&stop, NULL);
	secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);

	printf("\n");
	printf(" ======================= TIME TABLE ========================\n");
	printf(" ===========================================================\n");
	printf(" TIME SEND START 	--->   %s", (char *)(ctime(&start_time)));
	printf(" ===========================================================\n");
	printf(" TIME SEND FINISH 	--->   %s", (char *)(ctime(&start_time)));
	printf(" ===========================================================\n");
	printf(" TOTAL SEND TIME 	--->   %f (s) \n", secs);
	printf(" ===========================================================\n");
	

	close(newsockfd);
	close(sockfd);
	printf("Closed connection.\n");

	return 0;
}