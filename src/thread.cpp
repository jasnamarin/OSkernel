#include "kernel.h"
#include "listPCB.h"

extern PCB* running;
extern ListPCB allThreads;

Thread::Thread(StackSize stackSize, Time timeSlice) {
	lock
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock
	allThreads.addToList(myPCB);
}

ID Thread::getId() { return myPCB->threadID; }

ID Thread::getRunningId() { return running->threadID; }

Thread* Thread::getThreadById(ID id) { return allThreads.findByID(id)->myThread; }

void Thread::start() { myPCB->start(); }

void Thread::waitToComplete() { myPCB->waitToComplete(); }

void Thread::signal(SignalId signal) { myPCB->signal(signal); }

void Thread::registerHandler(SignalId signal, SignalHandler handler) { myPCB->registerHandler(signal, handler); }

void Thread::unregisterAllHandlers(SignalId id) { myPCB->unregisterAllHandlers(id); }

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) { myPCB->swap(id, hand1, hand2); }

void Thread::blockSignal(SignalId signal) { myPCB->blockSignal(signal); }

void Thread::blockSignalGlobally(SignalId signal) { PCB::blockSignalGlobally(signal); }

void Thread::unblockSignal(SignalId signal) { myPCB->unblockSignal(signal); }

void Thread::unblockSignalGlobally(SignalId signal) { PCB::unblockSignalGlobally(signal); }

Thread::~Thread() {
	allThreads.removeNodeFromList(myPCB->threadID);
	if(myPCB) delete myPCB;
}
