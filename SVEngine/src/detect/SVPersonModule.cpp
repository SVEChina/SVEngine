//
// SVPersonModule.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPersonModule.h"
#include "SVPerson.h"
#include "../basesys/SVConfig.h"
SVPersonModule::SVPersonModule(SVInst* _app)
:SVEventProc(_app){
    
}

SVPersonModule::~SVPersonModule() {
    
}

void SVPersonModule::initModule() {
    for (int i = 0; i < SUPPORT_MAX_PERSON_NUM; i++) {
        m_persons[i] = MakeSharedPtr<SVPerson>(mApp);
        m_persons[i]->setPersonID(i + 1);
        m_persons[i]->setDetectType(mApp->getConfig()->detectType);
        
        
    }
}

void SVPersonModule::destoryModule(){
    for (int i = 0; i < SUPPORT_MAX_PERSON_NUM; i++) {
        m_persons[i] = nullptr;
    }
}

SVPersonPtr SVPersonModule::getPerson(s32 _personId){
    for (s32 i = 0; i < SUPPORT_MAX_PERSON_NUM; i++) {
        if (m_persons[i]->getPersonID() == _personId)
            return m_persons[i];
    }
    return nullptr;
}

