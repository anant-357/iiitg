#include <bits/time.h>
#include <linux/net.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include "frame.h"
#include "myserver.h"
#include <sys/timerfd.h>
#include <poll.h>
#include <fcntl.h>
#include "window.h"

int total_acks;
int lost_acks;

int Rn;
int window_size = 4;
char filename[20];

struct hostAndPort
{
    char host[15];
    int port;
} typedef hp;

int receiveFrame(int sockfd, char* buffer, char* file){
		FRAME frame;
        if (recv(sockfd, buffer, sizeof(FRAME), 0) < 0)
        {
            perror("Unable to recieve from client! \n");
            return -1;
        }
		memcpy(&frame, buffer, sizeof(FRAME));
		if(frame.acknowledgement==1)
				return 0;
		if(frame.sequence == -1)
				return -2;
		if(frame.lost == 1 || frame.sequence != Rn)
				return -1;
		printFrame(frame);
		if(frame.sequence == Rn){
				int file_fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0666);
				int out = dup(STDOUT_FILENO);
				dup2(file_fd, STDOUT_FILENO);
				printf("%s",frame.data);
				fflush(stdout);
				dup2(out, STDOUT_FILENO);
				close(file_fd);
				Rn = (Rn + 1)%window_size;
		}
		return frame.sequence;
}

int sendAcknowledgement(int sockfd, int sequence, char* buffer){
		FRAME frame;
		frame.acknowledgement = 1;
		strcpy(frame.data,"Piggy backing Data");
		noise(&frame);
		total_acks++;
		printf("%d", frame.lost);
		if(frame.lost==1)
				lost_acks++;
		memcpy(buffer, &frame, sizeof(FRAME));
		fflush(stdout);
		if (send(sockfd, buffer , sizeof(FRAME), 0) <= 0)
        {
            perror("Error while acknowledging");
			return -1;
        }
		printFrame(frame);
		return 1;

}

void getFile(int sockfd, char *file)
{
    char buffer[sizeof(FRAME)];
	Rn = 0;
    while (1)
    {
		int sequence = receiveFrame(sockfd, buffer, file);
		bzero(buffer, sizeof(FRAME));
		if(sequence>=0){
				sendAcknowledgement(sockfd, sequence, buffer);
		    	bzero(buffer, sizeof(FRAME));
		}else if (sequence == -2){
				close(sockfd);
				break;
		}
	}
    return;
}

hp getPortandHost(int argc, char *argv[])
{
    hp HP;

    if (argc == 1)
    {   strcpy(filename,"abc.txt");
        printf("Using localhost(127.0.0.1) and  default Port(8080)\n");
        HP.port = PORT;
        strcpy(HP.host, LOCALHOST);
    }
    else if (argc == 2)
    {   
        strcpy(HP.host, LOCALHOST);
		strcpy(filename, argv[1]);
        HP.port = PORT;
    }
    else if (argc == 3)
    {
		int port = atoi(argv[2]);
        if (port == 0)
        {
            perror("Incorrect format of port.Example Usage:\n\n\t./server 5050");
            exit(0);
        }
        else
        {
            printf("Using port: %d", port);
			HP.port = port;
        }
		strcpy(filename, argv[1]);
    }
    else if (argc == 4)
    {
		int port = atoi(argv[3]);
        if (port == 0)
        {
            perror("Incorrect format of port.Example Usage:\n\n\t./server 5050");
            exit(0);
        }
        strcpy(HP.host, argv[2]);
        strcpy(filename, argv[1]);
		HP.port = port;

    }else{
		perror("Too many inputs!.Example Usage:\n\n\t./client \"127.0.0.1\" 5050");
        HP.port = -1;
        strcpy(HP.host, "");
	}
    return HP;
}

int main(int argc, char *argv[])
{
    hp HP = getPortandHost(argc, argv);
    int status, client_fd;
    struct sockaddr_in serv_addr;
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(HP.port);

    if (inet_pton(AF_INET, HP.host, &serv_addr.sin_addr) <= 0)
    {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status = connect(client_fd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    int socket = client_fd;
    char buffer[2];
	recv(socket,buffer,sizeof("1"),0);
	if (strcmp(buffer,"1") == 0)
			getFile(socket, filename);

	printf("total acks: %d, lost acks: %d", total_acks, lost_acks);
    return 0;
}
