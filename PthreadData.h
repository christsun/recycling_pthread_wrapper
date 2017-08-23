#ifndef _PTHREAD_WRAPPER_PTHREADDATA_H_
#define _PTHREAD_WRAPPER_PTHREADDATA_H_
extern "C" {
#include "pthread.h"
}
#include "APthread.h"
#include "PthreadEvent.h"

/*save thread attribute and data*/
class PthreadData {
public:
	PthreadData();
	void setRunnable(Runnable *r);
	void resetPthreadData();
	~PthreadData();
	/*hold 2 event to judge thread status*/
	PthreadEvent fStarted;
	PthreadEvent fBlockMain;
	pthread_t fThreadId;
	bool fValidPThread;
	pthread_attr_t fAttr;
	Runnable *fRunnable;
private:
	pthread_mutex_t fMutex;
};
#endif