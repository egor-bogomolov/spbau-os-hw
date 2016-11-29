#include "lock.h"

volatile int cnt_locks = 0; //one thread can use multiple locks

void lock(void) {
	disable_ints();
	cnt_locks++;
}

void unlock(void) {
	cnt_locks--;
	if (cnt_locks == 0) {
		enable_ints();
	}
}
