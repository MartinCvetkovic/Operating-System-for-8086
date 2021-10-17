#include "IVTEntry.h"
#include <dos.h>

extern IVTEntry* entryTable[256];

IVTEntry::IVTEntry(IVTNo ivtNumber, interruptRoutine newRoutine){
	lock
	ivtNo = ivtNumber;
	//oldRoutine = getvect(ivtNumber);
	//setvect(ivtNumber, newRoutine);
	this->newRoutine = newRoutine;

	entryTable[ivtNo] = this;
	unlock
}

IVTEntry::~IVTEntry(){
	//lock
	//setvect(ivtNo, oldRoutine);
	//unlock
}

void IVTEntry::callOldRoutine(){
	oldRoutine();
}

void IVTEntry::signal(){
	if(owner)
		owner->signal();
}
