#include "thread.h"
#include "PCB.h"
#include "pcb_lst.h"
#include "SCHEDULE.H"
#include "idle.h"
#include "lock.h"

Thread::Thread (StackSize stackSize, Time timeSlice){
		Lock::lock_flag = 1;

		int stSize;
		if(stackSize<=32768) stSize = stackSize;
		else stSize = 32768;
		myPCB = new PCB(stSize, timeSlice, this);

		Lock::lock_flag = 0;
}

Thread::~Thread(){
	waitToComplete();

	Lock::lock_flag = 1;
	delete myPCB;
	Lock::lock_flag = 0;
}

void Thread::start(){
	Lock::lock_flag = 1;

	myPCB->active = 1;
	PCB::num_of_active_threads++;
	Scheduler::put(myPCB);

	Lock::lock_flag = 0;
}

ID Thread::getId(){
	return this->myPCB->thread_id;
}

ID Thread::getRunningId(){
		return PCB::running->thread_id;
	};

Thread* Thread::getThreadById(ID id){
	return PCB::all_threads->find(id)->myThread;
};

PCB* Thread::getMyPCB(){return myPCB;}

void Thread::waitToComplete(){

	if(PCB::running == myPCB) return;
	if(myPCB->finished) return;

	if(this == Idle::idle_thread) return;

	Lock::lock_flag = 1;
	if(!myPCB->finished){
		myPCB->am_blocking_upon_del++;
		PCB::running->ready = 0;
		PCB::threads_blocked_upon_del->add_blocking(myPCB->thread_id, (PCB*)PCB::running);
		dispatch();
	}
	else Lock::lock_flag = 0;
}

