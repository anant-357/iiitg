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

int sockfd;

void communicate(int sender, int receiver){
		int child = fork();
		if (child == 0){
				while(1){
						char buffer[sizeof(FRAME)];
						if (recv(sender, buffer, sizeof(FRAME),0) < 0){
								perror("Unable to receive from sender");
						}
						if (send(receiver, buffer, sizeof(FRAME), 0) < 0){
								perror("Unable to send to receiver");
						}
				}
		}else{
				while(1){
						char buffer[sizeof(FRAME)];
						if (recv(receiver, buffer, sizeof(FRAME),0) < 0){
								perror("Unable to receive from receiver");
						}
						if (send(sender, buffer, sizeof(FRAME), 0) < 0){
								perror("Unable to send to sender");
						}
				}
				
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
				printf("Waiting for sender...");
				int senderfd = accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&address_length);
				if (senderfd < 0)
				{
						printf("Cannot accept");
				}
				printf("Sender is here!!");
				printf("Waiting for receiver");
				int receiverfd = accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&address_length);	
				if (receiverfd < 0)
				{
						printf("Cannot accept");
				}
				printf("Receiver is here!!");
			if(fork() == 0){
				send(senderfd, "1", sizeof("1"), 0);
				send(receiverfd, "1", sizeof("1"), 0);
				communicate(senderfd, receiverfd);
				
				close(senderfd);
				close(receiverfd);
			}
	}
	

    shutdown(socket_fd, SHUT_RDWR);
    return 0;
}
