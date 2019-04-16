//
//  SVDeformMgr.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/4/13.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVDeformMgr.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVCameraMgr.h"
#include "../core/SVDeformImageMove.h"
#include "../node/SVScene.h"
#include "../base/SVLock.h"

SVDeformMgr::SVDeformMgr(SVInst *_app)
:SVSysBase(_app){
    m_deformArray.clear();
}

SVDeformMgr::~SVDeformMgr() {
}

void SVDeformMgr::init() {
    m_defromLock = MakeSharedPtr<SVLock>();
}

void SVDeformMgr::destroy(){
   
}

void SVDeformMgr::update(f32 _dt){
    m_defromLock->lock();
    for(int i=0;i<m_deformArray.size();i++){
        m_deformArray[i]->update(_dt);
        m_deformArray[i]->render();
    }
    m_deformArray.clear();
    m_defromLock->unlock();
}

void SVDeformMgr::pushDeform(SVDeformImageMovePtr deform){
    m_defromLock->lock();
    m_deformArray.append(deform);
    m_defromLock->unlock();
}
