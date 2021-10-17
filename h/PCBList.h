// File: PCBList.h
#ifndef _PCBLIST_H_
#define _PCBLIST_H_

#include "PCB.h"

class elem{
	public:
		PCB* info;
		elem* next;
		elem(PCB* i){
			info = i;
			next = 0;
		}
};

class PCBList{
public:
	elem* head, * tail;

	PCBList();

	~PCBList();

	void add(PCB* data);
	void remove(PCB* data);
	PCB* pop();
	Thread* getById(ID id);
};

#endif
