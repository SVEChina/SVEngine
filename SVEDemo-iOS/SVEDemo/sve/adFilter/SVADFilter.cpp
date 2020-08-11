//
// SVADFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVADFilter.h"
#include "node/SVNode.h"
#include "base/SVLock.h"
#include "base/SVPreDeclare.h"
#include "basesys/SVSceneMgr.h"
#include "node/SVScene.h"
#include "node/SVNode.h"
#include "app/SVGlobalMgr.h"
#include "file/SVFileMgr.h"
#include "basesys/SVBasicSys.h"
#include "basesys/SVPictureProcess.h"
#include "basesys/filter/SVADFilterBase.h"
#include "basesys/filter/SVFilterOilPaint.h"
#include "mtl/SVMtlShinning.h"
#include "mtl/SVMtlBlackWhite.h"
#include "mtl/SVMtlStreak.h"
#include "mtl/SVMtlRainbowColor.h"
#include "mtl/SVMtlFlash.h"
#include "mtl/SVMtlGlitch.h"
#include "mtl/SVMtlDistorted.h"
#include "mtl/SVMtlArenaLight.h"
#include "mtl/SVMtlWindowLight.h"
#include "mtl/SVMtlOilPaint.h"

SVADFilter::SVADFilter(SVInst* _app)
:SVModuleBase(_app){
    m_state = AD_FILTER_SHINNING_READY;
    m_accTime = 0.0;
    m_filter = nullptr;
    m_filterMtl = nullptr;
}

SVADFilter::~SVADFilter(){
    m_filter = nullptr;
    m_filterMtl = nullptr;
}

void SVADFilter::init(){
    SVModuleBase::init();

}

void SVADFilter::destroy(){
    SVModuleBase::destroy();
}

void SVADFilter::open(){
    SVModuleBase::open();
//    m_filterMtl = MakeSharedPtr<SVMtlOilPaint>(mApp);
//    m_filter = MakeSharedPtr<SVADFilterBase>(mApp, m_filterMtl);
    
    SVFilterOilPaintPtr oilPaint = MakeSharedPtr<SVFilterOilPaint>(mApp);
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if(t_picproc){
        oilPaint->create();
        t_picproc->addFilter(oilPaint);
        t_picproc->openFilter(oilPaint);
    }
}

void SVADFilter::close() {
    SVModuleBase::close();
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if( t_picproc) {
        t_picproc->clearFilter(m_filter);
    }
}

bool SVADFilter::isOpen(){
    return SVModuleBase::isOpen();
}

void SVADFilter::update(f32 _dt) {
    SVModuleBase::update(_dt);
    if (!m_filter) {
        return;
    }
    return;
    if (m_accTime == 0.0f) {
        if (m_state == AD_FILTER_SHINNING_READY) {
            m_state = AD_FILTER_SHINNING_BEGIN;
            m_filter->setVisible(true);
        }
    }
    if (m_accTime >= 5.0f) {
        if (m_state == AD_FILTER_GLITCH_READY) {
            m_state = AD_FILTER_GLITCH_BEGIN;
            m_filter->setVisible(true);
        }
    }
    if (m_accTime >= 6.0f && m_accTime < 9.0f) {
        m_filter->setVisible(false);
    }
    if (m_accTime >= 9.0f) {
        if (m_state == AD_FILTER_FLASH_READY) {
            m_state = AD_FILTER_FLASH_BEGIN;
            m_filter->setVisible(true);
        }
    }
    if (m_accTime >= 10.0f && m_accTime < 13.0f) {
        m_filter->setVisible(false);
    }
    if (m_accTime >= 13.0f) {
        if (m_state == AD_FILTER_STREAK_READY) {
            m_state = AD_FILTER_STREAK_BEGIN;
            m_filter->setVisible(true);
        }
    }
    if (m_accTime >= 18.0f && m_accTime < 21.0f) {
        m_filter->setVisible(false);
    }
    if (m_accTime >= 21.0f) {
        if (m_state == AD_FILTER_DISTORTED_READY) {
            m_state = AD_FILTER_DISTORTED_BEGIN;
            m_filter->setVisible(true);
        }
    }
    if (m_accTime >= 25.0f && m_accTime < 28.0f) {
        m_filter->setVisible(false);
    }
    if (m_state == AD_FILTER_SHINNING_BEGIN) {
        m_state = AD_FILTER_GLITCH_READY;
        m_filterMtl = MakeSharedPtr<SVMtlShinning>(mApp);
        m_filter->setFilterMtl(m_filterMtl);
    }
    if (m_state == AD_FILTER_GLITCH_BEGIN) {
        m_state = AD_FILTER_FLASH_READY;
        m_filterMtl = MakeSharedPtr<SVMtlGlitch>(mApp);
        m_filter->setFilterMtl(m_filterMtl);
    }
    if (m_state == AD_FILTER_FLASH_BEGIN) {
        m_state = AD_FILTER_STREAK_READY;
        m_filterMtl = MakeSharedPtr<SVMtlFlash>(mApp);
        m_filter->setFilterMtl(m_filterMtl);
    }
    if (m_state == AD_FILTER_STREAK_BEGIN) {
        m_state = AD_FILTER_DISTORTED_READY;
        m_filterMtl = MakeSharedPtr<SVMtlStreak>(mApp);
        m_filter->setFilterMtl(m_filterMtl);
    }
    if (m_state == AD_FILTER_DISTORTED_BEGIN) {
        m_state = AD_FILTER_SHINNING_READY;
        m_filterMtl = MakeSharedPtr<SVMtlDistorted>(mApp);
        m_filter->setFilterMtl(m_filterMtl);
    }
    m_accTime += _dt;
    if (m_accTime >= 28.0f) {
       m_accTime = 0.0f;
    }
}

bool SVADFilter::procEvent(SVEventPtr _event) {
    return  true;
}


