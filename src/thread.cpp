#include "SCHEDULE.H"
#include "thread.h"
#include "pcb.h"
#include "timer.h"
#include "define.h"
#include "idle.h"
#include "queue.h"
#include <dos.h>
#include <iostream.h>

Thread::Thread(StackSize stackSize, Time timeSlice)
{
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	myPCB = new PCB(this, stackSize, timeSlice);
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}


Thread::~Thread()
{
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif

	if(this->myPCB->id != 0)
	{
		if(this->myPCB->state != PCB::finished) this->waitToComplete();
		myPCB->~PCB();
		myPCB = 0;
	}

#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif
}

ID Thread::getId()
{
	return myPCB->id;
}


ID Thread::getRunningId()
{
	return PCB::running->id;
}


Thread* Thread::getThreadById(ID id)
{
	Elem* temp = PCB::pcbs->getHead();
	while(temp)
	{
		if(temp->pcb->id == id) return temp->pcb->myThread;
		temp = temp->next;
	}
	return 0;
}


void Thread::start()   // POGLEDAJ KOMENTAR;neki lock-ovi mozda da se srede
{
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	//cout<<"startovana nit\n";
	if((myPCB != PCB::idlePCB) && (myPCB->state == PCB::newThread))
	{
		myPCB->state = PCB::ready;
		Scheduler::put(myPCB);

		//PCB::pcbs->put(this->myPCB);
	}

#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}


void Thread::waitToComplete() //da li da ovde bude implementacija ove metode
{
	myPCB->waitToComplete();
}

void dispatch()
{
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	//cout<<"\npozvan dispatch\n";
	zahtevana_promena_konteksta = 1;
	timer();

#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void Thread::run(){}
