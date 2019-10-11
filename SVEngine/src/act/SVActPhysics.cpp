//
// SVActPhysics.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVActPhysics.h"
#include "../core/SVModel.h"
#include "../core/SVMesh.h"
#include "../node/SVPatchNode.h"
#include "../node/SVLineNode.h"
#include "../node/SVModelNode.h"
#include "../physics/bodies/SVPhysicsBodyRope.h"
#include "../physics/bodies/SVPhysicsBodyCloth.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVRenderMesh.h"
SVActPhysics::SVActPhysics(SVInst *_app, SVPhysicsBodyPtr _physicsBody):SVActBase(_app){
    m_physicsBody = _physicsBody;
}

SVActPhysics::~SVActPhysics() {
    m_physicsBody = nullptr;
}

//运行
void SVActPhysics::run(SVNodePtr _nodePtr, f32 dt) {
    
}

bool SVActPhysics::isEnd() {
    return false;
}


SVActBodyRope::SVActBodyRope(SVInst *_app, SVPhysicsBodyPtr _physicsBody):SVActPhysics(_app, _physicsBody){
  
}

SVActBodyRope::~SVActBodyRope() {
    
}

//运行
void SVActBodyRope::run(SVNodePtr _nodePtr, f32 dt) {
    SVPhysicsBodyRopePtr t_bodyLine = DYN_TO_SHAREPTR(SVPhysicsBodyRope, m_physicsBody);
    if (t_bodyLine) {
        //test!!!
        SVLineNodePtr t_lineNode = DYN_TO_SHAREPTR(SVLineNode, _nodePtr);
        if (t_lineNode) {
            void *t_data = t_bodyLine->getLineVertexData();
            u32 t_size = t_bodyLine->getLineVertexDataSize();
            u32 t_count = t_bodyLine->getLineVertexCount();
            if (t_data && t_size > 0) {
                t_lineNode->setLineData((f32 *)t_data, t_size, t_count);
            }
        }
    }
}

bool SVActBodyRope::isEnd() {
    return false;
}


SVActBodyCloth::SVActBodyCloth(SVInst *_app, SVPhysicsBodyPtr _physicsBody):SVActPhysics(_app, _physicsBody){
  
}

SVActBodyCloth::~SVActBodyCloth() {
    
}

//运行
void SVActBodyCloth::run(SVNodePtr _nodePtr, f32 dt) {
    SVPhysicsBodyClothPtr t_bodyCloth = DYN_TO_SHAREPTR(SVPhysicsBodyCloth, m_physicsBody);
    if (t_bodyCloth) {
        //test!!!
        SVPatchNodePtr t_patchNode = DYN_TO_SHAREPTR(SVPatchNode, _nodePtr);
        if (t_patchNode) {
            void *t_data = t_bodyCloth->getFaceVertexData();
            u32 t_size = t_bodyCloth->getFaceVertexDataSize();
            u32 t_count = t_bodyCloth->getFaceVertexCount();
            if (t_data && t_size > 0) {
                SVDataSwapPtr t_pVertexData = MakeSharedPtr<SVDataSwap>();
                t_pVertexData->writeData(t_data, t_size);
                t_patchNode->setPatchVertexData(t_pVertexData, t_count);
            }
        }
        //
        SVModelNodePtr t_modelNode = DYN_TO_SHAREPTR(SVModelNode, _nodePtr);
        if (t_modelNode) {
            void *t_data = t_bodyCloth->getFaceVertexData();
            u32 t_size = t_bodyCloth->getFaceVertexDataSize();
            u32 t_count = t_bodyCloth->getFaceVertexCount();
            if (t_data && t_size > 0) {
                int a = 0;
                //测试面纱就一个mesh
                SVModelPtr t_model = t_modelNode->getModel();
                SVMeshPtr t_mesh = t_model->getMesh(0);
                SVRenderMeshPtr t_renderMesh = t_mesh->getRenderMesh();
                SVDataSwapPtr t_vertexData = MakeSharedPtr<SVDataSwap>();
                t_vertexData->writeData(t_data, t_size);
                t_renderMesh->setVertexData(t_vertexData);
                
            }
        }
    }
    //
}

bool SVActBodyCloth::isEnd() {
    return false;
}
