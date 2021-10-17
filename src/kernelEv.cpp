#include "kernelEv.h"
#include "dos.h"

extern IVTEntry* entryTable[256];

KernelEv::KernelEv(IVTNo ivtNo){
	lock
	val = 0;
	owner = (PCB*)running;
	ivtEntry = ivtNo;
	entryTable[ivtNo]->owner = this;
	entryTable[ivtNo]->oldRoutine = getvect(ivtNo);
	setvect(ivtNo, entryTable[ivtNo]->newRoutine);
	unlock
}

KernelEv::~KernelEv(){
	lock
	setvect(ivtEntry, entryTable[ivtEntry]->oldRoutine);
	unlock
}


void KernelEv::wait (){
	if(owner != running) return;
	if(val)
		val = 0;
	else{
		lock
		owner->blocked++;
		unlock
		dispatch();
	}
}

void KernelEv::signal(){
	if(!val && owner->blocked){
		lock
		owner->blocked--;
		Scheduler::put(owner);
		unlock
		dispatch();
	}
	else
		val = 1;
}
