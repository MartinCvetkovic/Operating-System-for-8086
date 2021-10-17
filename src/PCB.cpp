#include "PCB.h"

PCB::PCB(){
	lock
	stack = 0;
	semTimeSlice = 0;
	finished = 0;
	started = 0;
	waiting = 0;
	blocked = 0;
	waitingThreads = new PCBList();
	unlock
}

void PCB::wrapper(){
		running->myThread->run();
		lock
		running->finished = 1;
		for(elem* cur = running->waitingThreads->head; cur != 0; cur = cur->next){
			cur->info->waiting--;
			Scheduler::put(cur->info);
		}
		unlock
		dispatch();
}

PCB::~PCB(){
	delete waitingThreads;
}
