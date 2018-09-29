#include "SVLock.h"

SVLock::SVLock(){
    pthread_mutex_init(&mutex, nullptr);
}

SVLock::~SVLock(){
    pthread_mutex_destroy(&mutex);
}

void SVLock::lock() {
    pthread_mutex_lock(&mutex);
}

void SVLock::unlock() {
    pthread_mutex_unlock(&mutex);
}

int SVLock::trylock() {
    return pthread_mutex_trylock(&mutex);
}
