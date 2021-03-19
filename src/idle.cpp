#include "idle.h"
#include "define.h"
#include <iostream.h>

Idle::Idle():Thread(defaultStackSize,defaultTimeSlice){}

Idle::~Idle(){}

void Idle::run(){
	while(1){

		for(int k = 0; k < 10; k++){
			for(int i = 0; i < 10000; i++)
				for(int j = 0; j < 10000; j++);
		}
	//lockCout
	//cout<<"idle";
	//unlockCout

	}
}
