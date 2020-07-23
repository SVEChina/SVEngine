//
// SVOilPaintFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOilPaintFilter.h"
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
//#include "basesys/filter/SVOilPaintFilterBase.h"
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

SVOilPaintFilter::SVOilPaintFilter(SVInst* _app)
:SVModuleBase(_app){
    m_state = AD_FILTER_SHINNING_READY;
    m_accTime = 0.0;
    m_filter = nullptr;
    m_filterMtl = nullptr;
}

SVOilPaintFilter::~SVOilPaintFilter(){
    m_filter = nullptr;
    m_filterMtl = nullptr;
}

void SVOilPaintFilter::init(){
    SVModuleBase::init();

}

void SVOilPaintFilter::destroy(){
    SVModuleBase::destroy();
}

void SVOilPaintFilter::open(){
    SVModuleBase::open();
    m_filterMtl = MakeSharedPtr<SVMtlOilPaint>(mApp);
    m_filter = MakeSharedPtr<SVOilPaintFilterBase>(mApp, m_filterMtl);
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if(m_filter && t_picproc){
        m_filter->create();
        t_picproc->addFilter(m_filter);
        t_picproc->openFilter(m_filter);
    }
}

void SVOilPaintFilter::close() {
    SVModuleBase::close();
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if( t_picproc) {
        t_picproc->clearFilter(m_filter);
    }
}

bool SVOilPaintFilter::isOpen(){
    return SVModuleBase::isOpen();
}

void SVOilPaintFilter::update(f32 _dt) {
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

bool SVOilPaintFilter::procEvent(SVEventPtr _event) {
    return  true;
}


