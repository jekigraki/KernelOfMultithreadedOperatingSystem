#ifndef _KERNEV_H_
#define _KERNEV_H_

#include "event.h"
#include "pcb.h"

class PCB;


class KernelEv {
public:
	KernelEv (IVTNo ivtNo, Event* myEvent);
	~KernelEv ();

	void wait ();
	void signal();

	IVTNo ivtEntry;
	Event* myEvent;
    volatile int value;
	PCB* myPCB;


protected:
	friend class Event;

};


#endif
