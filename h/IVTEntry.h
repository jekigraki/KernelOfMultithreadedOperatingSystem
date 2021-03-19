#ifndef _IVTEntry_H_
#define _IVTEntry_H_
#include "define.h"
#include <DOS.H>

typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class KernelEv;

class IVTEntry{
public:

		IVTEntry(IVTNo ivtNo, pInterrupt newRoutine);
		~IVTEntry();

		static IVTEntry *IVTable[256];

		void signal();
		void callOldRoutine();



		pInterrupt oldRoutine;
		pInterrupt newRoutine;
		IVTNo ivtNo;
		KernelEv *kernelEvent;
};

#endif
