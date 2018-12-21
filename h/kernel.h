#ifndef _KERNEL_H_
#define _KERNEL_H_

void interrupt timer(...);
class PCB;

class Kernel{
public:

	static PCB* main_pcb;

	static void initialize_IV();
	static void restore_IV();

	static void system_initialization();
	static void system_restore();
};

#endif
