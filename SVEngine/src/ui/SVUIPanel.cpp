//
// SVUIPanel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVUIPanel.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtl2D.h"
#include "../rendercore/SVRenderMesh.h"
#include "../core/SVGeoGen.h"
#include "../basesys/SVConfig.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
#include "../basesys/SVStaticData.h"
//
SVUIPanel::SVUIPanel(SVInst *_app)
:SVNode(_app) {
    ntype = "SVUIPanel";
    m_width = 100.0f;
    m_height = 100.0f;
    m_archo = E_ARCHO_CC;
    m_rsType = RST_UI;
    m_dirty_mesh = true;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_canSelect = false;
    m_pTex = nullptr;
    m_pMesh = nullptr;
    m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
}

SVUIPanel::SVUIPanel(SVInst *_app,f32 _w,f32 _h)
:SVNode(_app) {
    ntype = "SVUIPanel";
    m_width = _w;
    m_height = _h;
    m_archo = E_ARCHO_CC;
    m_rsType = RST_UI;
    m_dirty_mesh = true;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pTex = nullptr;
    m_pMesh = nullptr;
    m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
}

SVUIPanel::~SVUIPanel() {
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
    m_pTex = nullptr;
}

//
void SVUIPanel::setSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_dirty_mesh = true;
}

//改变锚点
void SVUIPanel::setArcho(EUIARCHO _cho) {
    if(m_archo!=_cho) {
        m_archo = _cho;
        m_dirty_mesh = true;
    }
}

void SVUIPanel::setTexture(cptr8 _path, bool enableMipMap){
    m_pTex = mApp->getTexMgr()->getTextureSync(_path,true, enableMipMap);
}

void SVUIPanel::update(f32 dt) {
    SVNode::update(dt);
    if(m_dirty_mesh) {
        m_dirty_mesh = false;
        m_pMesh = SVGeoGen::genRectARCHO(mApp, m_width, m_height,m_archo,m_aabbBox);
    }
    //
    if (m_pRenderObj && m_pMesh && m_pMtl) {
        //创建新的材质
        SVMtlCorePtr t_mtl = m_pMtl->clone();
        t_mtl->setModelMatrix(m_absolutMat.get());
        t_mtl->setTexcoordFlip(-1.0f, -1.0f);
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        t_mtl->setTexture(0,m_pTex);
        t_mtl->update(dt);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(t_mtl);
    }
}

void SVUIPanel::render() {
    if (m_visible){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVUIPanel");
        }
    }
    SVNode::render();
}
