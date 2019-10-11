//
// SVPhysicsBodyRope.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBodyRope.h"
#include "../../base/SVDataSwap.h"
SVPhysicsBodyRope::SVPhysicsBodyRope(SVInst* _app, btSoftBodyWorldInfo& _worldInfo, FVec3 &_from, FVec3 &_to, s32 _resCount, s32 _fixeds ):SVPhysicsBodySoft(_app) {
    m_type = E_PHYSICS_BODY_ROPE;
    m_vertexCount = 0;
    m_pVertexData = MakeSharedPtr<SVDataSwap>();
    m_softBody = btSoftBodyHelpers::CreateRope(_worldInfo, btVector3(_from.x, _from.y, _from.z),
                                                    btVector3(_to.x, _to.y, _to.z),
                                                    _resCount,
                                                    _fixeds);
    m_softBody->m_cfg.piterations = 4;
    //线性刚度系数
    m_softBody->m_materials[0]->m_kLST = 0.01;
    m_softBody->setTotalMass(20);
}

SVPhysicsBodyRope::~SVPhysicsBodyRope() {
    m_pVertexData = nullptr;
}

void SVPhysicsBodyRope::init(){
    
}

void SVPhysicsBodyRope::destroy(){
    if (m_softBody) {
        delete m_softBody;
    }
}

void SVPhysicsBodyRope::update(f32 _dt){
    _updateLineVertexData();
}

void SVPhysicsBodyRope::_updateLineVertexData(){
    if (m_softBody && m_pVertexData) {
        /* Links    */
        s32 t_linksSize = m_softBody->m_links.size();
        s32 t_dataSize = t_linksSize*6;
        f32 t_data[t_dataSize];
        for (s32 i = 0; i < t_linksSize; i++)
        {
            const btSoftBody::Link& l = m_softBody->m_links[i];
            f32 t_from_x = l.m_n[0]->m_x[0];
            f32 t_from_y = l.m_n[0]->m_x[1];
            f32 t_from_z = l.m_n[0]->m_x[2];
            
            f32 t_to_x = l.m_n[1]->m_x[0];
            f32 t_to_y = l.m_n[1]->m_x[1];
            f32 t_to_z = l.m_n[1]->m_x[2];
            
            t_data[i*6 + 0] = t_from_x;
            t_data[i*6 + 1] = t_from_y;
            t_data[i*6 + 2] = t_from_z;
            t_data[i*6 + 3] = t_to_x;
            t_data[i*6 + 4] = t_to_y;
            t_data[i*6 + 5] = t_to_z;
        }
        m_pVertexData->writeData(t_data, sizeof(f32)*t_dataSize);
        m_vertexCount = t_dataSize;
    }
}

void *SVPhysicsBodyRope::getLineVertexData(){
    return m_pVertexData->getData();
}

u32   SVPhysicsBodyRope::getLineVertexDataSize(){
    return m_pVertexData->getSize();
}

u32   SVPhysicsBodyRope::getLineVertexCount(){
    return m_vertexCount;
}
