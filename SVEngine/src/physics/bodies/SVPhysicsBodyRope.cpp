//
// SVPhysicsBodyRope.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBodyRope.h"
SVPhysicsBodyRope::SVPhysicsBodyRope(SVInst* _app, btSoftBodyWorldInfo& _worldInfo, FVec3 &_from, FVec3 &_to, s32 _resCount, s32 _fixeds ):SVPhysicsBody(_app) {
    m_type = E_PHYSICS_BODY_ROPE;
    m_softBody = btSoftBodyHelpers::CreateRope(_worldInfo, btVector3(_from.x, _from.y, _from.z),
                                                    btVector3(_to.x, _to.y, _to.z),
                                                    _resCount,
                                                    _fixeds);
    m_softBody->m_cfg.piterations = 4;
    m_softBody->m_materials[0]->m_kLST = 0.1;
    m_softBody->setTotalMass(20);
//    pdemo->getSoftDynamicsWorld()->addSoftBody(psb);
}

SVPhysicsBodyRope::~SVPhysicsBodyRope() {
    
}


void SVPhysicsBodyRope::destroy(){
  
}

void SVPhysicsBodyRope::update(f32 _dt){
    
}

