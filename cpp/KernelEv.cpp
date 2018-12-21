#include "KernelEv.h"
#include "event.h"
#include "pcb.h"
#include "schedule.h"
#include "thread.h"
#include "lock.h"
#include "IVTEntr.h"

KernelEv::KernelEv(unsigned char ivt, Event* myEv, PCB* ev_creator){
	Lock::lock_flag = 1;

	ivtNo = ivt;
	my_event = myEv;
	my_blocked_thread = 0;
	event_creator = ev_creator;
	val = 0;
	IVTEntry::all_entries[ivtNo]->my_kernel_ev = this;

	Lock::lock_flag = 0;
}

KernelEv::~KernelEv(){}

void KernelEv::wait(){

	if(PCB::running!=event_creator) return;

	Lock::lock_flag = 1;

	if(val){
		val = 0;
		Lock::lock_flag = 0;
		return;
	}

	PCB::running->blocked = 1;
	my_blocked_thread = (PCB*)PCB::running;
	dispatch();

}

void KernelEv::signal(){
	Lock::lock_flag = 1;
	if(val){
		Lock::lock_flag = 0;
		return;
	}
	else{
		if(my_blocked_thread){
			my_blocked_thread->blocked = 0;
			Scheduler::put(my_blocked_thread);
			my_blocked_thread = 0;
			Lock::lock_flag = 0;
			return;
		}
		else{
			val = 1;
			Lock::lock_flag = 0;
			return;
		}
	}
}
