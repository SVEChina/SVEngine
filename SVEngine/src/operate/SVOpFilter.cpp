//
// SVOpFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpFilter.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPictureProcess.h"
#include "../basesys/filter/SVFilterBase.h"
#include "../basesys/filter/SVBasedonFilter.h"
#include "../basesys/filter/SVFilterDef.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVOpEvent.h"
#include "../base/SVDataSwap.h"
#include "../basesys/filter/SVParseLUTFilter.h"
#include "../basesys/filter/SVFilterLUT.h"
//设置美颜滤镜
SVOpSetBeautyFilter::SVOpSetBeautyFilter(SVInst* _app,cptr8 _scenename,cptr8 _filter,s32 _lows)
:SVOpBase(_app) {
    m_scenename = _scenename;
    m_filter = _filter;
    m_lows=_lows;
}

void SVOpSetBeautyFilter::_process(f32 _dt) {
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if( t_picproc ) {
//        SVBasedonFilterPtr t_baseOn=MakeSharedPtr<SVBasedonFilter>(mApp);
//        t_baseOn->create();
//        t_picproc->addFilter(t_baseOn);
//        t_picproc->openFilter(t_baseOn);
//        if(m_lows==0){
//            SVFairDataPtr t_fair=MakeSharedPtr<SVFairData>(mApp);
//            t_fair->create();
//            t_picproc->addFilter(t_fair);
//            t_picproc->openFilter(t_fair);
//        }else if(m_lows==1){
//            SVFairDataBlurPtr t_fair=MakeSharedPtr<SVFairDataBlur>(mApp);
//            t_fair->create();
//            t_picproc->addFilter(t_fair);
//            t_picproc->openFilter(t_fair);
//        }else if(m_lows==2){
//            SVFairLtraLowPtr t_fair=MakeSharedPtr<SVFairLtraLow>(mApp);
//            t_fair->create();
//            t_picproc->addFilter(t_fair);
//            t_picproc->openFilter(t_fair);
//        }
    }
}

//设置美颜滤镜
SVOpUpdateFilterSmooth::SVOpUpdateFilterSmooth(SVInst* _app,f32 _smooth,u32 _type)
:SVOpBase(_app) {
    m_smooth=_smooth;
    m_type=_type;
}

void SVOpUpdateFilterSmooth::_process(f32 _dt) {
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if( t_picproc ) {
        t_picproc->setFilterParam(m_smooth,(SVFILTERITEMTYPE)m_type);
    }
}

//设置美颜滤镜
SVOpUpdateBSplineFilter::SVOpUpdateBSplineFilter(SVInst* _app,SVDataSwapPtr _pDataSwap)
:SVOpBase(_app) {
    m_pDataSwap=_pDataSwap;
}

void SVOpUpdateBSplineFilter::_process(f32 _dt) {
//    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
//    if( t_picproc ) {
//        t_picproc->setFilterBSplineData((ptru8)m_pDataSwap->getData());
//
//    }
}

//关闭美颜滤镜
SVOpCloseBeautyFilter::SVOpCloseBeautyFilter(SVInst *_app)
:SVOpBase(_app) {
}

void SVOpCloseBeautyFilter::_process(f32 _dt) {
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if( t_picproc ) {
        t_picproc->clearFilter(SV_FUNC_BEAUTY);
    }
}

//
SVOpShapeFaceSmoothFilter::SVOpShapeFaceSmoothFilter(SVInst* _app,f32 _face,f32 _eye)
:SVOpBase(_app) {
    m_face = _face;
    m_eye=_eye;
}

void SVOpShapeFaceSmoothFilter::_process(f32 _dt) {
    SVFaceShapeEventPtr t_event = MakeSharedPtr<SVFaceShapeEvent>();
    t_event->m_faceSmooth = m_face;
    t_event->m_eyeSmooth = m_eye;
    mApp->getEventMgr()->pushEvent(t_event);
}

SVOpLUTFilterCreate::SVOpLUTFilterCreate(SVInst* _app,cptr8 _scenename,cptr8 _path,s32 resid)
:SVOpBase(_app){
     m_path = _path;
     m_resid = resid;
}

void SVOpLUTFilterCreate::_process(f32 _dt){
    SVParseLUTFilter t_filter(mApp);
    SVFilterLUTPtr t_lutFilter = t_filter.parse(m_path, m_resid);
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if( t_picproc&&t_lutFilter) {
        t_lutFilter->create();
        t_picproc->addFilter(t_lutFilter);
        t_picproc->openFilter(t_lutFilter);
    }
}

SVOpLUTFilterDestory::SVOpLUTFilterDestory(SVInst* _app,cptr8 _scenename,s32 resid)
:SVOpBase(_app){
    
}

void SVOpLUTFilterDestory::_process(f32 _dt){
    
}

