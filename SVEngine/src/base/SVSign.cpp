#include "SVSign.h"
#include "SVLock.h"

//
SVSignLock::SVSignLock(){
    m_pLock = MakeSharedPtr<SVLock>();
    pthread_cond_init(&m_cond, nullptr);
}

SVSignLock::~SVSignLock(){
    pthread_cond_destroy(&m_cond);
    m_pLock = nullptr;
}

void SVSignLock::lock(){
    m_pLock->lock();
}

void SVSignLock::unlock(){
    m_pLock->unlock();
}

void SVSignLock::wait(){
    pthread_cond_wait(&m_cond, &m_pLock->mutex);
}

void SVSignLock::notice(){
    pthread_cond_signal(&m_cond);
}

//
SVSignLockBroadcast::SVSignLockBroadcast(){
}

void SVSignLockBroadcast::wait(){
    SVSignLock::wait();
}

void SVSignLockBroadcast::notice(){
    pthread_cond_broadcast(&m_cond);
}
