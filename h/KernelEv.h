#ifndef _KERNELEV_H_
#define _KERNELEV_H_

class Event;
class PCB;

class KernelEv{
public:
	PCB* my_blocked_thread;
	PCB* event_creator;
	Event* my_event;

	int val;
	unsigned char ivtNo;

	KernelEv(unsigned char ivtNo, Event* myEv, PCB* ev_creator);
	~KernelEv();
	void wait();
	void signal();

};

#endif
