//
// SVModelMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModelMgr.h"
#include "../core/SVModel.h"
//
SVModelMgr::SVModelMgr(SVInst* _app)
:SVSysBase(_app){
    
}

SVModelMgr::~SVModelMgr() {
    
}

void SVModelMgr::init(){
    
}

void SVModelMgr::update(f32 _dt){
    
}

void SVModelMgr::destroy(){
    m_modelPool.destroy();
}

void SVModelMgr::addModel(SVModelPtr _model){
    if (_model) {
        if (!_hasModel(_model)) {
            m_modelPool.append(_model);
        }
    }
}

SVModelPtr SVModelMgr::getModel(SVString _name){
    SVModelPtr model = nullptr;
    for (s32 i = 0; i<m_modelPool.size(); i++) {
        SVModelPtr t_model = m_modelPool[i];
        if (strcmp(t_model->getName(), _name.c_str()) == 0) {
            model = t_model;
            break;
        }
    }
    return model;
}

bool SVModelMgr::_hasModel(SVModelPtr _model){
    for (s32 i = 0; i<m_modelPool.size(); i++) {
        SVModelPtr t_model = m_modelPool[i];
        if (t_model == _model) {
            return true;
        }
    }
    return false;
}
