#include "SVTdCore.h"
#include <fcntl.h>
#include <cerrno>

SVLockSpin::SVLockSpin(){
}

SVLockSpin::~SVLockSpin(){
}

void SVLockSpin::lock() {
    while (mflag.test_and_set());
}

void SVLockSpin::unlock() {
    mflag.clear();
}

//
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


//
SVCond::SVCond(){
    pthread_mutex_init(&m_mutex, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}

SVCond::~SVCond(){
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

void SVCond::lock(){
     pthread_mutex_lock(&m_mutex);
}

void SVCond::unlock(){
    pthread_mutex_unlock(&m_mutex);
}

void SVCond::wait(){
    pthread_cond_wait(&m_cond, &m_mutex);
}

void SVCond::notice(){
    pthread_cond_signal(&m_cond);
}

//
SVCondBroadcast::SVCondBroadcast(){
    pthread_mutex_init(&m_mutex, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}

SVCondBroadcast::~SVCondBroadcast(){
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

void SVCondBroadcast::wait(){
    pthread_cond_wait(&m_cond, &m_mutex);
}

void SVCondBroadcast::notice(){
    pthread_cond_broadcast(&m_cond);
}

//
SVSem::SVSem(s32 _resvalue){
    sem_init(&psem, 0, _resvalue);
}

SVSem::~SVSem(){
    sem_destroy(&psem);
}

void SVSem::wait(){
    sem_wait(&psem);
}

void SVSem::post(){
    sem_post(&psem);
}

void SVSem::lastcheck() {
    s32 t_value = 0;
    sem_getvalue(&psem, &t_value);
    if(t_value == -1) {
        post();
    }
}
