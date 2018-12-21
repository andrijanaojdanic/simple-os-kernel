#include "dos.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "idle.h"
#include "pcb_lst.h"
#include "lock.h"
#include "krnlsem.h"
#include "kernel.h"

PCB* Kernel::main_pcb = 0;

unsigned oldTimerOFF = 0;
unsigned oldTimerSEG = 0;

volatile int cntr = 2;
volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;
volatile int unlim = 0;

extern void tick();

typedef void interrupt (*p_interrupt)(...);

void Kernel::initialize_IV(){

	static p_interrupt old_timer_routine;

#ifndef BCC_BLOCK_IGNORE
	old_timer_routine = getvect(0x08);
	setvect(0x08,timer);
	setvect(0x60,old_timer_routine);
#endif
}

void Kernel::restore_IV(){

	static p_interrupt old_timer_routine_1;

#ifndef BCC_BLOCK_IGNORE
	old_timer_routine_1 = getvect(0x60);
	setvect(0x08,old_timer_routine_1);
#endif
}


void Kernel::system_initialization(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	initialize_IV();
	PCB::running = new PCB();
	main_pcb = (PCB*)PCB::running;
	Idle::idle_thread = new Idle();
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

void Kernel::system_restore(){

	while(PCB::num_of_active_threads) dispatch();


	#ifndef BCC_BLOCK_IGNORE
		asm cli;
		restore_IV();
		asm sti;
	#endif

	delete PCB::all_threads;
	delete Idle::idle_thread;
	delete main_pcb;

}

void interrupt timer(...){

	if(Lock::lock_flag){
		if(!Lock::switch_on_demand){
			tick();
#ifndef BCC_BLOCK_IGNORE
			asm int 60h;
#endif
		}
		return;
	}

	if(!Lock::switch_on_demand)
		if(PCB::sleeping_threads && PCB::sleeping_threads->num_of_nodes)
				PCB::sleeping_threads->decrement();

	if(!Lock::switch_on_demand && !PCB::running->unlimited) cntr--;

	if(PCB::running->unlimited && PCB::running->finished) unlim = 1;

	if(cntr == 0 || Lock::switch_on_demand || unlim) {

#ifndef BCC_BLOCK_IGNORE
		asm{
			mov word ptr tsp, sp
			mov word ptr tss, ss
			mov word ptr tbp, bp
		}
#endif

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if (PCB::running->ready &&!PCB::running->blocked &&!PCB::running->finished && PCB::running!=Idle::idle_thread->getMyPCB())
				Scheduler::put((PCB*)PCB::running);

		if(PCB::running->finished){ //logicnije je da stoji u wrapperu
			PCB::num_of_active_threads--;
			PCB::all_threads->remove(PCB::running->thread_id); //sto?
		}

		PCB::running = Scheduler::get();

		if(!PCB::running)
			PCB::running = Idle::idle_thread->getMyPCB();

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		if(PCB::running->unlimited) cntr = -1;
		else cntr = PCB::running->quantum;

#ifndef BCC_BLOCK_IGNORE
		asm{
			mov sp, word ptr tsp
			mov ss, word ptr tss
			mov bp, word ptr tbp
		}
#endif
	}


	if(!Lock::switch_on_demand){
		tick();
#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
#endif
	}

	Lock::switch_on_demand = 0;
	unlim = 0;
}

void dispatch(){
#ifndef BCC_BLOCK_IGNORE
	asm cli
	Lock::lock_flag = 0;
	Lock::switch_on_demand = 1;
	asm int 08h;
	asm sti
#endif
}
