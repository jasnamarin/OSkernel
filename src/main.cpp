#include "kernel.h"
#include "schedule.h"

extern PCB* running;
extern PCB* idle;
extern PCB* starter;

int main(int argc, char* argv[]) {

	StarterThread* startThread = new StarterThread();
	MainThread* mainThread = new MainThread(argc, argv);
	IdleThread* idleThread = new IdleThread();

	idleThread->start();
	idle = Scheduler::get();
	idle->completed = 1; // idle thread is never put into Scheduler
	idle->defaultTimeSlice = 1;

	startThread->start();
	starter = Scheduler::get();

	running = starter; // main kernel thread

	mainThread->start();
	int returnValue;

	mainThread->waitToComplete();

	returnValue = mainThread->returnValue;

	if(idleThread) delete idleThread;
	if(mainThread) delete mainThread;
	if(startThread) delete startThread;
	
	return returnValue;
}
