#include "window.h"
#include "frame.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<unistd.h>

void createWindow(WINDOW* window, int size){
    int max = (int)pow(2,SEQUENCE_BITS)-1;
    if(size > max){
		printf("size greater than max val:%d",max);
		window->Ssize = max;
	}else{
		window->Ssize = size;
	}
    window->Sf=-1;
	window->Sn=0;
	window->slots = malloc(sizeof(int)*(window->Ssize));
	window->frames = malloc(sizeof(FRAME)*(window->Ssize));
	for(int i=0;i<(window->Ssize+1);i++){
		window->slots[i] = UNSET;
	}
}

void printWindow(WINDOW window){
    printf("Window {Sf = %d, Sn = %d} : [", window.Sf, window.Sn);
    int slot_index = window.Sf == -1 ? 0 : window.Sf;
    printf("(%d|%d)",slot_index,window.slots[slot_index]);
	slot_index=(slot_index+1)%window.Ssize;
	while(slot_index != (window.Sf + window.Ssize) % window.Ssize){
		printf(" (%d|%d)",slot_index,window.slots[slot_index]);
		slot_index = (slot_index + 1) % window.Ssize;
	}
	printf("]\n");
}

void markFrameSent(WINDOW* window, FRAME frame){
    if(window->Sf == -1){
		window->Sf = 0;
	}
	window->slots[frame.sequence] = SENT;
	memcpy(&(window->frames[frame.sequence]),&frame,sizeof(FRAME));
    window->Sn = (window->Sn + 1) % window->Ssize;
}

void markFrameAcknowledged(WINDOW* window, FRAME frame){
    for(int slot_index=window->Sf; slot_index != frame.sequence; slot_index=(slot_index+1)%window->Ssize){
			window->slots[slot_index] = ACKNOWLEDGED;
		}
    window->slots[frame.sequence] = ACKNOWLEDGED;
    window->Sf = (frame.sequence + 1)%window->Ssize;
}


