#ifndef _PCB_H_
#define _PCB_H_

#include "thread.h"
#include "queue.h"

class Queue;

class PCB {

public:
		PCB(Thread *myThread, StackSize stackSize, Time timeSlice);
		PCB();
		~PCB();

		unsigned* stack;
		unsigned ss;
		unsigned sp;
		unsigned bp;

		StackSize stackSize;
		Time timeSlice;
		Time remainingTime;

		volatile static unsigned counter;
		static const int newThread;
		static const int ready;
		static const int finished;
		static const int blocked;
		static const int runningThread;
		volatile int state;

		volatile static int posID;
		ID id;

		Thread *myThread;
		volatile static PCB *running;
		static PCB* idlePCB;

		/*static PCB* userMainPCB;
		static PCB* mainPCB;*/

		static Queue *pcbs;
		Queue *waitToCompleteList;



		int x;
		void setIstekloVremeSpavanja(int x);
		int getIstekloVremeSpavanja();

		void waitToComplete();
		static void wrapper();


};

#endif
