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
#include <fcntl.h>
#include "myserver.h"

void getFile(int sockfd, char *file)
{
    int fd = open(file, O_CREAT | O_WRONLY, 0666);
    char buffer[PACKET_SIZE];
    int saved_stdout = dup(1);
    dup2(fd, 1);
    while (1)
    {
        if (recv(sockfd, buffer, PACKET_SIZE, 0) < 0)
        {
            perror("Unable to recieve from client! ananto\n");
            return;
        }
        printf(buffer);
        // if (fputs(buffer, fp) < 0)
        // {
        //     perror("Unable to write to file!");
        //     return;
        // }
        bzero(buffer, PACKET_SIZE);
        if (write(sockfd, ACKNOWLEDGEMENT, sizeof(ACKNOWLEDGEMENT)) < 0)
        {
            perror("Error while acknowledging");
        }
    }
    dup2(saved_stdout, 1);
    // fclose(fp);
    return;
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
    char *conn_established_msg = "Connected to Server\n";

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

    int *nclients = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *nclients = 0;
    fork();

    while (1)
    {
        curr_socket = accept(socket_fd, (struct sockaddr *)&address, (socklen_t *)&address_length);
        if (curr_socket < 0)
        {
            printf("Cannot accept");
        }
        printf("%d", *nclients);
        if (*nclients > 1)
        {
            write(curr_socket, "^", strlen("^"));
            close(curr_socket);
            continue;
        }
        *nclients = *nclients + 1;
        write(curr_socket, conn_established_msg, strlen(conn_established_msg));
        printf("Connection Established message sent\n");
        printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        getFile(curr_socket, "abc.txt");
        close(curr_socket);
        *nclients = *nclients - 1;
    }

    shutdown(socket_fd, SHUT_RDWR);
    return 0;
}