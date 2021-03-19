#ifndef _event_h_
#define _event_h_

#include "IVTEntry.h"

#define PREPAREENTRY(num, callold)\
void interrupt inter##num(...);\
IVTEntry ivtEntry##num(num, inter##num);\
void interrupt inter##num(...){\
	if (IVTEntry::IVTable[num])\
		IVTEntry::IVTable[num]->signal();\
	if (callold && IVTEntry::IVTable[num])\
		IVTEntry::IVTable[num]->callOldRoutine();\
}\


typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	 Event (IVTNo ivtNo);
	 ~Event ();

	 void wait ();

protected:
	 friend class KernelEv;
	 void signal(); // can call KernelEv

private:
	 KernelEv* myImpl;
};
#endif

