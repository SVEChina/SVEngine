#include "SVSem.h"
#include <fcntl.h>

SVSem::SVSem(cptr8 _name,s32 _resvalue){
    m_semName = _name;
    m_pSem = sem_open(m_semName.c_str(),O_CREAT|O_EXCL,0644,_resvalue);
    if( m_pSem == SEM_FAILED ){
        switch( errno ){
            case EEXIST:{
                SV_LOG_INFO( "Semaphore with name '%s' already exists.\n", _name );
                sem_unlink(m_semName.c_str());
                m_pSem = sem_open(m_semName.c_str(),O_CREAT|O_EXCL,0644,_resvalue);
                break;
            }
            default:{
                sem_unlink(m_semName.c_str());
                printf( "Unhandled error: %d.\n", errno );
                break;
            }
        }
    }
    pthread_mutex_init(&mutex, nullptr);
}

SVSem::~SVSem(){
    int t_flag = sem_close(m_pSem);
    t_flag = sem_unlink(m_semName.c_str());
    pthread_mutex_destroy(&mutex);
}

void SVSem::wait(){
    pthread_mutex_lock(&mutex);
    sem_wait(m_pSem);
    pthread_mutex_unlock(&mutex);
}

void SVSem::post(){
    pthread_mutex_lock(&mutex);
    sem_post(m_pSem);
    pthread_mutex_unlock(&mutex);
}

void SVSem::showValue(){
    int t_value = 0;
    sem_getvalue(m_pSem, &t_value);
    SV_LOG_INFO("sem %s value %d \n",m_semName.c_str(),t_value);
}
