#include "kernel.h"
#include "schedule.h"

void ListPCB::addToList(PCB* pcb) {
	lock

	Node* newNode = new Node(pcb);
	len++;

	if(first == 0) first = newNode;
	else last->next = newNode;
	last = newNode;

	unlock
}

void ListPCB::removeNodeFromList(int id) {
	lock

	Node* curr = first;
	Node* prev = 0;

	while(curr != 0) {
		if(curr->pcb->threadID == id)
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

// puts PCBs into Scheduler queue
// after removing them from the list
void ListPCB::removeFromList(int cnt) {
	lock

	Node* curr = first;
	Node* prev = 0;

	while(cnt > 0 && curr != 0) {
		prev = curr;
		curr = curr->next;

		prev->pcb->blocked = 0;
		Scheduler::put(prev->pcb);
		delete prev;

		cnt--;
		len--;
	}

	// list is empty
	if(curr == 0) {
		first = 0;
		last = 0;
	}

	else first = curr;

	unlock
}

PCB* ListPCB::pop() {
	lock

	if(first == 0) {
		unlock
		return 0;
	}

	Node* curr = first;
	first = first->next;
	if(first == 0) last = 0;
	len--;

	PCB* firstPCB = curr->pcb;
	delete curr;

	unlock
	return firstPCB;
}

PCB* ListPCB::findByID(int id) {
	lock

	Node* curr = first;
	while(curr) {
		if(curr->pcb->threadID == id)
			break;
		curr = curr->next;
	}

	if(curr == 0) {
		unlock;
		return 0;
	}

	unlock
	return curr->pcb;
}

void ListPCB::addToListTimed(PCB* pcb, Time ticks) {
	if(ticks == 0)
		return;

	lock

	Node* newNode = new Node(pcb);
	len++;

	if(first == 0) {
		newNode->ticks = ticks;
		first = newNode;
		last = newNode;
	}

	else {
		Node* curr = first;
		Node* prev = 0;
		int totalTicks = 0;

		while(curr != 0) {
			totalTicks += curr->ticks;
			if(totalTicks > ticks)
				break;

			prev = curr;
			curr = curr->next;
		}

		// add newNode as the last element
		if(curr == 0) {
			newNode->ticks = ticks - totalTicks;
			prev->next = newNode;
			last = newNode;
		}

		else {
			totalTicks -= curr->ticks;
			newNode->ticks = ticks - totalTicks;
			curr->ticks -= newNode->ticks;

			newNode->next = curr;

			if(curr != 0 && prev != 0)
				prev->next = newNode;

			// add newNode as the first element
			else first = newNode; 
		}
	}

	unlock
}

// puts PCBs into Scheduler queue
// after removing them from the list
void ListPCB::removeFromListTimed(int cnt) {
	lock

	Node* curr = first;
	Node* prev = 0;
	int totalTicks = 0;

	while(cnt > 0 && curr != 0) {
		prev = curr;
		curr = curr->next;
		totalTicks += prev->ticks;

		prev->pcb->blocked = 0;

		Scheduler::put(prev->pcb);
		delete prev;

		cnt--;
		len--;
	}

	// list is empty
	if(curr == 0) {
		first = 0;
		last = 0;
	}

	// update ticks of remaining nodes
	else {
		curr->ticks += totalTicks;
		first = curr;
	}

	unlock
}

PCB* ListPCB::popTimed() {
	lock

	if(first == 0) {
		unlock
		return 0;
	}

	Node* curr = first;
	first = first->next;
	if(first == 0) last = 0;
	else first->ticks += curr->ticks;
	len--;

	PCB* firstPCB = curr->pcb;
	delete curr;

	unlock
	return firstPCB;
}

void ListPCB::clear() {
	lock

	Node* curr;
	while(first != 0) {
		curr = first;
		first = first->next;
		delete curr;
	}

	unlock
}

ListPCB::~ListPCB() { clear(); }
