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

char filename[20];
int sockfd;

int total_frames_sent;
int ack_received;

pthread_t timer_thread_id;
pthread_t acknowledgement_thread_id;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
WINDOW window;

int Rn;
int window_size =4;


int sendFrame(FRAME* frame, int first_time){
		char frame_str[sizeof(FRAME)];
		memcpy(frame_str, frame, sizeof(FRAME));
		total_frames_sent++;
        if (send(sockfd, frame_str, sizeof(FRAME), 0) == -1)
        {
		print();
            perror("Unable to send Data");
            return -1;
        }
		if(first_time == 1){
				pthread_mutex_lock(&lock);
				markFrameSent(&window,*frame);
				pthread_mutex_unlock(&lock);
				printf("Sent frame: %d\n", frame->sequence);
		}
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
		ack_received++;
		pthread_mutex_lock(&lock);
		if(window.slots[frame.sequence]!=SENT){
				return 0;
		}
		markFrameAcknowledged(&window, frame);
		pthread_mutex_unlock(&lock);
		printf("Received Acknowledgement: %d\n", frame.sequence);
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
                printf("Timer expired -> resending Frame(s)\n");
				sendFrame(&(window.frames[window.Sf]), 0);
				printf("Resent frame: %d\n", window.Sf);
				for(int slot = window.Sf + 1; slot != window.Sn; slot = (slot + 1)%window.Ssize){
						sendFrame(&(window.frames[slot]), 0);
						printf("Resent frame: %d\n", slot);
				}            
			}
        } else{
            perror("poll");
        }
    }
}

void* acknowledgement_thread_function(){
    while(1){
		if(receiveAcknowledgement()==1){
				printf("Sending ACK for Piggy Backing Data\n");
				printWindow(window);
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
    FILE *fp = fopen(filename, "r");
    char buffer[DATA_SIZE];
	int sequence = 0;
	int finished = 0;
    while (1){
		while(sequence == window.Sn && window.slots[sequence]!=SENT){
				if (fgets(buffer, DATA_SIZE, fp)!=NULL){
						FRAME frame;
						createFrame(sequence, DATA, buffer, &frame);
						sendFrame(&frame, 1);
						printWindow(window);
						sequence = (sequence + 1)%window.Ssize;
						bzero(buffer, DATA_SIZE);
				}else{
						finished = 1;
						break;
				}	
		}
		if (finished){
				FRAME frame;
				strcpy(buffer, "END");
				createFrame(-1, DATA, buffer, &frame);
				sendFrame(&frame, 1);
				pthread_cancel(timer_thread_id);
				pthread_cancel(acknowledgement_thread_id);
				break;
		}
    }
    return (void*)NULL;
}

void sendFile()
{
    pthread_mutex_init(&lock, NULL);

    FRAME frame;
	char buffer[sizeof(FRAME)];
	frame.sequence = window.Ssize;
	strcpy(frame.data, "Window Size");
	memcpy(buffer, &frame, sizeof(FRAME));
	send(sockfd, buffer, sizeof(FRAME), 0);

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
}

struct hostAndPort
{
    char host[15];
    int port;
} typedef hp;

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
    int status;
    struct sockaddr_in serv_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
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

    if ((status = connect(sockfd, (struct sockaddr *)&serv_addr,
                          sizeof(serv_addr))) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    createWindow(&window,4);
	char buffer[2];
	recv(sockfd,buffer,sizeof("1"),0);
	if (strcmp(buffer,"1") == 0)
			sendFile();
    close(sockfd);
		
	printf("frames sent: %d, ack received: %d, lost frames: %d",total_frames_sent, ack_received, (total_frames_sent-ack_received));
    printf("efficiency: %.2f", (float)ack_received*100/(float)total_frames_sent);
	return 0;
}
