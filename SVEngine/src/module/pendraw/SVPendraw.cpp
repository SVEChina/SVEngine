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
#include "../../basesys/filter/SVFilterBase.h"
#include "../../basesys/SVStaticData.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../basesys/SVConfig.h"
#include "../../app/SVGlobalMgr.h"
#include "../../node/SVMultPassNode.h"
SVPendraw::SVPendraw(SVInst *_app)
:SVGameBase(_app)
,m_curStroke(nullptr){
    m_strokeWidth = mApp->m_pGlobalMgr->m_pConfig->m_strokeWidth;
    m_strokeColor = mApp->m_pGlobalMgr->m_pConfig->m_strokeColor;
    m_glowWidth = mApp->m_pGlobalMgr->m_pConfig->m_strokeGlowWidth;
    m_glowColor = mApp->m_pGlobalMgr->m_pConfig->m_strokeGlowColor;
}

SVPendraw::~SVPendraw() {
    m_curStroke = nullptr;
    m_pRenderObj = nullptr;
    m_fbo = nullptr;
    m_pTex1 = nullptr;
    m_pTex2 = nullptr;
    m_mtl1 = nullptr;
    m_mesh1 = nullptr;
    m_mtl2 = nullptr;
    m_mesh2 = nullptr;
    m_glowFilter = nullptr;
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
            m_pTex1 = t_renderer->getSVTex(E_TEX_HELP0);
        }else{
            m_pTex1 = t_renderer->createSVTex(E_TEX_HELP0, t_w, t_h, GL_RGBA);
        }
        
        if (t_renderer->hasSVTex(E_TEX_HELP1)) {
            m_pTex2 = t_renderer->getSVTex(E_TEX_HELP1);
        }else{
            m_pTex2 = t_renderer->createSVTex(E_TEX_HELP1, t_w, t_h, GL_RGBA);
        }
    }
    m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,m_pTex1,true,true);
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
    m_pRenderObj = MakeSharedPtr<SVMultMeshMtlRenderObject>();
    m_mtl1 = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl1->setTexcoordFlip(1.0f, 1.0f);
    m_mtl1->setTexture(0, E_TEX_HELP0);
    m_mtl1->setDepthEnable(false);
    m_mtl1->setBlendEnable(true);
    m_mtl1->setBlendState(GL_SRC_ALPHA, GL_ONE);
    m_mesh1 = mApp->getDataMgr()->m_screenMesh;
    m_mtl2 = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl2->setTexcoordFlip(1.0f, 1.0f);
    m_mtl2->setTexture(0, E_TEX_HELP1);
    m_mtl2->setDepthEnable(false);
    m_mtl2->setBlendEnable(true);
    m_mtl2->setBlendState(GL_SRC_ALPHA, GL_ONE);
    m_mesh2 = mApp->getDataMgr()->m_screenMesh;
    //做辉光效果处理
    m_glowFilter = MakeSharedPtr<SVFilterGlow>(mApp);
    m_glowFilter->setRSType(RST_AR);
    m_glowFilter->create(E_TEX_HELP0, E_TEX_HELP0);
}

void SVPendraw::destroy() {
    SVGameBase::destroy();
}

void SVPendraw::update(f32 _dt) {
    SVGameBase::update(_dt);
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (t_rs && t_renderer && m_pRenderObj && m_fbo) {
        SVRenderCmdFboBindPtr t_fbo_bind = MakeSharedPtr<SVRenderCmdFboBind>(m_fbo);
        t_fbo_bind->mTag = "pen_draw_bind";
        t_rs->pushRenderCmd(RST_AR_BEGIN, t_fbo_bind);
        
        SVRenderCmdClearPtr t_clear = MakeSharedPtr<SVRenderCmdClear>();
        t_clear->mTag = "pen_draw_clear";
        t_clear->setRenderer(t_renderer);
        t_clear->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        t_rs->pushRenderCmd(RST_AR_BEGIN, t_clear);
        
        SVRenderCmdFboUnbindPtr t_fbo_unbind = MakeSharedPtr<SVRenderCmdFboUnbind>(m_fbo);
        t_fbo_unbind->mTag = "pen_draw_unbind";
        t_rs->pushRenderCmd(RST_AR_END, t_fbo_unbind);
        //更新画笔
        for (s32 i =0; i<m_strokes.size(); i++) {
            SVPenStrokePtr stroke = m_strokes[i];
            stroke->update(_dt);
        }
        /*
        //画荧光背景
        {
            //切换到纹理1
            m_fbo->setTexture(m_pTex1);
            //
            for (s32 i =0; i<m_strokes.size(); i++) {
                SVPenStrokePtr stroke = m_strokes[i];
                stroke->renderGlow();
            }
            //做荧光模糊处理
            if (m_glowFilter) {
                SVNodePtr t_node = m_glowFilter->getNode();
                SVMultPassNodePtr t_passNode = DYN_TO_SHAREPTR(SVMultPassNode, t_node)
                if (t_passNode) {
                    t_passNode->update(_dt);
                    t_passNode->render();
                }
            }
        }
        */
        //画笔触
        {
            //切换换到纹理2
            m_fbo->setTexture(m_pTex2);
            //
            for (s32 i =0; i<m_strokes.size(); i++) {
                SVPenStrokePtr stroke = m_strokes[i];
                stroke->renderStroke();
            }
        }
        
        //再画回主纹理
        if (m_mtl1 && m_mesh1 && m_mtl2 && m_mesh2 && m_pRenderObj) {
            m_pRenderObj->clearMesh();
//            m_pRenderObj->addRenderObj(m_mesh1, m_mtl1);
            m_pRenderObj->addRenderObj(m_mesh2, m_mtl2);
            m_pRenderObj->pushCmd(t_rs, RST_AR_END, "SVPenStrokeRenderReback");
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

void SVPendraw::setGlowWidth(f32 _width){
    m_glowWidth = _width;
}

void SVPendraw::setGlowColor(FVec4 &_color){
    m_glowColor = _color;
}

bool SVPendraw::procEvent(SVEventPtr _event){
    if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_BEGIN){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (!m_curStroke) {
            m_curStroke = MakeSharedPtr<SVPenStroke>(mApp, m_strokeWidth, m_strokeColor, m_glowWidth, m_glowColor);
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


