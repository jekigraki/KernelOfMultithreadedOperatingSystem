#include "mainT.h"
#include <IOSTREAM.H>

extern int userMain(int, char* argv[]);

mainT::mainT(int argcc, char *argvv[]):Thread(4096, 0), argc(argcc), argv(argvv) {}


void mainT::run() {
//	cout<<"main thread run";
    userMain(argc, argv);
}



