#ifndef WINDOW_H
#define WINDOW_H

#include "frame.h"  // Include frame.h only when needed

typedef struct
{	
    int Sf;
	int Sn;
    int Ssize;
    int* slots;
	FRAME* frames;
} WINDOW;

#define UNSET -1
#define SENT 0
#define ACKNOWLEDGED 1


void createWindow(WINDOW* window, int size);

void markFrameSent(WINDOW* window, FRAME frame);

void markFrameAcknowledged(WINDOW* window, FRAME frame);

void printWindow(WINDOW window);

#endif

