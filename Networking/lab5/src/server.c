#include <bits/time.h>
#include <bits/types/struct_itimerspec.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <linux/net.h>
#include <sys/mman.h>
#include "myserver.h"
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <poll.h>
#include <sys/timerfd.h>
#include <signal.h>

FRAME backup;
int sockfd;

void frameToString(FRAME *frame, char *frame_str) {
    char sequence_str[10]; // Adjust the size as needed
    char acknowledgement_str[10]; // Adjust the size as needed

    snprintf(sequence_str, sizeof(sequence_str), "%d", frame->sequence);
    snprintf(acknowledgement_str, sizeof(acknowledgement_str), "%d", frame->acknowledgement);

    strcpy(frame_str, sequence_str);
    strcat(frame_str, " ");
    strcat(frame_str, acknowledgement_str);
    strcat(frame_str, " ");
    strcat(frame_str, frame->data);
}

void stringToFrame(FRAME *frame, char frame_str[PACKET_SIZE + 4])
{
    sscanf(frame_str, "%d %d %[^\n]", &(frame->sequence), &(frame->acknowledgement), frame->data);
}

void printFrame(FRAME frame){
		printf("[%d]", frame.sequence);
		if(frame.acknowledgement == 1){
				printf("Acknowledgement\n");
		}else{
				printf("Data Frame->(%s)\n", frame.data);
		}
}

int sendFrame(int sequence,char* buffer){
		FRAME frame;
		char frame_str[PACKET_SIZE+4];
		frame.sequence = sequence;
		backup.sequence = sequence;
		strcpy(frame.data, buffer);
		strcpy(backup.data, buffer);
		frame.acknowledgement = 0;
		backup.acknowledgement = 0;
		frameToString(&frame, frame_str);
        if (write(sockfd, frame_str, strlen(frame_str)) == -1)
        {
            perror("Unable to send Data");
            return -1;
        }
		printFrame(frame);
		return 1;
}

void* timer_thread_function(void* void_fd) {
    int fd = *(int *)void_fd;
    uint64_t expirations;
    while (1) {
        struct pollfd poll_fd;
        poll_fd.fd = fd;
        poll_fd.events = POLLIN; 
        int poll_val = poll(&poll_fd, 1, -1); 

        if (poll_fd.revents & POLLIN) {
            if (read(fd, &expirations, sizeof(expirations))==-1){
                perror("read");
            } else {
                printf("Timer expired -> resending Frame\n");
				sendFrame(backup.sequence, backup.data);
            }
        } else{
            perror("poll");
        }
    }
}

int receiveAcknowledgement(char* buffer){
		FRAME frame;
		frame.sequence = -1;
		frame.acknowledgement = -1;
		strcpy(frame.data,"");
        if(read(sockfd, buffer, PACKET_SIZE+4)==-1)
        {
            perror("Error while acknowledging");
            return -1;
        }
		stringToFrame(&frame, buffer);
		printFrame(frame);
		return 1;
}

void sendFile(char *file)
{
    FILE *fp = fopen(file, "r");
    char buffer[PACKET_SIZE+4];
	int sequence = 0;

    while (fgets(buffer, PACKET_SIZE, fp) != NULL)
    {
		int timer_fd;
		struct itimerspec timer;
		pthread_t timer_thread_id;
		timer.it_value.tv_sec = 1;
		timer.it_value.tv_nsec = 0;
		timer.it_interval.tv_sec = 1;
		timer_fd = timerfd_create(CLOCK_REALTIME,0);
		timerfd_settime(timer_fd, 0, &timer, NULL);
		pthread_create(&timer_thread_id, NULL, &timer_thread_function, &timer_fd);
		
		sendFrame(sequence, buffer);
        bzero(buffer, PACKET_SIZE+4);
		printf("Sent\n");
		if(receiveAcknowledgement(buffer)==1)
				pthread_cancel(timer_thread_id);
        bzero(buffer, PACKET_SIZE+4);
		sequence++;
    }
}

int getPort(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Using default Port(8080)");
        return PORT;
    }
    else if (argc == 2)
    {
        int port = atoi(argv[1]);
        if (port == 0)
        {
            perror("Incorrect format of port.Example Usage:\n\n\t./server 5050");
            exit(0);
        }
        else
        {
            printf("Using port: %d", port);
        }
        return port;
    }
    else
    {
        perror("Too many inputs!.Example Usage:\n\n\t./server 5050");
        return -1;
    }
}

int main(int argc, char *argv[])
{
    int socket_fd, curr_socket;
    int port = getPort(argc, argv);
    int opt = 1;

    struct sockaddr_in address;
    int address_length = sizeof(address);

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Could not Make Socket\n");
        exit(0);
    }
    printf("Socket Made...\n");
    // Setting up options
    if ((setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))))
    {
        perror("Could not setup socket options\n");
        exit(0);
    }
    printf("Socket Options Set-Up...\n");

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Binding Socket

    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind Failed\n");
        exit(0);
    }

    printf("Socket Bound...\n");

    // Listening on Port

    if (listen(socket_fd, 0) < 0)
    {
        perror("Cannot Listen\n");
        exit(0);
    }
    printf("Listening on %d...\n", port);

    sockfd = accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&address_length);
    if (curr_socket < 0)
    {
        printf("Cannot accept");
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    sendFile("abc.txt");
    close(sockfd);

    shutdown(socket_fd, SHUT_RDWR);
    return 0;
}
