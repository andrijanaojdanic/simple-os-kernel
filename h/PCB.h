#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "pcb_lst.h"
typedef unsigned REG;

class PCB_list;
class KernelSem;

class PCB{

public:
	static int num_of_active_threads;
	static int id_creator;
	volatile static PCB* running;
	static PCB_list* all_threads;
	static PCB_list* threads_blocked_upon_del;
	static PCB_list* sleeping_threads;


	KernelSem* blocked_on;

	REG sp;
	REG ss;
	REG bp;

	Thread* myThread;
	Time quantum;

	int active;
	int finished;
	int unlimited;
	int ready;
	int blocked;
	int am_blocking_upon_del;

	int thread_id;

	unsigned int remaining_blocking_interval;
	unsigned* stack;


	PCB(unsigned long st_sz, unsigned int tm, Thread* my_th );
	PCB();
	~PCB();


	static void wrapper();

	static void exit_thread();
	static int get_num_of_threads();
};


#endif
