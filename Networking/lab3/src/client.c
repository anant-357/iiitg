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
#include <pthread.h>

struct hostAndPort
{
    char host[15];
    int port;
} typedef hp;

int id;

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
        perror("Too many inputs!.Example Usage:\n\n\t./server 5050");
        HP.port = -1;
        strcpy(HP.host, "");
    }
    return HP;
}

void *read_thread(void *socket_void_ptr)
{
    int socket = *(int *)socket_void_ptr;
    while (1)
    {
        char message[MSG_SIZE + 1];

        bzero(message, sizeof(message));
        read(socket, message, sizeof(message));
        // printf("Message: %s", message);

        int dest;
        char get_message[MSG_SIZE + 1];
        sscanf(message, "%d %[^\n]", &dest, get_message);
        if (dest == id)
        {
            printf("accepted %s\n", get_message);
        }
        else
        {
            printf("rejected %s\n", get_message);
        }
    }
    return (void *)NULL;
}

void *write_thread(void *socket_void_ptr)
{
    int socket = *(int *)socket_void_ptr;
    int n = 0;
    char message[MSG_SIZE + 1];

    bzero(message, sizeof(message));
    printf("Please enter the message (<i> [message] )\n");
    while ((message[n++] = getchar()) != '\n')
    {
    }

    write(socket, message, sizeof(message));
    if (message[0] == '^')
    {
        return (void *)NULL;
    }
    return (void *)NULL;
}

void chat(int socket)
{
    pthread_t read;
    pthread_t write;
    pthread_create(&read, NULL, read_thread, &socket);
    pthread_create(&write, NULL, write_thread, &socket);

    pthread_join(read, NULL);
    pthread_join(write, NULL);
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
    printf("%d", client_fd);

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
    sscanf(message, "%d %[^\n]", &id, message);
    printf("id: %d\n", id);

    chat(client_fd);
    close(client_fd);

    return 0;
}
