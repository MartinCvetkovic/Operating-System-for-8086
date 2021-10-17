// File: krnlsem.h
#ifndef _krnlsem_h_
#define _krnlsem_h_

#include "semaphor.h"
#include "PCBList.h"

class KernelSem {
public:
	int val;
	PCBList* blockedList;
};
#endif
