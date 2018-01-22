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

	if(argc != 2){
		printf("Usage error \n");
		return -1;
	}

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

	int l = read(sock, buffer, 1024);

	if(l == -1){
		printf("Error in getting size of file from server.\n");
		return -1;
	}

	buffer[l] = '\0';

	if(buffer[0] == '-'){
		printf("%s", &buffer[3]);
		return -1;
	}

	int sizeOfFile = 0, done = 0, s = 0;

	for(int i=0; buffer[i] != '\0'; ++i){
		if(!done and buffer[i] == '\n') done = i;
		else if(done){
			buffer[i - done - 1] = buffer[i];
			s = i - done;
		}
		else sizeOfFile = (sizeOfFile * 10) + buffer[i] - '0';
	}

	buffer[s] = '\0';

	int fileWrite = open(fileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	int w = write(fileWrite, buffer, s);
	int sizeRead = 0;

	if(w == -1){
		printf("Error in writing to file.\n");
		return -1;
	}
	else sizeRead += w;
	
	if(fileWrite == -1){
		printf("Cannot open file on client.\n");
		return -1;
	}

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
	close(sock);

	return 0;
}
