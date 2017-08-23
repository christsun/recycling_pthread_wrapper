#include "APthread.h"
#include "PthreadData.h"
extern "C"{
#include <stdio.h>
}

static void *pthreadCommonEntry(void *args) {
	PthreadData *pthreadData = static_cast<PthreadData *>(args);
	while (true) {
		printf("%s: pthread[%lu] wait for start signal...\n", __FUNCTION__, pthreadData->fThreadId);
		// Wait for start signal
		pthreadData->fStarted.wait();
		if (pthreadData->fRunnable != nullptr) {
			printf("%s: pthread[%lu] got start signal and run task...\n", __FUNCTION__, pthreadData->fThreadId);
			pthreadData->fRunnable->run();
		} else {
			printf("%s: pthread[%lu] invalid runnable, exit pthread\n", __FUNCTION__, pthreadData->fThreadId);
			break;
		}

		printf("%s: pthread[%lu] task finished and trigger main.\n", __FUNCTION__, pthreadData->fThreadId);
		pthreadData->fBlockMain.trigger();
		pthreadData->fStarted.reset();
	}
	printf("%s: pthread[%lu] exit.\n", __FUNCTION__, pthreadData->fThreadId);
	pthreadData->resetPthreadData();
	return nullptr;
}

APthread::APthread() {
	PthreadData *pthreadData = new PthreadData();
	if (!pthreadData)
		printf("%s error: new PthreadData failed.\n", __FUNCTION__);
	fData = static_cast<void *>(pthreadData);
	fAPthreadStatus = init_status;
	printf("current status %s -> after %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
}

void APthread::createPthread() {
	if (fAPthreadStatus != init_status) {
		printf("current status %s -> Cannot %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
		return;
	}
	PthreadData *pthreadData = static_cast<PthreadData *>(fData);
	int ret = pthread_create(&pthreadData->fThreadId,
							&pthreadData->fAttr,
							pthreadCommonEntry,
							pthreadData);
	pthreadData->fValidPThread = (ret == 0);
	fAPthreadStatus = idle_status;
	printf("current status %s -> after %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
}

void APthread::setRunnable(Runnable *r) {
	if (fAPthreadStatus != idle_status) {
		printf("current status %s -> Cannot %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
		return;
	}
	PthreadData *pthreadData = static_cast<PthreadData *>(fData);
	pthreadData->setRunnable(r);
	fAPthreadStatus = loaded_status;
	printf("current status %s -> after %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
}

bool APthread::start() {
	PthreadData *pthreadData = static_cast<PthreadData *>(fData);
#if 0
	/*must be a valid pthread before start*/
	if (!pthreadData->fValidPThread) {
		printf("%s error: invalid pthread.\n", __FUNCTION__);
		return false;
	}
	/*avoid double trigger*/
	if (pthreadData->fStarted.isTriggered()) {
		printf("%s error: double trigger or resume error.\n", __FUNCTION__);
		return false;
	}
#endif
	if (fAPthreadStatus != loaded_status) {
		printf("current status %s -> Cannot %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
		return false;
	}

	pthreadData->fStarted.trigger();
	fAPthreadStatus = start_status;
	printf("current status %s -> after %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
	return true;
}

void APthread::waitUtilRunnableDone() {
	if (fAPthreadStatus != start_status) {
		printf("current status %s -> Cannot %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
		return;
	}

	PthreadData *pthreadData = static_cast<PthreadData *>(fData);
	pthreadData->fBlockMain.wait();
	fAPthreadStatus = idle_status;
	printf("current status %s -> after %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
	pthreadData->fBlockMain.reset();
}


void APthread::stop() {
	if (fAPthreadStatus == start_status) {
		waitUtilRunnableDone();
		setRunnable(nullptr);
		start();
		join();
		fAPthreadStatus = stop_status;
		return;
	}
	if (fAPthreadStatus == idle_status || fAPthreadStatus == loaded_status) {
		setRunnable(nullptr);
		start();
		join();
		fAPthreadStatus = stop_status;
	}
	fAPthreadStatus = stop_status;
	printf("current status %s -> after %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
}

void APthread::join() {
	PthreadData *pthreadData = static_cast<PthreadData *>(fData);
	if (!pthreadData->fValidPThread)
		return;
	printf("%s: join pthread[%lu]...\n", __FUNCTION__, pthreadData->fThreadId);
	pthread_join(pthreadData->fThreadId, nullptr);
}

APthread::~APthread() {
	if (fData) {
		PthreadData *pthreadData = static_cast<PthreadData *>(fData);
#if 0
		// If created thread but start was never called, kill the thread.
		if (pthreadData->fValidPThread && !pthreadData->fStarted.isTriggered()) {
			pthreadData->fCanceled.trigger();
			if (this->start()) {
				this->join();
			}
		}
#endif
		stop();
		fAPthreadStatus = release_status;
		printf("current status %s -> after %s.\n", curStatus(fAPthreadStatus), __FUNCTION__);
		delete pthreadData;
	}
}