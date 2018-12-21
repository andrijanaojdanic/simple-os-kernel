#ifndef _EVENT_H_
#define _EVENT_H_

#include "IVTEntr.h";

typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(InterruptNo, call_old)\
	void interrupt inte##InterruptNo(...);\
	IVTEntry entry##InterruptNo(InterruptNo,inte##InterruptNo);\
	void interrupt inte##InterruptNo(...){\
		entry##InterruptNo.signal();\
		if(call_old)\
			entry##InterruptNo.call_old_routine();\
	}

typedef unsigned char IVTNo;
class KernelEv;

class Event{
public:
	Event(IVTNo ivtNo);
	~Event();

	void wait();

protected:
	friend class KernelEv;
	void signal();

private:
	KernelEv* myImpl;
};

#endif

