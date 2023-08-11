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

void getMan(char *command)
{
    char man_command[MSG_SIZE] = "whatis ";
    strcat(man_command, command);
    FILE *fp;
    char output[MSG_SIZE];

    fp = popen(man_command, "r");
    if (fp != NULL)
    {

        fgets(output, sizeof(output), fp);
        strcpy(command, output);
        if (pclose(fp) != 0)
        {
            strcpy(command, "No such command or man page for such command.");
        }
    }
    else
    {
        printf("Unable to run command\n");
    }
}

int chat(int socket)
{
    while (1)
    {
        char message[MSG_SIZE];
        bzero(message, sizeof(message));
        int message_flag = read(socket, message, sizeof(message));
        if (message[0] == '^')
        {
            return -1;
        }
        if (message_flag > 1)
        {
            printf("Client: %s\n", message);

            getMan(message);
            write(socket, message, sizeof(message));
        }
    }
    return 1;
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
        printf("Accepted...\n");
        if (*nclients >= 1)
        {
            write(curr_socket, "^", strlen("^"));
            close(curr_socket);
            continue;
        }
        *nclients = *nclients + 1;
        write(curr_socket, conn_established_msg, strlen(conn_established_msg));
        printf("Connection Established message sent\n");
        printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
        if (chat(curr_socket) == -1)
        {
            printf("Disconnected from client\n");
            close(curr_socket);
            *nclients = *nclients - 1;
        }
    }

    shutdown(socket_fd, SHUT_RDWR);
    return 0;
}