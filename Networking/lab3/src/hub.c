#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <linux/net.h>
#include <pthread.h>

#include "myserver.h"

struct CQ
{
    int front;
    int rear;
    int clientQ[NODES];
} typedef CQ;

CQ cq;

int enqueue(CQ *cq, int client)
{
    if (cq->rear == NODES - 1)
        return -1;
    else
    {
        if (cq->front == -1)
            cq->front = 0;

        cq->rear = (cq->rear + 1) % NODES;

        cq->clientQ[cq->rear] = client;
        return 1;
    }
}

int empty(CQ *cq)
{
    if (cq->front == -1)
        return 1;
    return 0;
}

int front(CQ *cq)
{
    if (!empty(cq))
    {
        return cq->clientQ[cq->front];
    }
    return -1;
}

int full(CQ *cq)
{
    if ((cq->front == cq->rear + 1) || (cq->front == 0 && cq->rear == NODES - 1))
        return 1;
    return 0;
}

int dequeue(CQ *cq)
{
    if (cq->front == -1 || cq->front > cq->rear)
        return -1;
    else
    {

        int client = cq->clientQ[cq->front];
        if (cq->front == cq->rear)
        {
            cq->rear = -1;
            cq->front = -1;
        }
        cq->front = (cq->front + 1) % NODES;
        return client;
    }
}

void display(CQ *cq)
{
    for (int i = cq->front; i <= cq->rear; i++)
    {
        printf("%d %d %d\n", cq->front, cq->rear, cq->clientQ[cq->front]);
    }
}

void delete(CQ *cq, int client)
{
    CQ temporary;
    temporary.front = -1;
    temporary.rear = -1;
    int c = 1;

    while (cq->clientQ[cq->front] != client && !empty(cq))
    {
        enqueue(&temporary, cq->clientQ[cq->front]);
        dequeue(cq);
        c++;
    }

    if (empty(cq))
    {
        while (!empty(&temporary))
        {
            enqueue(cq, temporary.clientQ[temporary.front]);
            dequeue(&temporary);
        }
    }
    else
    {
        dequeue(cq);
        while (!empty(&temporary))
        {
            enqueue(cq, temporary.clientQ[temporary.front]);
            dequeue(&temporary);
        }
        int k = NODES - c - 1;
        while (k--)
        {
            int p = cq->clientQ[cq->front];
            dequeue(cq);
            enqueue(cq, p);
        }
    }
}

void broadcast(char *message)
{
    for (int i = cq.front; i <= cq.rear; i++)
    {
        printf("%ld", write(cq.clientQ[i], message, strlen(message) + 1));
    }
}

int do_hub_thing(int socket)
{
    while (1)
    {
        char message[MSG_SIZE + 1];
        bzero(message, sizeof(message));
        read(socket, message, sizeof(message));
        if (message[0] == '^')
            return -1;
        broadcast(message);
    }

    return 1;
}

void *client_thread(void *client_void_ptr)
{
    char *conn_established_msg = "Connected to Server\n";
    int client = *(int *)client_void_ptr;
    char client_str[200];
    sprintf(client_str, "%d", client);
    strcat(client_str, conn_established_msg);
    write(client, client_str, strlen(client_str));
    printf("Connection Established message sent to client-%d\n", client);
    enqueue(&cq, client);

    if (do_hub_thing(client) == -1)
    {
        printf("Disconnected from client\n");
        close(client);
        return (void *)0;
    }
    return (void *)0;
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

    if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind Failed\n");
        exit(0);
    }

    printf("Socket Bound...\n");

    if (listen(socket_fd, 2) < 0)
    {
        perror("Cannot Listen\n");
        exit(0);
    }
    printf("Listening on %d...\n", port);

    cq.front = -1;
    cq.rear = -1;
    while (1)
    {
        curr_socket = accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&address_length);
        if (curr_socket < 0)
        {
            printf("Cannot accept");
        }
        pthread_t client_thread_id;
        pthread_create(&client_thread_id, NULL, client_thread, &curr_socket);
    }

    shutdown(socket_fd, SHUT_RDWR);
    return 0;
}