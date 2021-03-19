#ifndef DEFINE_H_
#define DEFINE_H_

 #include "IVTEntry.h"

extern volatile int lockFlag;
extern volatile int zahtevana_promena_konteksta;

#define lock asm { pushf; cli; }
#define unlock asm { popf; }

#define lockCout lockFlag = 0;
#define unlockCout lockFlag = 1;\
if(zahtevana_promena_konteksta){\
	dispatch();\
}\


#endif
