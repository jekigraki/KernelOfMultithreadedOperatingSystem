#ifndef _SEMSLEEP_H_
#define _SEMSLEEP_H_

#include "pcb.h"
#include "semaphor.h"
#include "schedule.h"


class PCB;
class KernelSem;

class sleepElem{
public:
	PCB *pcb;
	Time time;
	int infinityTime;
	sleepElem *next;

	sleepElem(PCB *pcb, Time time, int i)
	{
		this->pcb = pcb;
		this->time = time;
		this->infinityTime = i;
		this->next = 0;
	}

	~sleepElem(){}
};


class SleepList
{
private:
	sleepElem *head, *tail;

public:
	SleepList(KernelSem* k);
	~SleepList();

	void block(PCB *pcb, Time time);
	void decrement();
	int deblock(int n);
	sleepElem* getHead();

	KernelSem *myKSem;
};


#endif
