#ifndef FRAME_H
#define FRAME_H
#include "myserver.h"

typedef struct
{
    int lost;
    int sequence;
    int acknowledgement;
    char data[DATA_SIZE];
} FRAME;

#define ACK 1
#define NACK -1
#define DATA 0
#define NOISE 3
#define FRAME_SIZE DATA_SIZE+3
#define SEQUENCE_BITS 3

void createFrame(int sequence, int type, char data[DATA_SIZE], FRAME* frame);

void printFrame(FRAME frame);

void noise(FRAME* frame);

#endif 
