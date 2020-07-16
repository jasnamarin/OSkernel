#ifndef _LIST_HANDLERS_H_
#define _LIST_HANDLERS_H_

typedef void (*SignalHandler)();

class ListHand {
public:
//------------------------list-node------------------------//
	class HandlerNode{
	public:
		SignalHandler handler;
		HandlerNode* next;

		HandlerNode(SignalHandler _handler, HandlerNode* _next = 0)
		: handler(_handler), next(_next) {}
		~HandlerNode() {}
	};
//------------------------list-node------------------------//
//---------------------------------------------------------//
//-----------------------attributes------------------------//

	HandlerNode* first;
	HandlerNode* last;
	int len;
	int locallyBlocked; 		  // if signal is blocked
								  // don't call handlers
	static int globalyBlocked[];

//-----------------------attributes------------------------//
//-------------------------methods-------------------------//

	ListHand() : first(0), last(0), len(0), locallyBlocked(0) {}
	~ListHand();

	void put(SignalHandler handler);
	SignalHandler get();

	void processHandlers();
	void swap(SignalHandler h1, SignalHandler h2);

	void clear();

//-------------------------methods-------------------------//
	friend class PCB;
	friend class Thread;
};

#endif
