//
// SVPendraw.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPendraw.h"
#include "SVPenStroke.h"
#include "../../event/SVOpEvent.h"
#include "../SVGameReady.h"
#include "../SVGameRun.h"
#include "../SVGameEnd.h"
#include "../../app/SVInst.h"
#include "../../base/SVDataSwap.h"
#include "../../mtl/SVMtlCore.h"
#include "../../rendercore/SVRenderTexture.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../../rendercore/SVRenderCmd.h"
#include "../../rendercore/SVRenderScene.h"
#include "../../rendercore/SVRenderObject.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../basesys/SVBasicSys.h"
#include "../../basesys/filter/SVFilterGlow.h"
#include "../../basesys/SVPictureProcess.h"
#include "../../basesys/SVStaticData.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../basesys/SVConfig.h"
#include "../../app/SVGlobalMgr.h"
SVPendraw::SVPendraw(SVInst *_app)
:SVGameBase(_app)
,m_curStroke(nullptr){
    m_strokeWidth = mApp->m_pGlobalMgr->m_pConfig->m_strokeWidth;
    m_strokeColor.set(mApp->m_pGlobalMgr->m_pConfig->m_strokeColor);
}

SVPendraw::~SVPendraw() {
    m_curStroke = nullptr;
    m_pRenderObj = nullptr;
    m_fbo = nullptr;
    m_pInTex = nullptr;
    m_pOutTex = nullptr;
    m_strokes.destroy();
}

void SVPendraw::init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end) {
    SVGameBase::init(_ready,_run,_end);
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if (t_renderer) {
        SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_MAIN);
        s32 t_w = t_tex->getwidth();
        s32 t_h = t_tex->getheight();
        if (t_renderer->hasSVTex(E_TEX_HELP0)) {
            m_pInTex = t_renderer->getSVTex(E_TEX_HELP0);
        }else{
            m_pInTex = t_renderer->createSVTex(E_TEX_HELP0, t_w, t_h, GL_RGBA);
        }
        
        if (t_renderer->hasSVTex(E_TEX_HELP1)) {
            m_pOutTex = t_renderer->getSVTex(E_TEX_HELP1);
        }else{
            m_pOutTex = t_renderer->createSVTex(E_TEX_HELP1, t_w, t_h, GL_RGBA);
        }
    }
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,nullptr,false,false);
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
    //做辉光效果处理
    SVPictureProcessPtr t_pic = mApp->getBasicSys()->getPicProc();
    SVFilterGlowPtr t_glow=MakeSharedPtr<SVFilterGlow>(mApp);
    t_glow->create(E_TEX_HELP0, E_TEX_HELP1);
    t_pic->addFilter(t_glow);
    t_pic->openFilter(t_glow);
}

void SVPendraw::destroy() {
    SVGameBase::destroy();
}

void SVPendraw::update(f32 _dt) {
    SVGameBase::update(_dt);
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (t_rs && t_renderer && m_pRenderObj && m_fbo) {
        m_fbo->setTexture(m_pInTex);
        SVRenderCmdFboBindPtr t_fbo_bind = MakeSharedPtr<SVRenderCmdFboBind>(m_fbo);
        t_fbo_bind->mTag = "pen_draw_bind";
        t_rs->pushRenderCmd(RST_AR_BEGIN, t_fbo_bind);
        
        SVRenderCmdClearPtr t_clear = MakeSharedPtr<SVRenderCmdClear>();
        t_clear->mTag = "pen_draw_clear";
        t_clear->setRenderer(t_renderer);
        t_clear->setClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        t_rs->pushRenderCmd(RST_AR_BEGIN, t_clear);
        
        SVRenderCmdFboUnbindPtr t_fbo_unbind = MakeSharedPtr<SVRenderCmdFboUnbind>(m_fbo);
        t_fbo_unbind->mTag = "pen_draw_unbind";
        t_rs->pushRenderCmd(RST_AR_END, t_fbo_unbind);
        
        //再画回主纹理
        SVMtlCorePtr t_lkMtl=MakeSharedPtr<SVMtlCore>(mApp,"screennor");
        t_lkMtl->setTexcoordFlip(1.0f, 1.0f);
        t_lkMtl->setTexture(0, E_TEX_HELP1);
        t_lkMtl->setBlendEnable(true);
        t_lkMtl->setBlendState(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        SVRenderMeshPtr t_mesh = mApp->getDataMgr()->m_screenMesh;
        m_pRenderObj->setMesh(t_mesh);
        m_pRenderObj->setMtl(t_lkMtl);
        m_pRenderObj->pushCmd(t_rs, RST_AR_END, "SVPenStrokeRenderReback");
        
        //画画笔
        for (s32 i =0; i<m_strokes.size(); i++) {
            SVPenStrokePtr stroke = m_strokes[i];
            stroke->update(_dt);
        }
    }
}

void SVPendraw::open() {
    SVGameBase::open();
}

void SVPendraw::close() {
    SVGameBase::close();
}

void SVPendraw::setStrokeWidth(f32 _width){
    m_strokeWidth = _width;
}

void SVPendraw::setStrokeColor(FVec4 &_color){
    m_strokeColor = _color;
}

bool SVPendraw::procEvent(SVEventPtr _event){
    if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_BEGIN){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (!m_curStroke) {
            m_curStroke = MakeSharedPtr<SVPenStroke>(mApp);
            m_curStroke->setStrokeWidth(m_strokeWidth);
            m_curStroke->setStrokeColor(m_strokeColor);
            m_strokes.append(m_curStroke);
        }
        m_curStroke->begin(t_touch->x,t_touch->y,0.0);
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_END){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (t_touch && m_curStroke) {
            m_curStroke->end(t_touch->x,t_touch->y,0.0f);
        }
        m_curStroke = nullptr;
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_MOVE){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (t_touch && m_curStroke) {
            m_curStroke->draw(t_touch->x,t_touch->y,0.0f);
        }
    }
    return true;
}


