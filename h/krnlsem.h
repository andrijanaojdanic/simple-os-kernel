#ifndef _KRNLSEM_H_
#define _KRNLSEM_H_

class Semaphore;
class PCB_list;

class KernelSem{

public:

	int val;
	Semaphore* my_sem;

	PCB_list* my_blocked_threads;

	KernelSem(int init, Semaphore* s);
	~KernelSem();

	int wait(unsigned int max_time);
	void signal();
};

#endif
