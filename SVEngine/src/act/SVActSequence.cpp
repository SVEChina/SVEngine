//
// SVActSequence.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActSequence.h"
SVActSequence::SVActSequence(SVInst *_app):SVActBase(_app) {
    m_acttype = "SVActSequence";
    m_end = false;
    m_curIndex = -1;
}

SVActSequence::~SVActSequence(){
    clearAct();
}

void SVActSequence::run(SVNodePtr _nodePtr, f32 dt) {
    if(m_curIndex<0 && m_actQuene.size()>0 ){
        m_curIndex = 0;
        m_actQuene[m_curIndex]->enter(_nodePtr);
    }
    if( m_curIndex>=0 && m_curIndex<m_actQuene.size() ){
        bool t_change = false;
        if( m_actQuene[m_curIndex]->isEnd() ){
            t_change = true;
            m_actQuene[m_curIndex]->exit(_nodePtr);
            m_curIndex++;
        }
        if( m_curIndex>=m_actQuene.size() ){
            m_end = true;
        }else{
            m_end = false;
            if(t_change){
                m_actQuene[m_curIndex]->enter(_nodePtr);
            }
            m_actQuene[m_curIndex]->run(_nodePtr,dt);
        }
    }
}

void SVActSequence::reset(){
    for(s32 i=0;i<m_actQuene.size();i++){
        m_actQuene[i]->reset();
    }
    m_end = false;
    m_curIndex = -1;
}

bool SVActSequence::isEnd(){
    return m_end;
}

void SVActSequence::addAct(SVActBasePtr _act){
    m_actQuene.append(_act);
}

void SVActSequence::clearAct(){
    m_actQuene.destroy();
    m_curIndex = -1;
}
