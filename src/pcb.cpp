#include "SCHEDULE.H"
#include "thread.h"
#include "pcb.h"
#include "idle.h"
#include "timer.h"
#include "define.h"
#include <iostream.h>
#include <dos.h>
#include "queue.h"


const int PCB::newThread = 0;
const int PCB::ready = 1;
const int PCB::finished = 2;
const int PCB::blocked = 3;
const int PCB::runningThread = 4;

volatile int PCB::posID = 0;
volatile unsigned PCB::counter = 0;
volatile PCB* PCB::running = 0;
PCB* PCB::idlePCB = 0;
Queue* PCB::pcbs = new Queue();
//PCB* PCB::userMainPCB = 0;
//PCB* PCB::mainPCB = 0;




PCB::PCB(Thread* myThread,StackSize stackSize,Time timeSlice) //mainThread??
{
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->x = 0; //da li je isteklo vreme spavanja?
	this->state = newThread;
	this->id = posID++;
	if(this->id == 0)
	{

		PCB::idlePCB = this;
		this->state = PCB::ready;
	}

	pcbs->put((PCB*)this);

	this->myThread = myThread;
	this->timeSlice = timeSlice;
	this->remainingTime = timeSlice;

	this->stackSize = stackSize/sizeof(unsigned);
	stack = new unsigned[this->stackSize];
/*
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize–1] = FP_SEG(myThread);
	stack[stackSize –2] = FP_OFF(myThread);
#endif
	stack[stackSize –5] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize –6] = FP_SEF(Thread::wrapper);
	stack[stackSize –7] = FP_OFF(Thread::wrapper);
#endif
#ifndef BCC_BLOCK_IGNORE
	ss= FP_SEG(stack + stackSize -16);
	sp= FP_OFF(stack + stackSize -16);
#endif
*/
	stack[this->stackSize-1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[this->stackSize-2] = FP_SEG(PCB::wrapper);
	stack[this->stackSize-3] = FP_OFF(PCB::wrapper);
#endif
#ifndef BCC_BLOCK_IGNORE
	sp = FP_OFF(stack + this->stackSize - 12);						//kako bre 12 ????????
	ss = FP_SEG(stack + this->stackSize - 12);
#endif
	//stack[stackSize - 12] = 0;
	bp=sp;
	if(this->id != 0)
	{
		counter++;
	}
//	cout<<"\n";cout<<counter;cout<<"\n";
	waitToCompleteList = new Queue();

#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
}

PCB::PCB()
{
	id = -1;
	ss = sp = bp = 0;
	stack = 0;
	stackSize = defaultStackSize;
	myThread = 0;
	timeSlice = remainingTime = defaultTimeSlice;
	state = 0;
	waitToCompleteList = 0;
	x = 0;
}

void PCB::waitToComplete() //main thread ovo ono, jel treba??
{
	if(this && (this != PCB::running) && (this->state != PCB::newThread) && (this->state != PCB::finished) && (this->id != 0) && (PCB::running != PCB::idlePCB))
	{
#ifndef BCC_BLOCK_IGNORE
	lock
#endif

		running->state = PCB::blocked;
		this->waitToCompleteList->put((PCB*)running);

#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
		dispatch();
	}
}


void PCB::wrapper() //promeni algoritam malo! ; main thread?
{
	PCB::running->myThread->run();
	PCB::running->state = PCB::runningThread;

#ifndef BCC_BLOCK_IGNORE
	lock
#endif

	PCB *temp = PCB::running->waitToCompleteList->removeFirst();
	while(temp)
	{
		temp->state = PCB::ready;
		Scheduler::put(temp);
		temp = PCB::running->waitToCompleteList->removeFirst();

	}
	delete temp;
	PCB::running->state = PCB::finished;
	counter--;

#ifndef BCC_BLOCK_IGNORE
	unlock
#endif

	dispatch();
}


PCB::~PCB()
{
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif

    myThread=0;
	delete waitToCompleteList;
	delete stack;

#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}



void PCB::setIstekloVremeSpavanja(int x)
{
	this->x = x;
}

int PCB::getIstekloVremeSpavanja()
{
	return x;
}









