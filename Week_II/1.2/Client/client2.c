/*
Ho_ten: Trinh Ngoc Huy
MSSV: 16022440

Content: Client

-	Client wan't download file from Server
	
*/

#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
int main(){

	

	int sockfd;
	int port = 1998;
	struct sockaddr_in serv_addr;

	char *serverIpAddr = (char *) malloc(100 * sizeof(char *));
	
	int nbytes;
	int size_Receive_Message;
	int size_Send_Message;
	char buffer[1024];

	time_t start_time;
	time_t end_time;

	struct timeval start, stop;
	double secs = 0;

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
	char helloMessage[] = "Hello Server! I want download file !";
	size_Send_Message = htonl(sizeof helloMessage);
	nbytes = write(sockfd,(char *)&size_Send_Message, sizeof(size_Send_Message));
	if(nbytes < 0){
		perror("ERROR on writing");
		return 1;
	}

	nbytes = write(sockfd, helloMessage, sizeof(helloMessage));
	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}

	// Receive hello message
	nbytes = read(sockfd, (char *)&size_Receive_Message, sizeof(size_Receive_Message));
	if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}
	size_Receive_Message = ntohl(size_Receive_Message);

	nbytes = read(sockfd, buffer, size_Receive_Message);
	if(nbytes < 0){
		perror("ERROR on reading");
		return 1;
	}
	printf("Server: %s\n", buffer);

	// Receive choose file message
	char choosefileMessage[1024];
	nbytes = read(sockfd, (char *)&size_Receive_Message,sizeof(size_Receive_Message));
	if(nbytes < 0){
		perror("ERROR on reading");
		return 1;
	}
	size_Receive_Message = ntohl(size_Receive_Message);
	nbytes = read(sockfd, choosefileMessage, size_Receive_Message);
	if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}
	printf("Server: %s\n", choosefileMessage);

	// Send file name
	char fileName[100];
	printf("Client: ");
	scanf("%s",fileName);
	size_Send_Message= htonl(sizeof(fileName));
	nbytes = write(sockfd, (char *)&size_Send_Message, sizeof(size_Send_Message));
	if(nbytes < 0){
		perror("ERROR on writing");
		return 1;
	}

	nbytes = write(sockfd, fileName, sizeof(fileName));
	if (nbytes < 0) {
		perror("ERROR on writing");
		return 1;
	}

	// // Check file status
	int fileStatus;
	nbytes = read(sockfd, &fileStatus, sizeof(int));
	if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}
	
	if(fileStatus == 0){
		printf("Server: Oops - File doesn't exist \n");
		close(sockfd);
		printf("Closed connection.\n");
		return 0;
	}
	else {
	printf("Server: File will download in a few second...\n");
	// Receive file size
	unsigned int fileSize = 0;
	char receiveBuffer[1024];
    unsigned int receivedSize = 0;

    nbytes = read(sockfd, &fileSize, sizeof(fileSize));
    if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}

	// Get time start receive
	time(&start_time);
	gettimeofday(&start, NULL);

	// Receive file
	FILE *freceive = fopen(fileName, "wb");
	while (receivedSize < fileSize) {
		int currentReceivedSize = read(sockfd, receiveBuffer, 1024);
		receivedSize += currentReceivedSize;
		fwrite(receiveBuffer, 1, currentReceivedSize, freceive);
    }

    // Get time finish receive
    time(&end_time);
	gettimeofday(&stop, NULL);
	secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
    printf("Client: Receive Successfully! \n");
	}
	printf("\n");
	printf(" ========================= TIME TABLE ==========================\n");
	printf(" ===============================================================\n");
	printf(" TIME RECEIVE START 	--->   %s", (char*)(ctime(&start_time)));
	printf(" ===============================================================\n");
	printf(" TIME RECEIVE FINISH 	--->   %s", (char*)(ctime(&start_time)));
	printf(" ===============================================================\n");
	printf(" TOTAL RECEIVE TIME 	--->   %f (s) \n", secs);
	printf(" ===============================================================\n");
	

	// Close connection

	close(sockfd);
	printf("Closed connection.\n");

	return 0;
}