#ifndef _IVT_ENTRY_H_
#define _IVT_ENTRY_H_

#include "kernel.h"

#define PREPAREENTRY(ivtNo, oldIRflag)\
void interrupt eventIR##ivtNo(...) {\
	IVTEntry::getEntry(ivtNo)->signal();\
	if(oldIRflag == 1 && IVTEntry::getOldIR(ivtNo) != 0) {\
		asm pushf;\
		(*IVTEntry::getOldIR(ivtNo))();\
		asm popf;\
	}\
	dispatch();\
} IVTEntry ivtEntry##ivtNo(ivtNo, eventIR##ivtNo);


#define MAX_IVTENTRY 256

class KernelEv;

class IVTEntry {
public:
//----------------------------methods-----------------------------//

	IVTEntry(int ivtNo, void interrupt (*interruptHandler)(...));
	~IVTEntry();

	void assignEvent(KernelEv* _event);
	void clearEvent();
	void signal();
	void interrupt (*getOldIR())(...);

	static IVTEntry* getEntry(int ivtNo);
	static void interrupt (*getOldIR(int ivtNo))(...);

//----------------------------methods-----------------------------//
//---------------------------attributes---------------------------//
	
	KernelEv* event;
private:
	int ivtNo;
	void interrupt (*oldIR)(...);

//---------------------------attributes---------------------------//
};

#endif
