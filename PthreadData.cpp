#include "PthreadData.h"

PthreadData::PthreadData()
: fThreadId()
, fValidPThread(false)
, fRunnable(nullptr)
{
	pthread_attr_init(&fAttr);
	pthread_attr_setdetachstate(&fAttr, PTHREAD_CREATE_JOINABLE);
	pthread_mutex_init(&fMutex, nullptr);
}

void PthreadData::setRunnable(Runnable *r) {
	pthread_mutex_lock(&fMutex);
	fRunnable = r;
	pthread_mutex_unlock(&fMutex);
}

void PthreadData::resetPthreadData() {
	fRunnable = nullptr;
	fValidPThread = false;
	fStarted.reset();
	fBlockMain.reset();
}

PthreadData::~PthreadData() {
	pthread_attr_destroy(&fAttr);
	pthread_mutex_destroy(&fMutex);
}