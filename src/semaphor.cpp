#include "semaphor.h"
#include "krnlsem.h"
#include "thread.h"
#include "semlist.h"

extern volatile PCB* running;
extern SemList* semlist;

Semaphore::Semaphore (int init){
	lock
	myImpl = new KernelSem();
	myImpl->val = init;
	myImpl->blockedList = new PCBList();
	semlist->add(myImpl);
	unlock
}
Semaphore::~Semaphore (){
	lock
	semlist->remove(myImpl);
	delete myImpl->blockedList;
	delete myImpl;
	unlock
}

int Semaphore::wait (Time maxTimeToWait){
	lock
	// odavde pomeren maxtime
	if(--myImpl->val < 0){
		running->semTimeSlice = maxTimeToWait;
		running->blocked++;
		myImpl->blockedList->add((PCB*)running);
		unlock
		dispatch();
		lock
	}
	if(running->semTimeSlice == 0 && maxTimeToWait != 0) {
		unlock
		return 0;
	}
	else {
		unlock
		return 1;
	}
}
void Semaphore::signal(){
	lock
	if(++myImpl->val <= 0){
		PCB* pcb = myImpl->blockedList->pop();
		pcb->blocked--;
		pcb->semTimeSlice = 0;
		Scheduler::put(pcb);
	}
	unlock
}

int Semaphore::val () const{
	return myImpl->val;
}
