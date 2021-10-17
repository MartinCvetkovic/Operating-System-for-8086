// File: PCB.h
#include "thread.h"
#include "PCBList.h"

#ifndef _PCB_H_
#define _PCB_H_

typedef unsigned int Time; // time, x 55ms
typedef int ID;

class Thread;
class PCBList;
void dispatch();
extern volatile PCB* running;

class PCB{
public:
	unsigned bp;
	unsigned sp;
	unsigned ss;
	unsigned* stack;
	char finished;
	char started;
	char waiting;
	char blocked;
	Time semTimeSlice;
	Time timeSlice;
	ID id;
	Thread* myThread;
	PCBList* waitingThreads;
	static void wrapper();
	PCB();
	~PCB();
};

#endif
