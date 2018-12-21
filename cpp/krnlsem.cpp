#include "krnlsem.h"
#include "pcb_lst.h"
#include "lock.h"
#include "pcb.h"
#include "schedule.h"

KernelSem::KernelSem(int init, Semaphore* s){
	Lock::lock_flag = 1;

	val = init;
	my_sem = s;
	my_blocked_threads = new PCB_list();

	Lock::lock_flag = 0;
}

KernelSem::~KernelSem(){
	Lock::lock_flag = 1;
	delete my_blocked_threads;
	Lock::lock_flag = 0;
}

int KernelSem::wait(unsigned int max_time){
	Lock::lock_flag = 1;

	if(max_time != 0){
		PCB::running->blocked = 1;
		PCB::running->blocked_on = this;
		PCB::running->remaining_blocking_interval = max_time;
		PCB::sleeping_threads->add((PCB*)PCB::running);
		dispatch();
		return 0;
	}
	else{
		val--;
		if(val>=0){
			Lock::lock_flag = 0;
			return 1;
		}
		else{
			PCB::running->blocked = 1;
			PCB::running->blocked_on = this;
			PCB::running->remaining_blocking_interval = 0;
			my_blocked_threads->add((PCB*)PCB::running);
			dispatch();
			return 1;
		}
	}
}

void KernelSem::signal(){
	Lock::lock_flag = 1;

	val++;

	if(val>0){
		Lock::lock_flag = 0;
		return;
	}

	else{
		if(my_blocked_threads == 0 || my_blocked_threads->num_of_nodes == 0) return;
		PCB* unblocked = my_blocked_threads->get_remove_last();
		if(!unblocked) return;
		unblocked->blocked = 0;
		unblocked->blocked_on = 0;
		Scheduler::put(unblocked);
		Lock::lock_flag = 0;
	}
}

