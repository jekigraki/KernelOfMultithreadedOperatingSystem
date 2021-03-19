#ifndef MAINT_H_
#define MAINT_H_


#include"thread.h"

class mainT: public Thread {
public:
	mainT(int argc, char* argv[]);
	virtual void run();

private:
	int argc;
	char **argv;
};


#endif
