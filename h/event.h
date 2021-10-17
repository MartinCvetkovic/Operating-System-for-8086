// File: event.h

#include "IVTEntry.h";

#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;
class KernelEv;
class IVTEntry;

#define PREPAREENTRY(entryNo, callOldFlag) \
	void interrupt inter##entryNo(...);\
	IVTEntry newEntry##entryNo(entryNo, inter##entryNo);\
	void interrupt inter##entryNo(...) {\
		newEntry##entryNo.signal();\
		if (callOldFlag) newEntry##entryNo.callOldRoutine();\
	}


class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();

	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};
#endif


