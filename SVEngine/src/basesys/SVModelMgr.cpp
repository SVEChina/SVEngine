//
// SVModelMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModelMgr.h"
#include "../core/SVModel.h"
#include "../work/SVTdCore.h"
//
SVModelMgr::SVModelMgr(SVInst* _app)
:SVSysBase(_app){
    m_lock = MakeSharedPtr<SVLock>();
}

SVModelMgr::~SVModelMgr() {
    m_lock = nullptr;
}

void SVModelMgr::init(){
    
}

void SVModelMgr::update(f32 _dt){
    _recycleUnuseModel();
}

void SVModelMgr::destroy(){
    m_modelPool.destroy();
}

void SVModelMgr::addModel(SVModelPtr _model){
    if (_model) {
        if (!_hasModel(_model)) {
            m_lock->lock();
            m_modelPool.append(_model);
            m_lock->unlock();
        }
    }
}

SVModelPtr SVModelMgr::getModel(SVString _name){
    SVModelPtr model = nullptr;
    m_lock->lock();
    for (s32 i = 0; i<m_modelPool.size(); i++) {
        SVModelPtr t_model = m_modelPool[i];
        if (strcmp(t_model->getName(), _name.c_str()) == 0) {
            model = t_model;
            break;
        }
    }
    m_lock->unlock();
    return model;
}

bool SVModelMgr::_hasModel(SVModelPtr _model){
    bool t_res = false;
    m_lock->lock();
    for (s32 i = 0; i<m_modelPool.size(); i++) {
        SVModelPtr t_model = m_modelPool[i];
        if (t_model == _model) {
            t_res = true;
        }
    }
    m_lock->unlock();
    return t_res;
}

void SVModelMgr::_recycleUnuseModel(){
    m_lock->lock();
    for(s32 i=0;i<m_modelPool.size();) {
        if(m_modelPool[i].use_count() == 1) {
            m_modelPool.remove(i);
        }else{
            i++;
        }
    }
    m_modelPool.reserveForce(m_modelPool.size());
    m_lock->unlock();
}
