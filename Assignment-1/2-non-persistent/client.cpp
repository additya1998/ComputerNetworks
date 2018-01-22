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

	int countOfFiles = argc - 1;
	if(countOfFiles < 0){
		cout << "Invalid input";
		return -1;
	}

	for(int c=0; c<countOfFiles; ++c){
		int error = 0;
		char fileName[4096 + 10];
		strcpy(fileName, argv[c + 1]);

		struct sockaddr_in address;
		int sock = 0;
		struct sockaddr_in serv_addr;
		char buffer[1024 + 10] = {0};

		if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
			printf("\n Socket creation error \n");
			return -1;
		}

		memset(&serv_addr, '0', sizeof(serv_addr));

		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);

		if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
			printf("\nInvalid address/ Address not supported \n");
			error = 1;
		}

		if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
			printf("\nConnection Failed \n");
			error = 1;
		}

		if(error){
			close(sock);
			continue;
		}

		cout << fileName;
		send(sock, fileName, strlen(fileName), 0);
		printf("Name of required file sent\n");

		int sizeOfFile = 0, sizeRead = 0;
		while(true){
			int t = read(sock, buffer, 1);
			if(t == -1){
				error = 1; break;
			}
			if(buffer[0] == '-'){
				read(sock, buffer, 2);
				error = 2; break;
			}
			else if(buffer[0] == '\n') break;
			else sizeOfFile = (sizeOfFile * 10) + buffer[0] - '0'; 
		}

		if(error == 1){
			cout << "Error in read.\n";
			close(sock);
			continue;
		}

		if(error == 2){
			cout << "File not found.\n";
			close(sock);
			continue;
		}

		int fileWrite = open(fileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if(fileWrite == -1){
			cout << "Cannot create file.";
			continue;
		}

		while(sizeRead < sizeOfFile){
			int nowRead = read(sock, buffer, min(1024, sizeOfFile - sizeRead));
			if(nowRead == -1){
				printf("Error in getting file content from server.\n");
				error = 1;
				break;
			}
			int nowWritten = write(fileWrite, buffer, nowRead);
			if(nowWritten != nowRead){
				printf("Error in writing to file.\n");
				error = 1;
				break;				
			}
			sizeRead += nowRead;
		}
		close(fileWrite);
		close(sock);
	}

	return 0;
}
