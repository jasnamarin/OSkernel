#include "listSign.h"
#include "kernel.h"

void ListSign::put(SignalId signal) {
	lock

	SignalNode* tmp = new SignalNode(signal);

	if(first == 0)
		first = tmp;
	else
		last->next = tmp;
	last = tmp;

	len++;

	unlock
}

SignalId ListSign::get() {
	lock

	if(first == 0) {
		unlock
		return -1;
	}

	SignalId signal = first->signal;
	SignalNode* tmp = first;
	first = first->next;
	if(first == 0)
		last = 0;

	tmp->next = 0;
	delete tmp;
	len--;

	unlock

	return signal;
}

void ListSign::clear() {
	lock

	SignalNode* old;
	while(first != 0) {
		old = first;
		first = first->next;
		delete old;
	}

	unlock
}

ListSign::~ListSign() {
	clear();
}

