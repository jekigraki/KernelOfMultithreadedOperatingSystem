#include "queue.h"
#include "define.h"
#include "pcb.h"

Queue::Queue()
{
	head = tail = 0;
}

Queue::~Queue()
{
	Elem *old;
	while(head)
	{
		old = head;
		head = head->next;
		delete old;
	}
}

PCB* Queue::removeFirst()
{
	if(head)
	{
		Elem *temp;
		temp = head;
		head = head->next;

		return temp->pcb;
	}
	return 0;
}

Elem* Queue::getHead()
{
	return head;
}

void Queue::put(PCB* pcb)
{
	Elem *newone = new Elem(pcb);

	if(head == 0)
	{
		head = tail = newone;
	}
	else
	{
	/*	Elem *temp = head;
		while(temp)
		{
			if(temp->pcb == pcb)
				return;
			temp = temp->next;
		}*/
		tail->next = newone;
		tail = tail->next;
	}
}

