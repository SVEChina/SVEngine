//
// SVResVBO.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResVBO.h"
#include "../app/SVInst.h"
#include "../base/SVLock.h"
#include "renderer/SVRendererBase.h"

SVResVBO::SVResVBO(SVInst* _app)
:SVRObjBase(_app){
    m_uid = mApp->m_IDPool.applyUID();
}

SVResVBO::~SVResVBO(){
    mApp->m_IDPool.returnUID(m_uid);
}

void SVResVBO::create(SVRendererBasePtr _renderer) {
}

void SVResVBO::destroy(SVRendererBasePtr _renderer) {
}

void SVResVBO::render(SVRendererBasePtr _renderer) {
    
}

void SVResVBO::updateConf(RENDERMESHCONF& _conf) {
    
}

void SVResVBO::updateData(RENDERMESHDATA& _data) {
    
}

