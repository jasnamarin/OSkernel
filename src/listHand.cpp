#include "kernel.h"
#include "listHand.h"

int ListHand::globalyBlocked[16];

void ListHand::put(SignalHandler handler) {
	lock

	HandlerNode* tmp = new HandlerNode(handler);

	if(first == 0)
		first = tmp;
	else
		last->next = tmp;
	last = tmp;

	len++;

	unlock
}

SignalHandler ListHand::get() {
	lock

	if(first == 0) {
		unlock
		return 0;
	}

	SignalHandler handler = first->handler;
	HandlerNode* tmp = first;
	first = first->next;
	if(first == 0)
		last = 0;

	tmp->next = 0;
	delete tmp;
	len--;

	unlock

	return handler;
}

void ListHand::processHandlers() {
	lock

	HandlerNode* tmp = first;
	while(tmp != 0) {
		tmp->handler();
		tmp = tmp->next;
	}

	unlock
}

void ListHand::swap(SignalHandler h1, SignalHandler h2) {
	lock

	if(first == 0) {
		unlock
		return;
	}

	HandlerNode* tmp1 = first;
	while(tmp1 != 0) {
		if(tmp1->handler == h1)
			break;
		tmp1 = tmp1->next;
	}

	if(tmp1 == 0) {
		unlock
		return;
	}

	HandlerNode* tmp2 = first;
	while(tmp2 != 0) {
		if(tmp2->handler == h2)
			break;
		tmp2 = tmp2->next;
	}

	if(tmp2 == 0) {
		unlock
		return;
	}

	SignalHandler tmp = tmp1->handler;
	tmp1->handler = tmp2->handler;
	tmp2->handler = tmp;

	unlock
}

void ListHand::clear() {
	lock

	HandlerNode* old;
	while(first != 0) {
		old = first;
		first = first->next;
		delete old;
	}

	unlock
}

ListHand::~ListHand() {
	clear();
}
