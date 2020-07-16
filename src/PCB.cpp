#include <dos.h>
#include "schedule.h"
#include "PCB.h"

extern PCB* running;

volatile ID PCB::IDcnt = -1;

PCB::PCB(StackSize _stackSize, Time _timeSlice, Thread* _myThread)
: stackSize(((_stackSize <= MAX_STACK_SIZE) ? _stackSize : MAX_STACK_SIZE) / sizeof(unsigned)),
  threadID(++IDcnt) {

	defaultTimeSlice = _timeSlice;
	timeSlice = _timeSlice;
	myThread = _myThread;

	completed = 0;
	blocked = 0;
	timedOut = 0;

	mySem = new KernelSem(0);

	stack = 0;

	if(threadID == 0) // main (starter) thread
		return;

	// create stack for user thread
	lock
	stack = new unsigned[stackSize];
	stack[stackSize - 1] = 0x200; // PSWI = 1
	stack[stackSize - 2] = FP_SEG(wrapper);
	stack[stackSize - 3] = FP_OFF(wrapper);
	sp = FP_OFF(stack + stackSize - 12);
	ss = FP_SEG(stack + stackSize - 12);
	bp = sp;

	parentThreadID = Thread::getRunningId();
	signals = new ListSign();

	unlock
}

void PCB::start() {
	lock
	Scheduler::put(this);
	unlock
}

void PCB::run() {
	myThread->run();
	mySem->clear();
	completed = 1;
	dispatch();
}

void PCB::wrapper() { 
	running->run();

	lock

	// release waiting threads
	running->mySem->clear();

	// send system signals
	Thread* parent = Thread::getThreadById(running->parentThreadID);
	if(parent != 0 && parent->myPCB != 0)
		parent->myPCB->signal(1);
	running->signal(2);
	running->handleSignals();

	running->completed = 1;

	unlock
	dispatch();
}

void PCB::waitToComplete() { if(completed == 0) mySem->wait(0); }

void PCB::signal(SignalId signal) {	signals->put(signal); }

void PCB::registerHandler(SignalId signal, SignalHandler handler) { handlers[signal].put(handler); }

void PCB::unregisterAllHandlers(SignalId id) { handlers[id].clear(); }

void PCB::blockSignal(SignalId signal) { handlers[signal].locallyBlocked = 1; }

void PCB::blockSignalGlobally(SignalId signal) { ListHand::globalyBlocked[signal] = 1; }

void PCB::unblockSignal(SignalId signal) { handlers[signal].locallyBlocked = 0; }

void PCB::unblockSignalGlobally(SignalId signal) { ListHand::globalyBlocked[signal] = 0; }

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	lock

	if(hand1 == hand2) {
		unlock
		return;
	}
	handlers[id].swap(hand1, hand2);

	unlock
}

void PCB::handleSignals() {
	lock

	if(signals == 0) {
		unlock
		return;
	}

	SignalId tmp = signals->get();
	while(tmp != -1) {
		// call handler functions for signal if it's not locally or globaly blocked
		if(handlers[tmp].locallyBlocked == 0 && ListHand::globalyBlocked[tmp] == 0) {
			handlers[tmp].processHandlers();
		}

		if(tmp == 0) {
			// kill this thread
			signals->clear();
			if(mySem != 0)
				delete mySem;
			completed = 1;
			unlock
			dispatch();
		}

		tmp = signals->get();
	}

	unlock
}

PCB::~PCB() {
	lock

	if(stack != 0)
		delete stack;
	if(mySem != 0)
		delete mySem;
	if(signals != 0)
		delete signals;

	unlock
}
