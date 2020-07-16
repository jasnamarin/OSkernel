#ifndef _LIST_SIGNAL_H_
#define _LIST_SIGNAL_H_

typedef unsigned SignalId;

class ListSign {

public:
//------------------------list-node------------------------//
	class SignalNode{
	public:
		SignalId signal;
		SignalNode* next;

		SignalNode(SignalId _signal, SignalNode* _next = 0)
		: signal(_signal), next(_next) {}
		~SignalNode() {}
	};
//------------------------list-node------------------------//
//---------------------------------------------------------//
//-----------------------attributes------------------------//

	SignalNode* first;
	SignalNode* last;
	int len;

//-----------------------attributes------------------------//
//-------------------------methods-------------------------//

	ListSign() : first(0), last(0), len(0) {}
	~ListSign();

	void put(SignalId signal);
	SignalId get();

	void clear();

//-------------------------methods-------------------------//
};

#endif
