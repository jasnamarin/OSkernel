#include "kernel.h"
#include "schedule.h"

ListPCB allThreads;
ListSem allSems;

PCB* running;
PCB* idle;
PCB* starter;

volatile unsigned lockFlag = 1; // context switch enabled
volatile unsigned request = 0;

unsigned tsp, tss, tbp;

unsigned oldTimerSEG, oldTimerOFF;

void inic() {
	asm {
		cli
		push es
		push ax

		mov ax, 0
		mov es, ax
		
		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

void restore() {
	asm {
		cli
		push es
		push ax

		mov ax, 0
		mov es, ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

void dispatch() {
	lock
	request = 1;
	timer();
	unlock
}

void interrupt timer() {
	if(request == 0) {
		tick();
		if(allSems.len > 0) {
			ListSem::SemNode* curr = allSems.first;
			while(curr != 0) {
				curr->sem->tick();
				curr = curr->next;
			}
		}
	}

	if(request == 0 && running->timeSlice != 0)
		running->timeSlice--;

	if(((running->defaultTimeSlice != 0 && running->timeSlice == 0) || request == 1) && lockFlag == 1) {
		// store current stack pointer to PCB of running thread
		asm {
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}
		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;

		if(running != 0 && running->completed == 0 && running->blocked == 0)
			Scheduler::put(running);

		do {
			// choose the next thread to run
			running = Scheduler::get();

			if(running == 0)
				running = idle;

			// assign time slice to it
			running->timeSlice = running->defaultTimeSlice;

			// move to the stack of the thread next to run
			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}

			lock
			if(running != idle) {
				running->handleSignals();
				if (running->completed == 1 && running->stack != 0)
					delete running->stack;
			}
			unlock

		} while(running->completed == 1 && running != idle);

	}

	if(request == 0) asm int 60h;
	request = 0;
}

void MainThread::run() {
	inic();
	returnValue = userMain(argc, argv);
	restore();
}

void IdleThread::run() {
	while(1);
}
