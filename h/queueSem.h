#ifndef _QUEUESEM_H_
#define _QUEUESEM_H_

#include "kernSem.h"

class KernelSem;

class ElemSem{
public:
	KernelSem *sem;
	ElemSem *next;

	ElemSem(KernelSem *sem)
	{
		this->sem = sem;
		this->next = 0;
	}

	~ElemSem(){}
};


class allSems{


public:
	allSems();
	~allSems();
	void put(KernelSem *sem);
	//KernelSem* removeFirst();
	ElemSem* getHead();

//	void decrementTime();

//private:
	ElemSem *head, *tail;


};


#endif
