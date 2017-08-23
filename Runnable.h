#ifndef _PTHREAD_WRAPPER_RUNNABLE_H_
#define _PTHREAD_WRAPPER_RUNNABLE_H_

class Runnable
{
public: 
	Runnable(){}
	virtual ~Runnable(){}
	virtual void run() {}
};


#endif