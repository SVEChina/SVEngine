//
// SVOpFaceBeautyExt.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpFaceBeautyExt.h"
#include "app/SVInst.h"
#include "app/SVGlobalMgr.h"
#include "basesys/SVSceneMgr.h"
#include "basesys/SVBasicSys.h"
#include "basesys/SVPictureProcess.h"
#include "basesys/filter/SVFilterBase.h"
#include "basesys/filter/SVBasedonFilter.h"
#include "SVFaceBeautyShowFilter.h"
#include "SVFilterExtDef.h"
#include "event/SVEventMgr.h"
#include "event/SVEvent.h"
#include "event/SVOpEvent.h"
#include "base/SVDataSwap.h"
#include "rendercore/SVRenderMgr.h"
#include "rendercore/renderer/SVRendererBase.h"
#include "node/SVScene.h"


//设置美颜滤镜
SVOpFaceBeautyExt::SVOpFaceBeautyExt(SVInst* _app,cptr8 _scenename,cptr8 _filter,s32 _lows)
        :SVOpBase(_app) {
    m_scenename = _scenename;
    m_filter = _filter;
    m_lows=_lows;
}
SVOpFaceBeautyExt::~SVOpFaceBeautyExt(){
    m_scenename = nullptr;
    m_filter = nullptr;
    m_lows=0.0f;
}

void SVOpFaceBeautyExt::_process(f32 _dt) {
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if( t_picproc ) {
        SVBasedonFilterPtr t_baseOn=MakeSharedPtr<SVBasedonFilter>(mApp);
        t_baseOn->create();
        t_picproc->addFilter(t_baseOn);
        t_picproc->openFilter(t_baseOn);
         if(m_lows==1){
            SVFairDataBlurPtr t_fair=MakeSharedPtr<SVFairDataBlur>(mApp);
            t_fair->create();
            t_picproc->addFilter(t_fair);
            t_picproc->openFilter(t_fair);
        }else if(m_lows==2){
            SVFairLtraLowPtr t_fair=MakeSharedPtr<SVFairLtraLow>(mApp);
            t_fair->create();
            t_picproc->addFilter(t_fair);
            t_picproc->openFilter(t_fair);
        }
    }
}


SVOpWatermarkExt::SVOpWatermarkExt(SVInst *_app, SVNodePtr _node)
        :SVOpBase(_app) {
    m_node=_node;
}

SVOpWatermarkExt::~SVOpWatermarkExt(){
    if(m_node){
        m_node= nullptr;
    }
}

void SVOpWatermarkExt::_process(f32 _dt){
    SVScenePtr t_scene = mApp->getSceneMgr()->getScene();
    if( t_scene ) {
        t_scene->addNode(m_node);
    }
}



