#include "semaphor.h"
#include "krnlsem.h"
#include "lock.h"

Semaphore::Semaphore(int init){
	myImpl = new KernelSem(init, this);
}

Semaphore::~Semaphore(){
	delete myImpl;
}

int Semaphore::val() const { return myImpl->val;}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal(){
	myImpl->signal();
}


