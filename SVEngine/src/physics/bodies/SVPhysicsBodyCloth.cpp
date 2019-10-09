//
// SVPhysicsBodyCloth.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBodyCloth.h"
//test
#include "../../node/SVPatchNode.h"
SVPhysicsBodyCloth::SVPhysicsBodyCloth(SVInst* _app, btSoftBodyWorldInfo& _worldInfo, const FVec3& _corner00,
                                       const FVec3& _corner10,
                                       const FVec3& _corner01,
                                       const FVec3& _corner11,
                                       s32 _resx,
                                       s32 _resy,
                                       s32 _fixeds,
                                       bool gendiags):SVPhysicsBodySoft(_app) {
    m_type = E_PHYSICS_BODY_CLOTH;
    m_softBody = btSoftBodyHelpers::CreatePatch(_worldInfo, btVector3(_corner00.x, _corner00.y, _corner00.z),
                                                     btVector3(_corner10.x, _corner10.y, _corner10.z),
                                                     btVector3(_corner01.x, _corner01.y, _corner01.z),
                                                     btVector3(_corner11.x, _corner11.y, _corner11.z),
                                                     _resx, _resy,
                                                     _fixeds, gendiags);
    
//    m_softBody->getCollisionShape()->setMargin(0.5);
    m_softBody->m_materials[0]->m_kLST = 0.005;
//    m_softBody->m_materials[0]->m_kAST = 0.001;
    m_softBody->generateBendingConstraints(2, m_softBody->m_materials[0]);
    m_softBody->setTotalMass(150);
}

SVPhysicsBodyCloth::~SVPhysicsBodyCloth() {
    m_softBody = nullptr;
}

void SVPhysicsBodyCloth::init(){
    
}

void SVPhysicsBodyCloth::destroy(){
    if (m_softBody) {
        delete m_softBody;
    }
}

void SVPhysicsBodyCloth::update(f32 _dt){
    //test!!!
    SVPatchNodePtr t_patchNode = DYN_TO_SHAREPTR(SVPatchNode, m_pNode);
    if (t_patchNode) {
        /* Nodes    */
//        s32 t_nodesSize = m_softBody->m_nodes.size();
//        V3 vertexData[t_nodesSize];
//        for (s32 i = 0; i < t_nodesSize; i++) {
//            const btSoftBody::Node& n = m_softBody->m_nodes[i];
//            vertexData[i].x = n.m_x[0];
//            vertexData[i].y = n.m_x[1];
//            vertexData[i].z = n.m_x[2];
//        }
        
        /* Faces    */
        s32 t_facesSize = m_softBody->m_faces.size();
        s32 t_vertexSize = t_facesSize*3;
        V3 vertexData[t_vertexSize];
        for (s32 i = 0; i < t_facesSize; i++) {
            const btSoftBody::Face& f = m_softBody->m_faces[i];
            vertexData[i*3 + 0].x = f.m_n[0]->m_x[0];
            vertexData[i*3 + 0].y = f.m_n[0]->m_x[1];
            vertexData[i*3 + 0].z = f.m_n[0]->m_x[2];
            //
            vertexData[i*3 + 1].x = f.m_n[1]->m_x[0];
            vertexData[i*3 + 1].y = f.m_n[1]->m_x[1];
            vertexData[i*3 + 1].z = f.m_n[1]->m_x[2];
            //
            vertexData[i*3 + 2].x = f.m_n[2]->m_x[0];
            vertexData[i*3 + 2].y = f.m_n[2]->m_x[1];
            vertexData[i*3 + 2].z = f.m_n[2]->m_x[2];
        }
        
        SVDataSwapPtr t_pVertexData = MakeSharedPtr<SVDataSwap>();
        t_pVertexData->writeData(&vertexData[0], sizeof(V3)*t_vertexSize);
        t_patchNode->setPatchData(t_pVertexData, t_vertexSize);
    }
}

