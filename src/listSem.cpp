#include "listSem.h"
#include "kernel.h"

void ListSem::addToList(KernelSem* sem) {
	lock

	SemNode* newNode = new SemNode(sem);
	len++;

	if(first == 0) first = newNode;
	else last->next = newNode;
	last = newNode;

	unlock
}

void ListSem::removeFromList(KernelSem* sem) {
	lock

	SemNode* curr = first;
	SemNode* prev = 0;

	while(curr != 0) {
		if(curr->sem == sem)
			break;

		prev = curr;
		curr = curr->next;
	}

	if(curr != 0) {
		len--;
		if(prev != 0) prev->next = curr->next;

		if(curr == first)
			first = first->next;
		if(curr == last)
			last = prev;

		delete curr;
	}

	unlock
}

void ListSem::clear() {
	lock

	SemNode* old;
	while(first != 0) {
		old = first;
		first = first->next;
		delete old;
	}

	unlock
}

ListSem::~ListSem() { clear(); }
