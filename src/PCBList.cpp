#include "PCBList.h"
#include "thread.h"

PCBList::PCBList(){
		//lock
		head = 0;
		tail = 0;
		//unlock
	}

PCBList::~PCBList(){
	//lock
	elem* prev = 0;
	for(elem* cur = head; cur != 0; cur = cur->next){
		if(prev){
			delete prev;
		}
		prev = cur;
	}
	if(prev) delete prev;
	//unlock
}

void PCBList::add(PCB* data){
	//lock
	if(!head){
		head = new elem(data);
		tail = head;
	}
	else{
		tail->next = new elem(data);
		tail = tail->next;
	}
	//unlock
}

void PCBList::remove(PCB* data){
	if(!head) return;
	//lock
	elem* prev = 0;
	for(elem* cur = head; cur != 0; cur = cur->next){
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

PCB* PCBList::pop(){
	if(!head) return 0;
	//lock
	PCB* info = head->info;
	elem* prev = head;
	head = head->next;
	delete prev;
	if(!head) tail = 0;
	//unlock
	return info;
}

Thread* PCBList::getById(ID id){
	if(!head) return 0;
	for(elem* cur = head; cur != 0; cur = cur->next){
		if(cur->info->id == id){
			return cur->info->myThread;
		}
	}
	return 0;
}
