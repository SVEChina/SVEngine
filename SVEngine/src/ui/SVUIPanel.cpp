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
#include "../mtl/SVMtlColor.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
#include "../basesys/SVStaticData.h"
//
SVUIPanel::SVUIPanel(SVInst *_app)
:SVUIBase(_app) {
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
    m_texMtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
    m_texMtl->setTexcoordFlip(1.0f, -1.0f);
    m_texMtl->setBlendEnable(true);
    m_texMtl->setBlendState(MTL_BLEND_SRC_ALPHA, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
    m_pMtl = m_texMtl;
    //
    m_colorMtl = MakeSharedPtr<SVMtlColor>(mApp);
    m_colorMtl->setBlendEnable(true);
    m_colorMtl->setBlendState(MTL_BLEND_SRC_ALPHA, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
    //
    setColor(0.0f, 0.0f, 0.0f, 0.0f);
}

SVUIPanel::SVUIPanel(SVInst *_app,f32 _w,f32 _h)
:SVUIBase(_app) {
    ntype = "SVUIPanel";
    m_canSelect = false;
    m_visible = true;
    m_width = _w;
    m_height = _h;
    m_archo = E_ARCHO_CC;
    m_rsType = RST_UI;
    m_dirty_mesh = true;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pTex = nullptr;
    m_pMesh = nullptr;
    m_texMtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
    m_texMtl->setTexcoordFlip(1.0f, -1.0f);
    m_texMtl->setBlendEnable(true);
    m_texMtl->setBlendState(MTL_BLEND_SRC_ALPHA, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
    m_pMtl = m_texMtl;
    //
    m_colorMtl = MakeSharedPtr<SVMtlColor>(mApp);
    m_colorMtl->setBlendEnable(true);
    m_colorMtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
    setColor(0.0f, 0.0f, 0.0f, 0.0f);
}

SVUIPanel::~SVUIPanel() {
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
    m_pTex = nullptr;
    m_colorMtl = nullptr;
    m_texMtl = nullptr;
}

//
void SVUIPanel::setSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_dirty_mesh = true;
}

void SVUIPanel::getSize(FVec2 &_size){
    _size.set(m_width, m_height);
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

void SVUIPanel::setColor(f32 _r, f32 _g, f32 _b, f32 _a){
    if (m_colorMtl) {
        m_colorMtl->setColor(_r, _g, _b, _a);
    }
}

void SVUIPanel::update(f32 dt) {
    SVNode::update(dt);
    if(m_dirty_mesh) {
        m_dirty_mesh = false;
        m_pMesh = SVGeoGen::genRectARCHO(mApp, m_width, m_height,m_archo,m_aabbBox);
        //
        m_aabbBox.clear();
        SVRect t_rc = SVUIBase::getRect(m_archo,m_width,m_height);
        FVec3 t_min(t_rc.m_lb_pt);
        FVec3 t_max(t_rc.m_rt_pt);
        m_aabbBox.expand(t_min);
        m_aabbBox.expand(t_max);
    }
    //
    if (m_pRenderObj && m_pMesh && m_pMtl) {
        if (m_pTex) {
            m_pMtl = m_texMtl->clone();
            m_pMtl->setTexture(0,m_pTex);
        }else{
            m_pMtl = m_colorMtl->clone();
            m_pMtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
        }
        m_pMtl->setModelMatrix(m_absolutMat.get());
        m_pMtl->update(dt);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(m_pMtl);
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
