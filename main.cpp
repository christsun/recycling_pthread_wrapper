extern "C" {
#include <stdio.h>
#include <pthread.h>
}
#include "APthread.h"

#define TOSTRING(x) (#x)



void job(void*) {
	printf("%s debug: thread[%lu] doing job...\n", __FUNCTION__, pthread_self());
}

class myjob: public Runnable {
public:
	myjob() { c = count++; }
	void run() {
		printf("%s debug: thread[%lu] doing job", __FUNCTION__, pthread_self());
		printf("[%u]\n", c);
	}
	static int count;
private:
	int c;
};
int myjob::count = 0;
int main(){
#if 0
	myjob mj;
	printf("main[%lu] new a apthread...\n", pthread_self());
	APthread *aptread = new APthread(mj);
	printf("main[%lu] start apthread..\n", pthread_self());
	aptread->start();
	printf("main[%lu] wait for job done.\n", pthread_self());
	aptread->join();
	printf("main[%lu] finished job.\n", pthread_self());
	delete aptread;
#endif
	myjob mj[5];
	int i = 0;
	printf("main[%lu] new a apthread...\n", pthread_self());
	APthread *apthread = new APthread();
	apthread->createPthread();
	while (i < 5) {
		apthread->setRunnable(&mj[i]);
		apthread->start();
		apthread->waitUtilRunnableDone();
		i++;
	}
	
	
	//apthread->setRunnable(nullptr);
	//apthread->start();
	//apthread->join();
	delete apthread;
	printf(" myjob::count = %d.\n", myjob::count);
	return 0;
}