#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#define PORT 8080

using namespace std;

int main(int argc, char *argv[]){

	char *fileName = argv[1];

	struct sockaddr_in address;
	int sock = 0;
	struct sockaddr_in serv_addr;
	char buffer[1024] = {0};

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket creation error \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}

	send(sock, fileName, strlen(fileName), 0);
	printf("Name of required file sent\n");

	if(read(sock, buffer, 1024) == -1){
		printf("Error in getting size of file from server.\n");
		return -1;
	}

	int sizeOfFile = atoi(buffer);

	if(sizeOfFile == -1){
		printf("File not found or insufficient privileges to send file on server.\n");
		return -1;
	}

	int fileWrite = open(fileName, O_CREAT | O_TRUNC);

	if(fileWrite == -1){
		printf("Cannot open file on client.\n");
		return -1;
	}

	int sizeRead = 0;
	while(sizeRead < sizeOfFile){
		int nowRead = read(sock, buffer, 1024);
		if(nowRead == -1){
			printf("Error in getting file content from server.\n");
			return -1;		
		}

		int t = write(fileWrite, buffer, nowRead);
		if(t == -1 or t != nowRead){
			printf("Error in writing to file.\n");
			return -1;
		}

		sizeRead += nowRead;
	}

	close(fileWrite);

	return 0;
}
