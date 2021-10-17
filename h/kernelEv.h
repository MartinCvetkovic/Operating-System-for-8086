// File: kernelEv.h
#ifndef _kernelEv_h_
#define _kernelEv_h_

#include "event.h"
#include "thread.h"
#include "IVTEntry.h"

class KernelEv {
public:
	char val;
	PCB* owner;
	IVTNo ivtEntry;

	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait ();
	void signal();
};
#endif
