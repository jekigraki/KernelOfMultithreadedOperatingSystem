#include <iostream.h>
#include "schedule.h"
#include "pcb.h"
#include "thread.h"
#include "define.h"
#include "timer.h"
#include "kernSem.h"
#include "kernEv.h"
#include "IVTEntry.h"

KernelEv::KernelEv (IVTNo ivtNo, Event* myEvent){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	ivtEntry = ivtNo;
	this->myEvent = myEvent;
	value = 0;
	myPCB = (PCB*)PCB::running;
	IVTEntry::IVTable[ivtEntry]->kernelEvent = this;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

KernelEv::~KernelEv (){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	 myPCB=0;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}


void KernelEv::wait (){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if((PCB*)PCB::running == myPCB){
		if(value == 0){
			value = 1;
			PCB::running->state = PCB::blocked;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
			dispatch();
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
		}

	}
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void KernelEv::signal(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(value == 1){
		value = 0;
		myPCB->state = PCB::ready;
		Scheduler::put(myPCB);
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
		dispatch();
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	}
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}
