#include "pcb_lst.h"
#include "pcb.h"
#include "lock.h"
#include "schedule.h"

PCB_list::PCB_list(){
	first = 0; last = 0;
	num_of_nodes = 0;
}

void PCB_list::add(PCB* pcb){

	Node* new_node = new Node(pcb);

	if(first == 0) first = last = new_node;
	else {
		last->next = new_node;
		new_node->prev = last;
		last = new_node;
	}
	num_of_nodes++;
}

void PCB_list::add_blocking(int blocker_id, PCB* pcb){

	Node* new_node = new Node(pcb,blocker_id);

		if(first == 0) first = last = new_node;
		else {
			last->next = new_node;
			new_node->prev = last;
			last = new_node;
		}
		num_of_nodes++;
}

void PCB_list::remove(int thread_id){

	if(first == 0) return;

	Node *temp = first;

	while(temp && temp->info->thread_id != thread_id)
		temp = temp->next;

	if(temp == 0) return;

	Node* old = temp;

	if(temp == first){

		if(first == last){
			first = last = 0;
			num_of_nodes = 0;
			delete old;
			return;
		}

		else{
			first = first->next;
			first->prev = 0;
			num_of_nodes--;
			delete old;
			return;
		}
	}

	if(temp == last){
		last = last->prev;
		last->next = 0;
		num_of_nodes--;
		delete old;
		return;
	}

	temp->prev->next = temp->next;
	temp->next->prev = temp->prev;
	num_of_nodes--;
	delete old;
}

void PCB_list::quick_remove(Node* n){

	if(n == 0) return;

	if(n == first){
		if(n == last){
			first = last = 0;
			num_of_nodes = 0;
			delete n;
			return;
		}

		else{
			first = first->next;
			first->prev = 0;
			num_of_nodes--;
			delete n;
			return;
		}
	}

	if(n == last){
		last = last->prev;
		last->next = 0;
		num_of_nodes--;
		delete n;
		return;
	}

	n->prev->next = n->next;
	n->next->prev = n->prev;
	num_of_nodes--;
	delete n;
}

PCB* PCB_list::get_remove_last(){

	if(last == 0) return 0;

	PCB* ret = last->info;
	Node* old = last;

	if(last == first){
		last = first = 0;
		num_of_nodes = 0;
		delete old;
		return ret;
	}

	else{
		last = last->prev;
		last->next = 0;
		num_of_nodes--;
		delete old;
		return ret;
	}
}

void PCB_list::unblock_threads(int blocker_id, int num_of_blocked){

	Node *temp = first;
	int i = num_of_blocked;

	while(temp && i){
		if(temp->del_blocker == blocker_id){
			temp->info->ready = 1;
			Scheduler::put(temp->info);
			Node* old = temp;
			temp = temp->next;
			quick_remove(old);
			i--;
		}
		else temp = temp->next;
	}
}

void PCB_list::decrement(){

	Node* temp = first;
	while(temp){
		temp->info->remaining_blocking_interval--;

		if(temp->info->remaining_blocking_interval == 0){
			temp->info->blocked = 0;
			temp->info->blocked_on = 0;
			Scheduler::put(temp->info);
			Node* old = temp;
			temp = temp->next;
			quick_remove(old);
		}
		else temp = temp->next;
	}
}

void PCB_list::delete_list(){

	if(first == 0) return;

	Node *temp = first;
	Node * old = 0;

	while(temp){
		old = temp;
		temp = temp->next;
		delete old;
	}

	num_of_nodes = 0;
	first = last = 0;
}

PCB* PCB_list::find(int id){

	if(first == 0) return 0;

	Node* temp = first;
	while(temp && temp->info->thread_id!=id)
		temp = temp->next;

	if(temp == 0) return 0;

	else return temp->info;
}


