#ifndef _KERNEL_EVENT_H_
#define _KERNEL_EVENT_H_

#include "IVTEntry.h"

class PCB;

class KernelEv {
public:
//-------------------------methods-------------------------//

	KernelEv(int _ivtn);
	~KernelEv();

	void wait();
	void signal();

	int getIVTNo() const { return ivtNo; }

//-------------------------methods-------------------------//
private:
//-----------------------attributes------------------------//

	int ivtNo;
	PCB* blockedThread; // owner thread or 0
	int threadID; // owner thread ID
	int value;

//-----------------------attributes------------------------//
};

#endif
