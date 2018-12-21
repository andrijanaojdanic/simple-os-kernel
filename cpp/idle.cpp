#include "idle.h"
#include "lock.h"
#include "thread.h"

void Idle::run(){
	while(1);
}

Idle::Idle():Thread(1024,1){
	PCB::id_creator--;
	Thread::getMyPCB()->thread_id = -1;
}

Idle::~Idle(){}

Idle* Idle::idle_thread;
