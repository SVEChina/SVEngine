//
// SVAnimateSkin.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVAnimateSkin.h"


SVAnimateSkin::SVAnimateSkin(SVInst* _app,cptr8 _name)
:SVGBase(_app)
,m_name(_name){
    
}

SVAnimateSkin::~SVAnimateSkin() {
    
}

cptr8 SVAnimateSkin::getName() {
    return m_name.c_str();
}


//
SVSkinMgr::SVSkinMgr(SVInst* _app)
:SVGBase(_app){
    
}

SVSkinMgr::~SVSkinMgr() {
    
}

void SVSkinMgr::addSkin(SVAnimateSkinPtr _skin) {
    //m_skinPool
}

void SVSkinMgr::remvoeSkin(cptr8 _name) {
    
}
