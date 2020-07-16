#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "PCB.h"
#include "listPCB.h"
#include "listSem.h"

#define lock asm { pushf; cli; }
#define unlock asm popf

extern int userMain(int args, char* argv[]);
extern void tick();

void inic();
void interrupt timer();
void restore();

//--------------------main-thread--------------------//
class MainThread : public Thread {
public:
	int returnValue;
	virtual void run();

	MainThread(int _argc, char* _argv[])
	: Thread(), argc(_argc), argv(_argv) {}

private:
	int argc;
	char** argv;
};
//--------------------main-thread--------------------//

//--------------------idle-thread--------------------//
class IdleThread : public Thread {
public:
	virtual void run();

	IdleThread() : Thread() {}
};
//--------------------idle-thread--------------------//

//-------------------starter-thread------------------//
class StarterThread : public Thread {
public:
	StarterThread() : Thread() {}
};
//------------------starter-thread-------------------//

#endif
