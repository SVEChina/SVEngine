#include "SVLockSpin.h"

SVLockSpin::SVLockSpin(){
    //pthread_spin_init(&mutex, nullptr);
}

SVLockSpin::~SVLockSpin(){
    //pthread_spin_destroy(&mutex);
}

void SVLockSpin::lock() {
    //pthread_spin_lock(&mutex);
}

void SVLockSpin::unlock() {
    //pthread_spin_unlock(&mutex);
}

int SVLockSpin::trylock() {
    //return pthread_spin_trylock(&mutex);
    return 0;
}
