#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "pcb.h"

class PCB;

class Elem
{
public:
	PCB *pcb;
	Elem *next;

	Elem(PCB *pcb)
	{
		this->pcb = pcb;
		this->next = 0;
	}

	~Elem(){}
};


class Queue
{
private:
	Elem *head, *tail;

public:
	Queue();
	~Queue();
	void put(PCB *pcb);
	PCB* removeFirst();
	Elem* getHead();

};


#endif
