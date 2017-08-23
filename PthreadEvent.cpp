#include "PthreadEvent.h"

PthreadEvent::PthreadEvent(): fMeetCondition(false) {
	pthread_mutex_init(&fCondMutex, nullptr);
	pthread_cond_init(&fCondition, nullptr);
}

PthreadEvent::~PthreadEvent() {
	pthread_mutex_destroy(&fCondMutex);
	pthread_cond_destroy(&fCondition);
}

void PthreadEvent::trigger() {
	pthread_mutex_lock(&fCondMutex);
	fMeetCondition = true;
	/* the routine is used to signal (or wake up) another thread which is waiting on the condition variable*/
	pthread_cond_signal(&fCondition);
	pthread_mutex_unlock(&fCondMutex);
}

void PthreadEvent::wait() {
	pthread_mutex_lock(&fCondMutex);
	/* Recommendation: Using a WHILE loop instead of an IF statement to check the waited for condition
	* can help deal with several potential problems, such as:
	* 1. If several threads are waiting for the same wake up signal, they will take turns acquiring the mutex,
	*    and any one of them can then modify the condition they all waited for.
	* 2. If the thread received the signal in error due to a program bug.
	* 3. The Pthreads library is permitted to issue spurious wake ups to a waiting thread without violating
	*    the standard.*/
	while (!fMeetCondition) {
		/* the api will do the following flows:
		* 1. do a Atomic operation including unlock mutex and hang current thread.
		* 2. receive a signal and try to get mutex to wake up current thread */
		pthread_cond_wait(&fCondition, &fCondMutex);
	}
	pthread_mutex_unlock(&fCondMutex);
}

bool PthreadEvent::isTriggered() {
	bool curFlag;
	pthread_mutex_lock(&fCondMutex);
	curFlag = fMeetCondition;
	pthread_mutex_unlock(&fCondMutex);
	return curFlag;
}

void PthreadEvent::reset() {
	pthread_mutex_lock(&fCondMutex);
	fMeetCondition = false;
	pthread_mutex_unlock(&fCondMutex);
}