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
#include "window.h"
#include "frame.h"
#include <math.h>

FRAME* backup;
int backup_length;
int backup_start;
int backup_next;
int sockfd;

pthread_t timer_thread_id;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

WINDOW window;

void addFrameToBackup(FRAME frame){
		backup[backup_next].sequence = frame.sequence;
		backup[backup_next].acknowledgement = frame.acknowledgement;
		backup[backup_next].lost = frame.lost;
		strcpy(backup[backup_next].data ,frame.data);
		backup_next = (backup_next + 1)%backup_length;
		if(backup_next == backup_start){
				backup_start = (backup_next+1)%backup_length;
		}
}

int sendFrame(FRAME* frame){
		char frame_str[sizeof(FRAME)];
		addFrameToBackup(*frame);
		memcpy(frame_str, frame, sizeof(FRAME));
		printf("%s\n", frame_str);
        if (send(sockfd, frame_str, sizeof(FRAME), 0) == -1)
        {
            perror("Unable to send Data");
            return -1;
        }
		pthread_mutex_lock(&lock);
		markFrameSent(&window,*frame);
		pthread_mutex_unlock(&lock);
		printFrame(*frame);
		printWindow(window);
		return 1;
}

int receiveAcknowledgement(){
		char buffer[sizeof(FRAME)];
		FRAME frame;
		frame.sequence = -1;
		frame.acknowledgement = -1;
		strcpy(frame.data,"");
        if(recv(sockfd, buffer, sizeof(FRAME), 0)==-1)
        {
            perror("Error while acknowledging");
            return -1;
        }
		memcpy(&frame, buffer, sizeof(FRAME));
		if(frame.lost==1){
				return 0;
		}
		pthread_mutex_lock(&lock);
		markFrameAcknowledged(&window, frame);
		printFrame(frame);
		printWindow(window);
		pthread_mutex_unlock(&lock);
		return 1;
}

void* timer_thread_function(void* void_fd) {
    int fd = *(int *)void_fd;
    uint64_t expirations;
	while (1) {
        struct pollfd poll_fd;
        poll_fd.fd = fd;
        poll_fd.events = POLLIN;
		if (poll(&poll_fd, 1, -1) == -1) {
            perror("poll");
            exit(EXIT_FAILURE);
        }
        if (poll_fd.revents & POLLIN) {
            if (read(fd, &expirations, sizeof(expirations))==-1){
                perror("read");
            } else {
                printf("Timer expired -> resending Frame\n");
				for(int i = backup_start; i!=backup_next; i = (i + 1)%backup_length)
						sendFrame(&(backup[i]));
            }
        } else{
            perror("poll");
        }
    }
}

void* acknowledgement_thread_function(){
    while(1){
		int r = receiveAcknowledgement();
		if(r==1){
				pthread_cancel(timer_thread_id);
				int timer_fd;
				timer_fd = timerfd_create(CLOCK_MONOTONIC,0);
				if (timer_fd == -1) {
				    perror("timerfd_create");
						exit(EXIT_FAILURE);
				}
				struct itimerspec new_timer;
				new_timer.it_value.tv_sec = TIMEOUT;
				new_timer.it_value.tv_nsec = 0;
				new_timer.it_interval.tv_sec = TIMEOUT;
				new_timer.it_interval.tv_nsec = 0;
				if (timerfd_settime(timer_fd, 0, &new_timer, NULL) == -1) {
						perror("timerfd_settime");
						exit(EXIT_FAILURE);
				}
				pthread_create(&timer_thread_id, NULL, &timer_thread_function, &timer_fd);
		}
	}
		return (void*)NULL;
}

void* frame_thread_function(){
    char file[10] = "abc.txt";
    FILE *fp = fopen(file, "r");
    char buffer[DATA_SIZE];
	int sequence = 0;
    while (1){
		while(sequence >= window.Sn && sequence < window.Ssize + window.Sf && window.slots[sequence]!=SENT){
				if (fgets(buffer, DATA_SIZE, fp)!=NULL){
						FRAME frame;
						createFrame(sequence, DATA, buffer, &frame);
						sendFrame(&frame);
						sequence=(sequence + 1) % window.Ssize;
						bzero(buffer, DATA_SIZE);
				}else{
						break;
				}	
		}
    }
    return (void*)NULL;
}

void sendFile()
{
    pthread_mutex_init(&lock, NULL);
	printWindow(window);
	pthread_t acknowledgement_thread_id;
	pthread_t frame_thread_id;
	int timer_fd;
	struct itimerspec timer;
	timer.it_value.tv_sec = TIMEOUT;
	timer.it_value.tv_nsec = 0;
	timer.it_interval.tv_sec = TIMEOUT;
    timer.it_interval.tv_nsec = 0;
	timer_fd = timerfd_create(CLOCK_MONOTONIC,0);
	timerfd_settime(timer_fd, 0, &timer, NULL);
	pthread_create(&timer_thread_id, NULL, &timer_thread_function, &timer_fd);
	pthread_create(&frame_thread_id, NULL, &frame_thread_function, NULL);
	pthread_create(&acknowledgement_thread_id, NULL, &acknowledgement_thread_function, NULL);		
	pthread_join(frame_thread_id, NULL);
	pthread_join(acknowledgement_thread_id, NULL);
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
    int socket_fd;
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
    while(1){
			fork();
    sockfd = accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&address_length);
	
	if (sockfd < 0)
    {
        printf("Cannot accept");
    }
    printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    createWindow(&window,7);
    backup_length = window.Ssize;
	backup = malloc(sizeof(FRAME)*backup_length);
	backup_start = 0;
	backup_next = 0;
	sendFile();
	}
    close(sockfd);

    shutdown(socket_fd, SHUT_RDWR);
    return 0;
}
