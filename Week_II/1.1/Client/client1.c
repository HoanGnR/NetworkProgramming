/*
Ho_ten: Trinh Ngoc Huy
MSSV: 16022440

Content: Client

-	Client upload file to Server
	
*/

#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>
#include<strings.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

int main(){

	int sockfd;
	int port = 1998;
	struct sockaddr_in serv_addr;

	char *serverIpAddr = (char *) malloc(100 * sizeof(char *));
	
	int nbytes;
	int size_Receive_Message;
	int size_Send_Message;
	char buffer[1024];


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
	char helloMessage[] = "Hello Server! I want upload file !";
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


	// Insert file
	char fileName[100];
	printf("Client: ");
	scanf("%s",fileName);

	//Open file
	FILE *fsend =fopen(fileName,"rb");

	while(fsend == NULL){
		printf("File doesn't exist. Please rechoose: ");
		scanf("%s",fileName);
		fsend=fopen(fileName,"rb");
	}

	// Send file name
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

	// Check file status
	int fileStatus;
	nbytes = read(sockfd, &fileStatus, sizeof(int));
	if (nbytes < 0) {
		perror("ERROR on reading");
		return 1;
	}
	
	if(fileStatus == 1){
		printf("Server: Oops - File already exist \n");
		close(sockfd);
		printf("Closed connection.\n");
		return 0;
	}

	//Send file size. 
	int size;
	fseek(fsend, 0, SEEK_END);
    size = ftell(fsend);
    fseek(fsend, 0, SEEK_SET);
	nbytes = write(sockfd,(void *) &size, sizeof(int));
	if(nbytes < 0){
		perror("ERROR on writing");
		return 1;
	}

	// Read file and send
	while (!feof(fsend)) {
	    int readSize = fread(buffer, 1, sizeof(buffer) - 1, fsend);
	    write(sockfd, buffer, readSize);
	    bzero(buffer, sizeof(buffer));
	}
    printf("Client: Sent Successfully!\n");

	// Close connection
	close(sockfd);
	printf("Closed connection.\n");

	return 0;
}