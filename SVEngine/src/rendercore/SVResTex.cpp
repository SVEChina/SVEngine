//
// SVResTex.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResTex.h"
#include "../app/SVInst.h"
#include "../base/SVLock.h"
#include "renderer/SVRendererBase.h"

SVResTex::SVResTex(SVInst* _app)
:SVRObjBase(_app){
    m_uid = mApp->m_IDPool.applyUID();
    m_texLock = MakeSharedPtr<SVLock>();
}

SVResTex:: ~SVResTex(){
    mApp->m_IDPool.returnUID(m_uid);
    m_texLock = nullptr;
}

void SVResTex::create(SVRendererBasePtr _renderer){
    
}

void SVResTex::destroy(SVRendererBasePtr _renderer){
    
}

void SVResTex::setTexData(void *_data, s32 _len){
    
}

void SVResTex::commit(){
    
}
