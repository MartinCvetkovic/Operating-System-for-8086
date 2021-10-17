#include "thread.h"
#include <dos.h>

static ID id = -2;

extern PCBList* pcblist;
extern volatile PCB* running;

extern volatile int requestedContextSwitch;
void interrupt timer();

Thread::Thread (StackSize stackSize, Time timeSlice){
	lock
	if(stackSize > 65535) stackSize = 65535;
	myPCB = new PCB();
	myPCB->id = ++id;
	stackSize /= sizeof(unsigned);
	myPCB->stack = new unsigned[stackSize];
	myPCB->stack[stackSize - 1] = 0x200;//1

	myPCB->stack[stackSize - 2] = FP_SEG(myPCB->wrapper);
	myPCB->stack[stackSize - 3] = FP_OFF(myPCB->wrapper);
	myPCB->sp = FP_OFF(myPCB->stack + stackSize - 12);
	myPCB->ss = FP_SEG(myPCB->stack + stackSize - 12);
	myPCB->bp = FP_OFF(myPCB->stack + stackSize - 12);

	myPCB->timeSlice = timeSlice;
	myPCB->myThread = this;

	pcblist->add(myPCB);
	unlock
}

void Thread::start(){
	if(myPCB->started) return;
	lock
	myPCB->started++;
	Scheduler::put(myPCB);
	unlock
}

void Thread::waitToComplete(){
	lock
	if(myPCB->finished || !myPCB->started) {
		unlock
		return;
	}
	myPCB->waitingThreads->add((PCB*)running);
	running->waiting++;
	unlock
	dispatch();
}

Thread::~Thread(){
	//if(!myPCB->finished) this->waitToComplete();
	lock
	pcblist->remove(myPCB);
	if(myPCB->stack)
		delete[] myPCB->stack;
	delete myPCB;
	unlock
}

ID Thread::getId(){
	return myPCB->id;
}

ID Thread::getRunningId(){
	return running->id;
}

Thread* Thread::getThreadById(ID id){
	return pcblist->getById(id);
}

void dispatch(){ // sinhrona promena konteksta
	lock
	requestedContextSwitch = 1;
	timer();
	unlock
}
