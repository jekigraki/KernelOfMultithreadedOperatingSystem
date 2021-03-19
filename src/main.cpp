#include "timer.h"
#include "thread.h"
#include "idle.h"
#include "pcb.h"
#include <iostream.h>
#include "mainT.h"
#include "semSleep.h"
//#include "sleepQ.h"
#include "semaphor.h"

//#include "sleepQ.h"
//volatile int counter=0;
PCB* mainPCB;

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	Idle* IdleThread = new Idle();
	mainT *userMainThread = new mainT(argc,argv);
//	userMainThread->start();
	mainPCB = new PCB();
	PCB::running = mainPCB;


	inic();

//	dispatch();

	int ret = userMain(argc, argv);

	delete IdleThread;
	delete userMainThread;

	restore();

	cout<<"\nMain finished! ";
	int x;
	cin>>x;

	return ret;
}



