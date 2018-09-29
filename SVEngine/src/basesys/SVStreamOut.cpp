//
// SVStreamOut.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVStreamOut.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../base/SVLock.h"
#include "../node/SVIOSOutstreamNode.h"
#include "../node/SVScene.h"
#include "../node/SVTransTexNode.h"
#include "../basesys/SVSceneMgr.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderCmdOut.h"

SVStreamOut::SVStreamOut(SVInst *_app)
:SVProcess(_app){
    m_lock = MakeSharedPtr<SVLock>();
    m_outStream = nullptr;
}

SVStreamOut::~SVStreamOut() {
    m_lock = nullptr;
    m_outStream = nullptr;
}

void SVStreamOut::createOutStream(cptr8 _name, s32 _type, s32 _format, s32 _steamType) {
    if(_type == 0){
#ifdef SV_IOS
        m_outStream = MakeSharedPtr<SVIOSOutstreamNode>(mApp);
        SVIOSOutstreamNodePtr t_streamNode = std::dynamic_pointer_cast<SVIOSOutstreamNode>(m_outStream);
        if( t_streamNode ) {
            s32 _w = mApp->m_pGlobalParam->m_inner_width;
            s32 _h = mApp->m_pGlobalParam->m_inner_height;
            t_streamNode->init(SV_OUTSTEAMFORMATE(_format),_w,_h);
            if (_steamType) {
                t_streamNode->setRSType(RST_MASK2D);
            }
        }
#endif
    }else if(_type == 1){
#ifdef SV_ANDROID
        m_outStream = MakeSharedPtr<SVTransTexNode>(mApp);
        SVTransTexNodePtr t_streamNode = std::dynamic_pointer_cast<SVTransTexNode>(m_outStream);
#endif
    }
}

void SVStreamOut::destroyOutStream() {
    m_outStream = nullptr;
}

void SVStreamOut::active() {
    SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
    if(t_sc && m_outStream){
        t_sc->addNode(m_outStream);
    }
}

void SVStreamOut::unactive() {
    if(m_outStream){
        m_outStream->removeFromParent();
    }
}

void SVStreamOut::update(f32 _dt) {
    
}

s32 SVStreamOut::getTexId(){
    SVTransTexNodePtr t_streamNode = std::dynamic_pointer_cast<SVTransTexNode>(m_outStream);
    if(t_streamNode){
        return  t_streamNode->getTexId();
    }
    return 0;

}

void SVStreamOut::setStreamOutCB(cb_out_stream _cb) {
#ifdef SV_IOS
    m_lock->lock();
    m_pOutStreamCB = _cb;
    if( m_outStream && m_pOutStreamCB) {

        SVIOSOutstreamNodePtr t_streamNode = std::dynamic_pointer_cast<SVIOSOutstreamNode>(m_outStream);
        t_streamNode->setCallback(m_pOutStreamCB);
    }
    m_lock->unlock();
#endif
}
