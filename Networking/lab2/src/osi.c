#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define APPLICATIONHEADER "[Application Header]-> "
#define PRESENTATIONHEADER "[Presentation Header]-> "
#define SESSIONHEADER "[Session Header]-> "
#define TRANSPORTHEADER "[Transport Header]-> "
#define NETWORKHEADER "[Network Header]-> "
#define DATALINKHEADER "[Data Link Header]-> "
#define PHYSICALHEADER "[Physical Header]-> "

const char *physicalLayer(char *bits, int size)
{
    char *header = malloc((strlen(PHYSICALHEADER) + size) * sizeof(char));
    header = strcat(header, PHYSICALHEADER);
    header = strcat(header, bits);
    // printf("%s/n", header);
    return header;
}

const char *dataLinkLayer(char *frame, int size)
{
    char *header = malloc((strlen(DATALINKHEADER) + size) * sizeof(char));

    header = strcat(header, DATALINKHEADER);
    header = strcat(header, frame);
    printf("%s\n", header);
    return physicalLayer(header, strlen(header));
}

const char *networkLayer(char *packet, int size)
{
    char *header = malloc((strlen(NETWORKHEADER) + size) * sizeof(char));

    header = strcat(header, NETWORKHEADER);
    header = strcat(header, packet);
    printf("%s\n", header);
    return dataLinkLayer(header, strlen(header));
}

const char *transportLayer(char *segment, int size)
{
    char *header = malloc((strlen(TRANSPORTHEADER) + size) * sizeof(char));

    header = strcat(header, TRANSPORTHEADER);
    header = strcat(header, segment);
    printf("%s\n", header);
    return networkLayer(header, strlen(header));
}

const char *sessionLayer(char *message, int size)
{
    char *header = malloc((strlen(SESSIONHEADER) + size) * sizeof(char));

    header = strcat(header, SESSIONHEADER);
    header = strcat(header, message);
    printf("%s\n", header);
    return transportLayer(header, strlen(header));
}

const char *presentationLayer(char *message, int size)
{

    char *header = malloc((strlen(PRESENTATIONHEADER) + size) * sizeof(char));

    header = strcat(header, PRESENTATIONHEADER);
    header = strcat(header, message);
    printf("%s\n", header);
    return sessionLayer(header, strlen(header));
}

const char *applicationLayer(char *message, int size)
{
    char *header = malloc((strlen(APPLICATIONHEADER) + size) * sizeof(char));

    header = strcat(header, APPLICATIONHEADER);
    header = strcat(header, message);
    printf("%s\n", header);
    return presentationLayer(header, strlen(header));
}

int main()
{
    char *buffer = "message";
    printf("%s", applicationLayer(buffer, strlen(buffer)));
    return 0;
}