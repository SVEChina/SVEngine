//
// SVResShader.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResShader.h"
#include "../app/SVInst.h"
#include "../base/SVLock.h"
#include "renderer/SVRendererBase.h"

SVResShader::SVResShader(SVInst* _app)
:SVRObjBase(_app){
    //m_uid = mApp->m_IDPool.applyUID();
}

SVResShader::~SVResShader(){
    //mApp->m_IDPool.returnUID(m_uid);
}

