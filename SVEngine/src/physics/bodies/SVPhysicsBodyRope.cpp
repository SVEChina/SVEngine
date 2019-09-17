//
// SVPhysicsBodyRope.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBodyRope.h"
//test
#include "../../node/SVLineNode.h"
SVPhysicsBodyRope::SVPhysicsBodyRope(SVInst* _app, btSoftBodyWorldInfo& _worldInfo, FVec3 &_from, FVec3 &_to, s32 _resCount, s32 _fixeds ):SVPhysicsBodySoft(_app) {
    m_type = E_PHYSICS_BODY_ROPE;
    m_softBody = btSoftBodyHelpers::CreateRope(_worldInfo, btVector3(_from.x, _from.y, _from.z),
                                                    btVector3(_to.x, _to.y, _to.z),
                                                    _resCount,
                                                    _fixeds);
    m_softBody->m_cfg.piterations = 4;
    m_softBody->m_materials[0]->m_kLST = 0.05;
    m_softBody->setTotalMass(20);
//    m_softBody->addForce(btVector3(0.0f, 5.0f, 0.0f));
}

SVPhysicsBodyRope::~SVPhysicsBodyRope() {
    m_softBody = nullptr;
}

void SVPhysicsBodyRope::init(){
    
}

void SVPhysicsBodyRope::destroy(){
    if (m_softBody) {
        delete m_softBody;
    }
}

void SVPhysicsBodyRope::update(f32 _dt){
    //test!!!
    SVLineNodePtr t_lineNode = DYN_TO_SHAREPTR(SVLineNode, m_pNode);
    if (t_lineNode) {
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
            s32 t = 0;
        }
        /*
        s32 t_nodesSize = m_softBody->m_nodes.size();
        s32 t_dataSize = t_nodesSize*3;
        f32 t_data[t_dataSize];
        for (s32 i = 0; i < t_nodesSize; i++)
        {
            const btSoftBody::Node& n = m_softBody->m_nodes[i];
            f32 t_x = n.m_x[0];
            f32 t_y = n.m_x[1];
            f32 t_z = n.m_x[2];
//
            t_data[i*3 + 0] = t_x;
            t_data[i*3 + 1] = t_y;
            t_data[i*3 + 2] = t_z;
//            t_data[i*6 + 3] = t_to_x;
//            t_data[i*6 + 4] = t_to_y;
//            t_data[i*6 + 5] = t_to_z;
            s32 t = 0;
        }
         */
        t_lineNode->setLineData(t_data, t_dataSize);
    }
}

