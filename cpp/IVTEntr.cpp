#include "IVTEntr.h"
#include "dos.h"
#include "lock.h"
#include "thread.h"
#include "KernelEv.h"

IVTEntry* IVTEntry::all_entries[256];

IVTEntry::IVTEntry(unsigned char ivtNo, pInterrupt newRout){
	Lock::lock_flag = 1;
	old_routine = 0;
	ivt_no = ivtNo;
	new_routine = newRout;
	my_kernel_ev = 0;
	all_entries[ivtNo] = this;
#ifndef BCC_BLOCK_IGNORE
	lock
	old_routine = getvect(ivtNo);
	setvect(ivtNo,new_routine);
	unlock
#endif
	Lock::lock_flag = 0;
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock
	setvect(ivt_no, old_routine);
	unlock
#endif
	IVTEntry::all_entries[ivt_no] = 0;
}

IVTEntry* IVTEntry::get_ivt_ptr(int entry_num){
	if(all_entries[entry_num]) return all_entries[entry_num];
	else return 0;
}

void IVTEntry::signal(){
	if(my_kernel_ev) my_kernel_ev->signal();
}

void IVTEntry::call_old_routine(){
#ifndef BCC_BLOCK_IGNORE
	lock
	if (old_routine) (*old_routine)();
	unlock
#endif
}
