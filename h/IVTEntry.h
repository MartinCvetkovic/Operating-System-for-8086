// File: IVTEntry.h
#ifndef _IVTEntry_h_
#define _IVTEntry_h_

#include "kernelEv.h"
#include "event.h"

typedef void interrupt(*interruptRoutine)(...);

class IVTEntry {
public:
	IVTNo ivtNo;
	KernelEv* owner;
	interruptRoutine oldRoutine;
	interruptRoutine newRoutine;

	IVTEntry(IVTNo ivtNumber, interruptRoutine newRoutine);
	~IVTEntry();

	void callOldRoutine();
	void signal();
};
#endif
