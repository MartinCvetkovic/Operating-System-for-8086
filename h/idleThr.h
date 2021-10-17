// File: idleThr.h
#ifndef _idleThr_h_
#define _idleThr_h_

#include "thread.h"

class IdleThread: public Thread {
	virtual void run() {
		while(1);
	}
public:
	IdleThread():Thread(1024, 1) {}
	PCB* getIdlePCB(){
		return getPCB();
	}
	void init(){
		getPCB()->started++;
		getPCB()->blocked++;
	}
};

#endif
