#include <linux/net.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "myserver.h"

struct hostAndPort
{
    char host[15];
    int port;
} typedef hp;

void sendFile(int sockfd, char *file)
{
    FILE *fp = fopen(file, "r");
    char buffer[PACKET_SIZE];
    char acknowledgment[sizeof(ACKNOWLEDGEMENT)];

    while (fgets(buffer, PACKET_SIZE, fp) != NULL)
    {
        if (send(sockfd, buffer, sizeof(buffer), 0) == -1)
        {
            perror("Unable to send Data");
            return;
        }
        bzero(buffer, PACKET_SIZE);
        if (read(sockfd, acknowledgment, sizeof(acknowledgment)) && strcmp(acknowledgment, ACKNOWLEDGEMENT) < 0)
        {
            perror("Error while acknowledging");
            return;
        }
        printf(acknowledgment);
    }
}

hp getPortandHost(int argc, char *argv[])
{
    hp HP;

    if (argc == 1)
    {
        printf("Using localhost(127.0.0.1) and  default Port(8080)\n");
        strcpy(HP.host, LOCALHOST);
        HP.port = PORT;
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
    }
    else if (argc == 3)
    {
        int port = atoi(argv[2]);
        if (port == 0)
        {
            perror("Incorrect format of port.Example Usage:\n\n\t./server 5050");
            exit(0);
        }
        strcpy(HP.host, argv[1]);
        HP.port = port;
    }
    else
    {
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

    char message[MSG_SIZE + 1];

    bzero(message, sizeof(message));
    read(socket, message, sizeof(message));
    if (message[0] == '^')
    {
        printf("Line Busy!");
        return -1;
    }
    else
    {
        sendFile(socket, "alice.txt");
    }
    return 0;
}