// File: semlist.h
#ifndef _semlist_H_
#define _semlist_H_

#include "krnlsem.h"

class KernelSem;

class elemS{
public:
	KernelSem* info;
	elemS* next;
	elemS(KernelSem* i){
		info = i;
		next = 0;
	}
};

class SemList{
public:
	elemS* head, * tail;

	SemList();

	~SemList();

	void add(KernelSem* data);
	void remove(KernelSem* data);
	KernelSem* pop();
};

#endif
