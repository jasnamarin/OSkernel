#ifndef _KERNEL_SEM_
#define _KERNEL_SEM_

class ListPCB;

typedef unsigned int Time;

class KernelSem {
public:
//-------------------------------attributes-------------------------------//

	ListPCB* blockedLimited; // threads blocked with maxTimeToWait != 0
	ListPCB* blockedUnlimited; // threads blocked with maxTimeToWait == 0

	int value;

//-------------------------------attributes-------------------------------//
//--------------------------------methods---------------------------------//

	KernelSem(int init);
	~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n);

	void tick();
	void clear();

	int val() const;

//--------------------------------methods---------------------------------//

};

#endif
