#include "PCB.h"
#include "dos.h"
#include "pcb_lst.h"
#include "lock.h"

int PCB::num_of_active_threads = 0;
int PCB::id_creator = 0;

volatile PCB* PCB::running;

PCB_list* PCB::all_threads = new PCB_list();
PCB_list* PCB::threads_blocked_upon_del = new PCB_list();
PCB_list* PCB::sleeping_threads = new PCB_list();

PCB::PCB(unsigned long st_sz, unsigned int tm,Thread* my_th ){

		Lock::lock_flag = 1;

		blocked = 0;
		blocked_on = 0;
		active = 0;
		am_blocking_upon_del = 0;
		if(tm==0) unlimited = 1;
		else unlimited = 0;
		ready = 1;
		finished = 0;

		quantum = tm;
		remaining_blocking_interval = 0;

		myThread = my_th;
		thread_id = ++id_creator;

		unsigned func_off = 0;
		unsigned func_seg = 0;

#ifndef BCC_BLOCK_IGNORE
		func_off = FP_OFF(wrapper);
		func_seg = FP_SEG(wrapper);
#endif

		stack = new unsigned[st_sz];
		stack[st_sz-1] = 0x0200;
		stack[st_sz-2] = func_seg;
		stack[st_sz-3] = func_off;

		sp = bp = ss = 0;

		unsigned* temp = stack+st_sz-12;

#ifndef BCC_BLOCK_IGNORE
		sp = FP_OFF(temp);
		bp = FP_OFF(temp);
		ss = FP_SEG(temp);
#endif


		all_threads->add(this);

		Lock::lock_flag = 0;
}

PCB::PCB(){

			Lock::lock_flag = 1;

			blocked = 0;
			blocked_on = 0;
			active = 0;
			am_blocking_upon_del = 0;
			unlimited = 0;
			ready = 1;

			quantum = 2;
			remaining_blocking_interval = 0;

			myThread = 0;
			thread_id = 0;

			unsigned func_off = 0;
			unsigned func_seg = 0;

			stack = new unsigned[4096];

			sp = bp = ss = 0;

	#ifndef BCC_BLOCK_IGNORE
			sp = bp = FP_OFF(stack+4096-12);
			ss = FP_SEG(stack+4096-12);
	#endif

		finished = 0;
		PCB::all_threads->add(this);

		Lock::lock_flag = 0;
}

PCB::~PCB(){
	Lock::lock_flag = 1;
	delete[] stack;
	Lock::lock_flag = 0;
}

void PCB::wrapper(){
		running->myThread->run();
		exit_thread();
	}

int PCB::get_num_of_threads(){return all_threads->num_of_nodes;}

void PCB::exit_thread(){

		Lock::lock_flag = 1;
		running->finished = 1;
		if(running->am_blocking_upon_del) threads_blocked_upon_del->unblock_threads(running->thread_id,running->am_blocking_upon_del);
		running->am_blocking_upon_del = 0;
		Lock::lock_flag = 0;
		dispatch();
	}
