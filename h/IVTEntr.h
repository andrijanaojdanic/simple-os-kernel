#ifndef _IVTENTR_H_
#define _IVTENTR_H_

typedef void interrupt (*pInterrupt)(...);

class KernelEv;

class IVTEntry{

public:
	static IVTEntry* all_entries[256];
	static IVTEntry* get_ivt_ptr(int entry_num);

	KernelEv* my_kernel_ev;
	pInterrupt old_routine;
	pInterrupt new_routine;
	unsigned char ivt_no;

	IVTEntry(unsigned char ivtNo, pInterrupt newRout);
	~IVTEntry();

	void call_old_routine();
	void signal();
};

#endif
