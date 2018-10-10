//
// SVIOSInstreamNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVIOSInstreamNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../app/SVInst.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTextureIOS.h"
#include "../mtl/SVMtlCore.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../rendercore/renderer/SVContextBase.h"
#include "../core/SVGeoGen.h"

SVIOSInstreamNode::SVIOSInstreamNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVIOSInstreamNode";
    m_tt = E_TEX_INSTREAM;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"screennorbgra");
}

SVIOSInstreamNode::~SVIOSInstreamNode() {
    destroy();
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
}

void SVIOSInstreamNode::init(SVTEXTYPE _tt){
    m_tt = _tt;
}

void SVIOSInstreamNode::destroy(){
}

void SVIOSInstreamNode::update(f32 dt) {
    SVNode::update(dt);
    if(m_pRenderObj && m_pMtl){
        m_pMtl->setTexcoordFlip(1.0f, -1.0f);
        m_pMtl->setTexture(0, m_tt);    //那第一张纹理
        m_pMtl->setModelMatrix(m_absolutMat.get());
        m_pMtl->update(dt);
        m_pRenderObj->setMtl(m_pMtl);
        m_pRenderObj->setMesh(mApp->getDataMgr()->m_screenMesh);
    }
}

void SVIOSInstreamNode::render() {
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (m_pRenderObj ) {
        m_pRenderObj->pushCmd(t_rs, RST_BACKGROUND, "SVIOSInstreamNode");
    }
}










