#include <iostream.h>
#include <dos.h>
#include "pcb.h"
#include "idle.h"
#include "timer.h"
#include "define.h"
#include "queueSem.h"
#include "semSleep.h"
#include "SCHEDULE.H"
#include "kernSem.h"


#include "IVTentry.h"


volatile int zahtevana_promena_konteksta = 0;
volatile unsigned tsp,tss,tbp;
volatile unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina
volatile int lockFlag = 1;

extern PCB* mainPCB;

class ElemSem;

void tick();

// postavlja novu prekidnu rutinu
void inic(){
#ifndef BCC_BLOCK_IGNORE
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
#endif
}

// vraca staru prekidnu rutinu
void restore(){    //ivt?
#ifndef BCC_BLOCK_IGNORE
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
#endif
}


void interrupt timer()
{
	if(zahtevana_promena_konteksta == 0)
	{
		if(PCB::running->timeSlice != 0) PCB::running->remainingTime--;

		ElemSem* temp= KernelSem::sems->head;
		while(temp)
		{
			temp->sem->sleepLst->decrement();
			temp = temp->next;
		}

		//KernelSem::sems->decrementTime();
		tick();

	}
	if(zahtevana_promena_konteksta || PCB::running->remainingTime==0)
	{
		if(lockFlag)
		{
			if(PCB::running->remainingTime == 0 && PCB::running->timeSlice != 0) PCB::running->remainingTime = PCB::running->timeSlice;
			zahtevana_promena_konteksta = 0;

#ifndef BCC_BLOCK_IGNORE
			asm{
				mov tss, ss
				mov tsp, sp
				mov tbp, bp
			}
#endif

			PCB::running->ss = tss;
			PCB::running->sp = tsp;
			PCB::running->bp = tbp;


			if (PCB::running->state == PCB::runningThread && PCB::running && PCB::running != PCB::idlePCB && PCB::running != mainPCB)
			{
				PCB::running->state = PCB::ready;
				Scheduler::put((PCB*)PCB::running);
			}

			PCB::running = Scheduler::get();

			if(PCB::running == 0)
			{
				PCB::running = PCB::idlePCB;
			}
			if(PCB::counter == 0)
			{
				PCB::running = mainPCB;
			}

			PCB::running->state = PCB::runningThread;

			tss = PCB::running->ss;
			tsp = PCB::running->sp;
			tbp = PCB::running->bp;

#ifndef BCC_BLOCK_IGNORE
			asm{
				mov ss, tss
				mov sp, tsp
				mov bp, tbp
			}
#endif
		}else
			zahtevana_promena_konteksta = 1;
	}
	if(!zahtevana_promena_konteksta)
	{
#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
#endif
	}

}















