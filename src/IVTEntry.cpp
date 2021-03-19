#include "IVTEntry.h"
#include <DOS.H>
#include "kernEv.h"
#include "define.h"


IVTEntry* IVTEntry::IVTable[256] = {0};

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newRoutine){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
	this->ivtNo = ivtNo;
	this->newRoutine = newRoutine;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
#endif
	kernelEvent = 0;
	IVTable[ivtNo] = this;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

IVTEntry::~IVTEntry(){
#ifndef BCC_BLOCK_IGNORE
	lock;
#endif
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldRoutine);
#endif
	IVTable[ivtNo] = 0;
	kernelEvent = 0;
#ifndef BCC_BLOCK_IGNORE
	unlock;
#endif
}

void IVTEntry::signal(){
	kernelEvent->signal();
}


void IVTEntry::callOldRoutine(){
#ifndef  BCC_BLOCK_IGNORE
	lock;
#endif
	oldRoutine();
#ifndef  BCC_BLOCK_IGNORE
	unlock;
#endif
}



