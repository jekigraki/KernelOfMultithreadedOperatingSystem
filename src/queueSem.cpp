#include "queueSem.h"
#include "define.h"
#include "kernSem.h"
#include "thread.h"
#include "semSleep.h"
//#include "sleepQ.h"

allSems::allSems()
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	head = tail = 0;
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
}

allSems::~allSems()
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	while(head)
	{
		ElemSem *old;
		old = head;
		head = head->next;
		delete old;
	}
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
}

/*
KernelSem* allSems::removeFirst()
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	if(head)
	{
		ElemSem *temp;
		temp = head;
		head = head->next;

		return temp->sem;
	}
	return 0;
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
}
*/

ElemSem* allSems::getHead()
{
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	return head;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void allSems::put(KernelSem* sem)
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	ElemSem *newone = new ElemSem(sem);

	if(head == 0)
	{
		head = tail = newone;
	}
	else
	{
		tail->next = newone;
		tail = tail->next;
	}
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
}

/*
void allSems::decrementTime(){
	ElemSem* curr = head;
	while(curr){
		curr->sem->sleepLst->decrement();
		curr=curr->next;
	}
}
*/
