#include "kernelEv.h"
#include "IVTEntry.h"
#include "schedule.h"

extern PCB* running;
extern volatile unsigned lockFlag;
extern IVTEntry* ivtList[MAX_IVTENTRY];

KernelEv::KernelEv(int _ivtNo) {
	lock

	ivtNo = _ivtNo;
	value = 0;
	threadID = running->threadID;

	if(ivtList[ivtNo] != 0)
		ivtList[ivtNo]->assignEvent(this);

	unlock
}

void KernelEv::wait() {
	// only the owner of this event
	// can be blocked on it
	if(running->threadID != threadID)
		return;								

	lockFlag = 0;

	// block running thread on this event
	if(value == 0) {
		running->blocked = 1;
		blockedThread = running;

		lockFlag = 1;

		dispatch();

		// when thread runs again
		lockFlag = 0;

		if(blockedThread != 0) {
			blockedThread->blocked = 0;
			value = 0;
			blockedThread = 0;
		}
	}

	value = 0;
	lockFlag = 1;
}

void KernelEv::signal() {
	lock

	value = 1;
	if(blockedThread != 0) {
		blockedThread->blocked = 0;
		value = 0;
		Scheduler::put(blockedThread);
		blockedThread = 0;
	}

	unlock
}

KernelEv::~KernelEv() {
	if(ivtList[ivtNo] != 0)
		ivtList[ivtNo]->event = 0;
}
