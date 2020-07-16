#ifndef _LIST_PCB_H_
#define _LIST_PCB_H_

class PCB;

class ListPCB {
public:

//------------------------list-node------------------------//
	class Node{
	public:
		PCB* pcb;
		Time ticks; // for semaphores
		Node* next;

		Node(PCB* _pcb, Time _ticks = 0, Node* _next = 0)
		: pcb(_pcb), ticks(_ticks), next(_next) {}
		~Node() {}
	};
//------------------------list-node------------------------//
//---------------------------------------------------------//
//-----------------------attributes------------------------//

	Node* first;
	Node* last;
	int len;

//-----------------------attributes------------------------//
//-------------------------methods-------------------------//

	ListPCB() : first(0), last(0), len(0) {}
	~ListPCB();

	void addToList(PCB* pcb);
	void removeFromList(int cnt);
	void removeNodeFromList(int id);
	PCB* pop();

  //---------for-semaphore-waiting-queue---------//
	void addToListTimed(PCB* pcb, Time ticks);
	void removeFromListTimed(int cnt);
	PCB* popTimed();
  //---------for-semaphore-waiting-queue---------//
	
	PCB* findByID(int id);
	void clear();

//-------------------------methods-------------------------//

};

#endif