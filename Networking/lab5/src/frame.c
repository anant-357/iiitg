#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "myserver.h"
#include "frame.h"

void createFrame(int sequence, int type, char data[DATA_SIZE], FRAME* frame){
   frame->sequence = sequence;
   frame->acknowledgement = type;
   frame->lost = 0;
   strcpy(frame->data, data);
}

void noise(FRAME *frame){
    srand(time(0));
    if(NOISE == -1 || rand()%NOISE == 0){
		frame->lost=1;
		printf("loosing frame\n");
	}
}

void printFrame(FRAME frame){
		printf("[%d]", frame.sequence);
		if(frame.acknowledgement == ACK){
				printf("Acknowledgement for %d : data-> %s\n", frame.sequence, frame.data);
		}else if(frame.acknowledgement == NACK){
				printf("Acknowledgement for %d\n", frame.sequence);
		}else if(frame.acknowledgement == DATA){
				printf("Data Frame->(%s)\n", frame.data);
		}else{
				printf("corrupt frame %d %d %s", frame.acknowledgement, frame.sequence, frame.data);
		}
}

