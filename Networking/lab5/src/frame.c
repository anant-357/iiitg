#include <stdio.h>
#include <string.h>
#include "myserver.h"
#include "frame.h"

void frameToString(FRAME *frame, char *frame_str) {
    char sequence_str[10]; // Adjust the size as needed
    char acknowledgement_str[10]; // Adjust the size as needed

    snprintf(sequence_str, sizeof(sequence_str), "%d", frame->sequence);
    snprintf(acknowledgement_str, sizeof(acknowledgement_str), "%d", frame->acknowledgement);

    strcpy(frame_str, sequence_str);
    strcat(frame_str, " ");
    strcat(frame_str, acknowledgement_str);
    strcat(frame_str, " ");
    strcat(frame_str, frame->data);
}

void stringToFrame(FRAME *frame, char frame_str[PACKET_SIZE + 4])
{
    sscanf(frame_str, "%d %d %[^\n]", &(frame->sequence), &(frame->acknowledgement), frame->data);
}

void printFrame(FRAME frame){
		printf("[%d]", frame.sequence);
		if(frame.acknowledgement == 1){
				printf("Acknowledgement\n");
		}else{
				printf("Data Frame->(%s)\n", frame.data);
		}
}

