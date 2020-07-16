#include "kerSem.h"
#include "kernel.h"
#include "schedule.h"

extern PCB* running;
extern ListSem allSems;

KernelSem::KernelSem(int init) {
	value = init;
	blockedLimited = new ListPCB();
	blockedUnlimited = new ListPCB();

	allSems.addToList(this);
}

int KernelSem::wait(Time maxTimeToWait) {
	lock

	if(--value < 0) {
		running->timedOut = 0;
		running->blocked = 1;

		if(maxTimeToWait == 0) blockedUnlimited->addToList(running);

		else blockedLimited->addToListTimed(running, maxTimeToWait);

		unlock

		// switch context
		dispatch();

		// when thread runs again
		if(running->timedOut == 1) return 0;
		return 1;
	}

	unlock
	return 1;
}

int KernelSem::signal(int n) {
	int returnVal;
	PCB* tmp;
	lock

	if(n == 0) {
		if(++value <= 0) {
			if(blockedUnlimited->len > 0)
				tmp = blockedUnlimited->pop();
			else
				tmp = blockedLimited->popTimed();

			tmp->blocked = 0;
			Scheduler::put(tmp);
		}		
		returnVal = 0;
	}

	else if(n > 0) {
		value += n;
		int toWake = n;
		int woken;

		if(blockedUnlimited->len > 0) {
			woken = ((blockedUnlimited->len < toWake) ? blockedUnlimited->len : toWake);
			blockedUnlimited->removeFromList(woken);
			toWake-= woken;
		}

		if(toWake > 0 && blockedLimited->len > 0) {
			woken = ((blockedLimited->len < toWake) ? blockedLimited->len : toWake);
			blockedLimited->removeFromListTimed(woken);
			toWake -= woken;
		}

		returnVal = n - toWake;		
	}

	else returnVal = n;

	unlock
	return returnVal;
}

void KernelSem::tick() {
	lock

	if(blockedLimited->len > 0) {
		blockedLimited->first->ticks--;

		if(blockedLimited->first->ticks == 0) {
			int cnt = 0;
			ListPCB::Node* curr = blockedLimited->first;

			while(curr != 0 && curr->ticks == 0) {
				cnt++;
				curr->pcb->timedOut = 1;
				curr = curr->next;
			}

			blockedLimited->removeFromList(cnt);
		}
	}

	unlock
}

void KernelSem::clear() {
	lock

	if(blockedUnlimited->len > 0) {
		value += blockedUnlimited->len;
		blockedUnlimited->removeFromList(blockedUnlimited->len);
	}

	if(blockedLimited->len > 0) {
		value += blockedLimited->len;
		blockedUnlimited->removeFromList(blockedLimited->len);
	}

	unlock
}

int KernelSem::val() const { return value; }

KernelSem::~KernelSem() {
	allSems.removeFromList(this);
	clear();
}
