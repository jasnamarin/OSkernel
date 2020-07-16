#include <dos.h>
#include "IVTEntry.h"
#include "kernelEv.h"

IVTEntry* ivtList[MAX_IVTENTRY];

IVTEntry::IVTEntry(int _ivtNo, void interrupt (*interruptHandler)(...)) {
	lock

	ivtNo = _ivtNo;
	oldIR = getvect(ivtNo);

	setvect(ivtNo, interruptHandler);
	ivtList[ivtNo] = this;

	unlock
}

void IVTEntry::assignEvent(KernelEv* _event) { 
	lock
	event = _event;
	unlock
}

void IVTEntry::signal() { if(event != 0) event->signal(); }

void interrupt (*IVTEntry::getOldIR())(...) { return oldIR; }

//----------------static-methods-----------------//

	IVTEntry* IVTEntry::getEntry(int ivtNo) {
		return ivtList[ivtNo];
	}

	void interrupt (*IVTEntry::getOldIR(int ivtNo))(...) {
		return ivtList[ivtNo]->getOldIR();
	}
	
//----------------static-methods-----------------//

void IVTEntry::clearEvent() {
	lock

	//oldIR();
	setvect(ivtNo, oldIR);
	ivtList[ivtNo] = 0;

	unlock
}

IVTEntry::~IVTEntry() {
	clearEvent();
}
