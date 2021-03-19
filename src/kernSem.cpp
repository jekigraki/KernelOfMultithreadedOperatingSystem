
#include "define.h"
#include "kernSem.h"
#include "pcb.h"
#include "queueSem.h"
//#include "listKSem.h"

#include "schedule.h"
#include "semSleep.h"
//#include "sleepQ.h"



allSems* KernelSem::sems = new allSems();


KernelSem::KernelSem(Semaphore *s, int init)
{
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	mySem = s;
	value = init;
	sleepLst = new SleepList(this);
	KernelSem::sems->put(this);

#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

KernelSem::~KernelSem(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	value = 0;


	//sleepLst->remove(sleepLst->getCnt());

	delete sleepLst;

	//sems->remove(this);
	mySem = 0;



#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

int KernelSem::getVal() {
	return value;
}


int KernelSem::wait (Time maxTimeToWait)
{
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	if(--value < 0)
	{
		PCB::running->state = PCB::blocked;
		sleepLst->block((PCB*)PCB::running, maxTimeToWait);

#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
		dispatch();
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;


		int b=((PCB*)PCB::running)->getIstekloVremeSpavanja();
		if(b == 1)
		{
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif
			return 0;
		}
		else
		{
			return 1;
		}

	}else
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
	return 1;
}

int KernelSem::signal(int n)
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif
		int ret = 0;
		if(n==0)
		{
			if(value++ <= 0) ret = sleepLst->deblock(1);  // sleepLst->remove(1);
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif
			return ret;
		}
		else if(n > 0)
		{
				value += n;
				ret = sleepLst->deblock(n);// sleepLst->remove(n);
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif
				return ret;
		}
		else
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif
			return n;
}


