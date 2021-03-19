#include "semSleep.h"
#include "define.h"
#include "SCHEDULE.H"
#include "kernSem.h"
#include "pcb.h"


SleepList::SleepList(KernelSem* k)
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	head = tail = 0;
	myKSem= k;
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
}


SleepList::~SleepList()

{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	sleepElem *old;
	while(head)
	{
		old = head;
		head = head->next;
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;

		delete old;
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
	}

#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
}


void SleepList::block(PCB *pcb, Time time)
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	int infinityTime = 0;
	if(time == 0) infinityTime = 1;

	sleepElem *novi = new sleepElem(pcb, time, infinityTime);

	if(!head) //prvi element u listi
	{
		head = tail = novi;
	}
	else //lista vec postoji
	{
		if(novi->infinityTime == 0) //ako postoji vreme cekanja
		{
			sleepElem *temp = head;
			if(novi->time < temp->time || temp->infinityTime == 1) //ako stavljamo element na pocetak
			{
				if(temp->infinityTime != 1)	temp->time -= novi->time;
				novi->next = temp;
				head = novi;
				return;
			}
			else //ako stavljamo element u sredinu ili na kraj
			{
				sleepElem *prev = temp;
				novi->time -= temp->time;
				temp = temp->next;
				while(temp)
				{
					if(novi->time >= temp->time && temp->infinityTime != 1) //jos nismo nasli gde da stavimo element, samo iteriramo
					{
						novi->time -= temp->time;
						prev = temp;
						temp = temp->next;
					}
					else if(novi->time >= temp->time && temp->infinityTime == 1) //element treba da ide pre elementa sa beskonacnim vremenom
					{
						if(prev == temp) //nema jos nijednom elementa sa konacnim vremenom
						{
							novi->next = temp;
							head = novi;
							return;
						}
						else //izmedju elementa sa konacnim i elementa sa beskonacnim vremenom
						{
							prev->next = novi;
							novi->next = temp;
							return;
						}
					}
					else //izmedju dva sa konacnim vremenom
					{
						prev->next = novi;
						novi->next = temp;
						temp->time -= novi->time;
						return;
					}
				}
				prev->next = novi; //nema elemenata sa beskonacnim, ide na kraj liste
				tail = novi;
				return;
			}
		}
		else //ako je vreme cekanja beskonacno
		{
			tail->next = novi;
			tail = novi;
			return;
		}

	} //zatvara else granu, ako head vec postoji

#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;

}//zatvara block()


void SleepList::decrement() //da li treba da uvecavam value?
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	if(head && head->infinityTime != 1)
	{
		head->time--;
		while(head && head->time == 0 && head->infinityTime != 1)
		{
			sleepElem *temp = head;
			head = head->next;
			temp->pcb->state = PCB::ready;
			temp->pcb->setIstekloVremeSpavanja(1);
#ifndef BCC_BLOCK_IGNORE
		lock;
		lockCout;
#endif;
			Scheduler::put(temp->pcb);
			myKSem->value++;
#ifndef BCC_BLOCK_IGNORE
		unlock;
		unlockCout;
#endif;
		//	temp->pcb = 0; //vidi da li radi i bez ovoga
			delete temp;
		}
	}
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;

}



int SleepList::deblock(int n) //sta ako ima vise niti sa vremenom 0, nego sto treba da se probude [n]??
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
	//int sum = 0;
	int cnt = 0;
	while(head && (n > 0))
	{
		sleepElem *temp = head;
		//sum += temp->time;

		temp->pcb->state = PCB::ready;
		temp->pcb->setIstekloVremeSpavanja(0);
		Scheduler::put(temp->pcb);
		cnt++;
		n--;

		head = head->next;
		delete temp;
	}
	//if(head && head->infinityTime != 1 && head->time == 0) head->time = sum;

#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
	return cnt;
}



sleepElem* SleepList::getHead()
{
#ifndef BCC_BLOCK_IGNORE
		lock;
#endif;
		return head;
#ifndef BCC_BLOCK_IGNORE
		unlock;
#endif;
}

