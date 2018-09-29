//
// SVBGRAInstreamNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVBGRAInstreamNode.h"
#include "SVScene.h"
#include "SVCameraNode.h"
#include "../mtl/SVMtlCore.h"
#include "../rendercore/SVRenderMesh.h"
#include "../core/SVGeoGen.h"
#include "../basesys/SVConfig.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../basesys/SVStaticData.h"

//
SVBGRAInstreamNode::SVBGRAInstreamNode(SVInst *_app)
:SVNode(_app) {
    SVBGRAInstreamNode(_app,100,100);
}

SVBGRAInstreamNode::SVBGRAInstreamNode(SVInst *_app,f32 _w,f32 _h)
:SVNode(_app) {
    ntype = "SVBGRAInstreamNode";
    m_rsType = RST_ANIMATE;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_canSelect = false;
    m_pMesh = nullptr;
    setSpriteSize(_w,_h);
}

SVBGRAInstreamNode::~SVBGRAInstreamNode() {
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
    m_pTex = nullptr;
}

//
void SVBGRAInstreamNode::setSpriteSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_pMesh = SVGeoGen::genRect(mApp, m_width, m_height, 0, 0, m_width, m_height,m_aabbBox);
}

void SVBGRAInstreamNode::setMaterial(SVMtlCorePtr _mtl){
    m_pMtl = _mtl;
}

void SVBGRAInstreamNode::setTexture(SVTexturePtr _tex){
    m_pTex = _tex;
}

void SVBGRAInstreamNode::update(f32 dt) {
    SVNode::update(dt);
    if (m_pRenderObj && m_pMesh) {
        if(m_pMtl){
            m_pMtl->setBlendEnable(true);
            m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            m_pMtl->setModelMatrix(m_absolutMat.get());
            m_pMtl->setTexcoordFlip(1.0f, -1.0f);
            m_pMtl->update(dt);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(m_pMtl);
        }else{
            //创建新的材质
            SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2dBGRA");
            t_mtl->setBlendEnable(false);
            t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            t_mtl->setModelMatrix(m_absolutMat.get());
            t_mtl->setTexcoordFlip(1.0f, -1.0f);
            t_mtl->setTexture(0,m_pTex);
            t_mtl->setBlendEnable(true);
            t_mtl->setBlendState(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            t_mtl->update(dt);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(t_mtl);
        }
    }
}

void SVBGRAInstreamNode::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->m_pGlobalParam->m_curScene->getRenderRS();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVBGRAInstreamNode");
        }
    }
    SVNode::render();
}
