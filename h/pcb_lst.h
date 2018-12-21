#ifndef _PCBLIST_H_
#define _PCBLIST_H_

class PCB;

class PCB_list{

private:
	struct Node{
		PCB* info;
		Node* prev;
		Node* next;
		int del_blocker;

		Node(PCB* p){
			info = p;
			next = 0;
			prev = 0;
			del_blocker = -2;
		}

		Node(PCB* p, int b){
			info = p;
			next = 0;
			prev = 0;
			del_blocker = b;
		}
	};

	Node* first;
	Node* last;

public:
	int num_of_nodes;

	PCB_list();
	~PCB_list(){delete_list();}

	void add(PCB* pcb);

	void add_blocking(int blocker_id, PCB* pcb);
	void unblock_threads(int blocker_id, int num_of_blocked);

	void remove(int thread_id);
	void quick_remove(Node* n);

	PCB* get_remove_last();

	void delete_list();

	PCB* find(int id);

	void decrement();

};

#endif
