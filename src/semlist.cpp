#include "semlist.h"
#include "thread.h"

SemList::SemList(){
		//lock
		head = 0;
		tail = 0;
		//unlock
	}

SemList::~SemList(){
		//lock
		elemS* prev = 0;
		for(elemS* cur = head; cur != 0; cur = cur->next){
			if(prev){
				delete prev;
			}
			prev = cur;
		}
		if(prev) delete prev;
		//unlock
	}

void SemList::add(KernelSem* data){
	//lock
	if(!head){
		head = new elemS(data);
		tail = head;
	}
	else{
		tail->next = new elemS(data);
		tail = tail->next;
	}
	//unlock
}

void SemList::remove(KernelSem* data){
	if(!head) return;
	//lock
	elemS* prev = 0;
	for(elemS* cur = head; cur != 0; cur = cur->next){
		if(cur->info == data){
			if(!prev){
				head = cur->next;
				if(!head) tail = 0;
				delete cur;
			}
			else{
				prev->next = cur->next;
				if(!cur->next) tail = prev;
				delete cur;
			}
			//unlock
			return;
		}
		prev = cur;
	}
	//unlock
}

KernelSem* SemList::pop(){
	if(!head) return 0;
	//lock
	KernelSem* info = head->info;
	elemS* prev = head;
	head = head->next;
	delete prev;
	if(!head) tail = 0;
	//unlock
	return info;
}
