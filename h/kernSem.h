#ifndef _KERNELSEM_H_
#define _KERNELSEM_H_

#include "kernSem.h"
#include "semaphor.h"
#include "thread.h"
#include "queueSem.h"
#include "semSleep.h"
//#include "sleepQ.h"
//#include "listKSem.h"


class allSems;
//class SleepList;
class SleepQueue;

class KernelSem {
public:
	KernelSem (Semaphore *sem, int init);
    ~KernelSem ();

	 int wait (Time maxTimeToWait);
	 int signal(int n);

	int getVal();

	static allSems *sems;
	//static ListKSem *sems;

	int value;
	Semaphore* mySem;

	SleepList *sleepLst;
	//SleepQueue* sleepLst;


};

#endif
