#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "kernel.h"
#include "kerSem.h"
#include "listHand.h"
#include "listSign.h"

const StackSize MAX_STACK_SIZE = 65536;

class PCB {
public:
//-----------------------------attributes-----------------------------//

	const StackSize stackSize;
	unsigned* stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;

	Time defaultTimeSlice; // assigned time
	Time timeSlice; // remaining time

  //---------flags---------//
	volatile int completed;
	volatile int blocked;
	volatile int timedOut;
  //-----------------------//

	Thread* myThread;

	volatile static ID IDcnt;
	ID threadID;

	KernelSem* mySem;

  //----------signal----------//
	ID parentThreadID;
	ListHand handlers[16];
	ListSign* signals;
  //----------signal----------//

//-----------------------------attributes-----------------------------//
//------------------------------methods-------------------------------//

	PCB(StackSize _stackSize, Time _defaultTimeSlice, Thread* _myThread);
	~PCB();

	void start();
	void waitToComplete();
	void run();

	static void wrapper();

  //-----------------------------signal-----------------------------//
	void signal(SignalId signal);

	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);

	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	void handleSignals();
  //-----------------------------signal-----------------------------//

//------------------------------methods-------------------------------//

};

#endif
