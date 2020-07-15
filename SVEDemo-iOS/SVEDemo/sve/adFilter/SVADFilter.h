//
// SVADFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ADFILTER_H
#define SV_ADFILTER_H
#include "module/SVGameBase.h"
#include "SVADFilterDef.h"
namespace sv{
    class SVADFilter : public SVModuleBase {
    public:
        enum ADFILTERSTATE{
            AD_FILTER_SHINNING_READY = 1,
            AD_FILTER_SHINNING_BEGIN,
            AD_FILTER_GLITCH_READY,
            AD_FILTER_GLITCH_BEGIN,
            AD_FILTER_FLASH_READY,
            AD_FILTER_FLASH_BEGIN,
            AD_FILTER_STREAK_READY,
            AD_FILTER_STREAK_BEGIN,
            AD_FILTER_DISTORTED_READY,
            AD_FILTER_DISTORTED_BEGIN
        };
        SVADFilter(SVInst* _app);
        
        ~SVADFilter();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool isOpen();
        
        virtual bool procEvent(SVEventPtr _event);
    protected:
        SVADFilterBasePtr m_filter;
        SVMtlADFilterBasePtr m_filterMtl;
        ADFILTERSTATE m_state;
        f32 m_accTime;
        f32 m_time;
    };
    
    
}//!namespace sv

#endif /* SV_ADFILTER_H */
