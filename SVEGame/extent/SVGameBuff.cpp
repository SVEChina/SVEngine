//
// Created by yinlong on 17/10/27.
//

#include "SVGameBuff.h"
#include "../app/SVInst.h"

SVBuffBase::SVBuffBase(SVInst*_app)
:SVGBase(_app)
{
    m_bEnd=false;
    m_iBufType=0;
    m_iBufId = mApp->m_IDPool.applyUID();
}
    
SVBuffBase::~SVBuffBase(){
    mApp->m_IDPool.returnUID(m_iBufId);
}
    
void SVBuffBase::enter(){
}
    
void SVBuffBase::exit(){
}
    
bool SVBuffBase::isEnd(){
    return m_bEnd;
}
    
bool SVBuffBase::isTrig(){
    return true;
}
    
void SVBuffBase::proc(){
}
    
void SVBuffBase::update(f32 _dt){
}

void SVBuffBase::setBufType(s32 iType){
    m_iBufType = iType;
}

int  SVBuffBase::getBufType(){
    return m_iBufType;
}

int  SVBuffBase::getBufId(){
    return m_iBufId;
}


SVBuffMgr::SVBuffMgr(SVInst* _app)
:SVGBase(_app){
}

SVBuffMgr::~SVBuffMgr(){
}
    
void SVBuffMgr::init(){
}

void SVBuffMgr::destory(){
    BUFCONTAIN::Iterator iter = m_bufContainer.begin();
    while (iter != m_bufContainer.end()) {
        SVBuffBase* t_buf = (*iter);
        if (t_buf){
            t_buf->release();
        }
        iter++;
    }
    m_bufContainer.clear();
}

void SVBuffMgr::update(f32 _dt){
    BUFCONTAIN::Iterator iter = m_bufContainer.begin();
    while (iter != m_bufContainer.end()) {
        SVBuffBase* t_buf = (*iter);
        t_buf->update(_dt);
        
        if (t_buf->isEnd()){
            t_buf->exit();
            t_buf->release();
            iter = m_bufContainer.erase(iter);
            continue;
        }
        
        if (t_buf->isTrig()){
            t_buf->proc();
        }
        iter++;
    }
}

void SVBuffMgr::addBuff(SVBuffBase* pBuf){
    if (pBuf == NULL)
        return;
    
    pBuf->retain();
    pBuf->enter();
    m_bufContainer.push_back(pBuf);
}

SVBuffBase* SVBuffMgr::getBuff(s32 iBufId){
    BUFCONTAIN::Iterator iter = m_bufContainer.begin();
    while (iter != m_bufContainer.end()) {
        SVBuffBase* t_buf = (*iter);
        if (t_buf->getBufId() == iBufId){
            return t_buf;
        }
        iter++;
    }
    return NULL;
}
