#ifndef _PTHREAD_WRAPPER_APTHREAD_H_
#define _PTHREAD_WRAPPER_APTHREAD_H_
#include "Runnable.h"

/*define a posix thread*/
class APthread {
public:
	enum status {
		init_status,
		idle_status,
		loaded_status,
		start_status,
		suspend_status,//phase out
		stop_status,
		release_status
	};
	static const char *curStatus(enum status cs) {
		static const char *status[7] = { "init", "idle", "loaded", "start", "suspend", "stop",
			"release" };
		switch (cs) {
		case init_status: return status[0];
		case idle_status: return status[1];
		case loaded_status: return status[2];
		case start_status: return status[3];
		case suspend_status: return status[4];
		case stop_status: return status[5];
		case release_status: return status[6];
		}
		return "invalid status";
	}

	APthread();
	void createPthread();
	void setRunnable(Runnable *r);
	void waitUtilRunnableDone();
	/*stop pthread before destructor be invoked.*/
	void stop();

	/*Non-virtual, no subclass.*/
	~APthread();
	/*Starts the thread. Returns false if the thread could not be started.*/
	bool start();
	/*Waits for the thread to finish.
     * If the thread has not started, returns immediately.*/
	void join();
private:
	void *fData;
	enum status fAPthreadStatus;
};
#endif