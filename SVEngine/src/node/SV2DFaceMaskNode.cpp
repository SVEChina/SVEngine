//
// SV2DFaceMaskNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SV2DFaceMaskNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../app/SVInst.h"
#include "../basesys/SVConfig.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlFace2D.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../app/SVGlobalMgr.h"
#include "../rendercore/SVRenderMgr.h"

SV2DFaceMaskNode::SV2DFaceMaskNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SV2DFaceMaskNode";
    m_pIndexs = MakeSharedPtr<SVDataSwap>();
    m_pVerts = MakeSharedPtr<SVDataSwap>();
    m_pT0 = MakeSharedPtr<SVDataSwap>();
    m_renderObject = MakeSharedPtr<SVRenderObject>();
    m_pFaceMesh = MakeSharedPtr<SVRenderMeshDvid>(mApp);
}

SV2DFaceMaskNode::~SV2DFaceMaskNode() {
    m_pIndexs = nullptr;
    m_pVerts  = nullptr;
    m_pT0     = nullptr;
    m_mtlFace2D = nullptr;
    m_renderObject = nullptr;
    m_pFaceMesh = nullptr;
}

void SV2DFaceMaskNode::update(f32 dt) {
    SVNode::update(dt);
}

void SV2DFaceMaskNode::render() {
    SVNode::render();
}

void SV2DFaceMaskNode::setMaterial(SVMtlFace2DPtr _mtlFace2D){
    m_mtlFace2D = _mtlFace2D;
}

SVMtlFace2DPtr SV2DFaceMaskNode::getMaterial(){
    return m_mtlFace2D;
}
