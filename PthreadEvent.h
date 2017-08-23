#ifndef _PTHREAD_WRAPPER_PTHREADEVENT_H_
#define _PTHREAD_WRAPPER_PTHREADEVENT_H_
extern "C" {
#include "pthread.h"
}
/*a control class.
 *trigger a thread to run or block a thread on waiting*/
class PthreadEvent {
public:
	PthreadEvent();
	~PthreadEvent();
	void trigger();
	void wait();
	bool isTriggered();
	void reset();
private:
	pthread_cond_t fCondition;
	pthread_mutex_t fCondMutex;
	bool fMeetCondition;
};
#endif