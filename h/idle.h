#ifndef _IDLE_H_
#define _IDLE_H_

#include "PCB.h"

class Idle: public Thread{
public:
	static Idle* idle_thread;
	void run();

	Idle();
	~Idle();
};

#endif
