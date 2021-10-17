#include <iostream.h>
#include <dos.h>
#include "thread.h"
#include "semaphor.h"
#include "idleThr.h"
#include "IVTEntry.h"

IVTEntry* entryTable[256];
SemList* semlist = new SemList();
PCBList* pcblist = new PCBList();
IdleThread* idleThread;

volatile PCB* running;
volatile PCB* mainPCB = new PCB();

volatile unsigned int lockFlag = 1;
//pomocne promenljive za prekid tajmera
unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int counter = 2;
volatile int requestedContextSwitch = 0;

void tick();

void decrementSem(){
	// decrement semaphore
	lock
	for(elemS* cur = semlist->head; cur != 0; cur = cur->next){
		//cout << cur << " " << cur->next << endl;
		//cout << "";
		for(elem* curPCB = cur->info->blockedList->head; curPCB != 0;){
			PCB* info = curPCB->info;
			//cout << info->id << "::" << info->semTimeSlice << endl;
			elem* nextPCB = curPCB->next;
			if(info->semTimeSlice != 0){
				info->semTimeSlice--;
				if(info->semTimeSlice == 0){
					cur->info->blockedList->remove(info);
					info->blocked--;
					Scheduler::put(info);
					cur->info->val++;
				}
			}
			curPCB = nextPCB;
		}
	}
	unlock
}

void interrupt timer(){ // prekidna rutina
	if (!requestedContextSwitch) {
		counter--;
		decrementSem();
	}

	if(!requestedContextSwitch) {
		asm int 60h;
		tick();
	}
	if (counter == 0 || requestedContextSwitch) {
		if (lockFlag){
			requestedContextSwitch = 0;
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;

			if (!running->finished && !running->waiting && !running->blocked) Scheduler::put((PCB *) running);
			running = Scheduler::get();
			if(!running)
				running = idleThread->getIdlePCB();

			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;

			counter = running->timeSlice;
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		}
		else requestedContextSwitch = 1;
	}

}

unsigned oldTimerOFF, oldTimerSEG; // old interrupt routine

// initialize kernel
void init(){
	#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		// save old routine
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		// set new routine
		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		// set old routine on int 60h
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
	#endif BCC_BLOCK_IGNORE
}

// restore to initial value
void restore(){
	#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
	#endif BCC_BLOCK_IGNORE
}

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	init();
	lock
	idleThread = new IdleThread();
	idleThread->init();
	pcblist->pop();
	pcblist->add((PCB*)mainPCB);
	mainPCB->id = -2;
	mainPCB->timeSlice = 2;
	mainPCB->started++;
	running = mainPCB;
	unlock
	int ret = userMain(argc, argv);
	restore();
	lock
	//cout<<"SEMLIST"<<endl;
	delete semlist;
	//cout<<"pcbLIST"<<endl;
	delete pcblist;
	//cout<<"idlT"<<endl;
	delete idleThread;
	//cout<<"mainPCB"<<endl;
	delete mainPCB;
	unlock
	lockFlag--;
	cout << "Happy end!";
    lockFlag++;
	return ret;
}
