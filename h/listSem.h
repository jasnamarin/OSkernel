#ifndef _LIST_SEM_H_
#define _LIST_SEM_H_

class KernelSem;

class ListSem {
public:

//------------------------list-node------------------------//
	class SemNode {
	public:
		KernelSem* sem;
		SemNode* next;

		SemNode(KernelSem* _sem, SemNode* _next = 0)
		: sem(_sem), next(_next) {}
		~SemNode() {}
	};
//------------------------list-node------------------------//
//---------------------------------------------------------//
//-----------------------attributes------------------------//

	SemNode* first;
	SemNode* last;
	int len;

//-----------------------attributes------------------------//
//-------------------------methods-------------------------//

	ListSem() : first(0), last(0), len(0) {}
	~ListSem();

	void addToList(KernelSem* sem);
	void removeFromList(KernelSem* sem);
	void clear();
	
//-------------------------methods-------------------------//

};

#endif
