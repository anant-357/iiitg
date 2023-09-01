#include "myserver.h"

typedef struct
{
    int sequence;
    int acknowledgement;
    char data[PACKET_SIZE];
} FRAME;

void frameToString(FRAME* frame, char* frame_str);

void stringToFrame(FRAME* frame, char frame_str[PACKET_SIZE+4]);

void printFrame(FRAME frame);


