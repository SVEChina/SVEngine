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
#include "basesys/filter/SVFaceBeautyFilter.h"
#include "node/SVFaceShapeNode.h"
#include "node/SVFacePointNode.h"
#include "node/SVScene.h"
#include "SVFaceBeautyShowFilter.h"
#include "SVFilterExtDef.h"
#include "event/SVEventMgr.h"
#include "event/SVEvent.h"
#include "event/SVOpEvent.h"
#include "base/SVDataSwap.h"
#include "SVFaceMattingFilter.h"

//设置美颜滤镜
SVOpFaceBeautyExt::SVOpFaceBeautyExt(SVInst* _app,cptr8 _scenename,cptr8 _filter,s32 _lows)
        :SVOpBase(_app) {
    m_scenename = _scenename;
    m_filter = _filter;
    m_lows=_lows;
}

void SVOpFaceBeautyExt::_process(f32 _dt) {
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if( t_picproc ) {
        SVBasedonFilterPtr t_baseOn=MakeSharedPtr<SVBasedonFilter>(mApp);
        t_baseOn->create();
        t_picproc->addFilter(t_baseOn);
        t_picproc->openFilter(t_baseOn);
        if(m_lows==0){
            SVFairLtraLowPtr t_fair1=MakeSharedPtr<SVFairLtraLow>(mApp);
            t_fair1->create();
            t_picproc->addFilter(t_fair1);
            t_picproc->openFilter(t_fair1);
            SVFairDataPtr t_fair=MakeSharedPtr<SVFairData>(mApp);
            t_fair->create();
            t_picproc->addFilter(t_fair);
            t_picproc->openFilter(t_fair);
        }else if(m_lows==1){
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
//    SVFaceMattingFilterPtr t_face = MakeSharedPtr<SVFaceMattingFilter>(mApp);
//    t_face->create();
//    t_picproc->addFilter(t_face);
//    t_picproc->openFilter(t_face);
//    SVFaceShapeNodePtr m_shape=MakeSharedPtr<SVFaceShapeNode>(mApp);
//    m_shape->init();
//    SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
//    if(t_sc){
//        if(m_shape){
//            t_sc->addNode(m_shape);
//        }
//    }
//    
//    
//    SVFacePointNodePtr m_point=MakeSharedPtr<SVFacePointNode>(mApp);
//    m_point->init();
//    if(t_sc){
//        if(m_shape){
//            t_sc->addNode(m_point);
//        }
//    }
}
