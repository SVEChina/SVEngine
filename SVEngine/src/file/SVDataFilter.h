//
// SVDataFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DATAFILTER_H
#define SV_DATAFILTER_H

#include "SVDataBase.h"

namespace sv {
    
    namespace logic{
        
        //滤镜数据
        class SVDataFilter : public SVDataBase{
        public:
            SVDataFilter(SVInst *_app);
            
            ~SVDataFilter();
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
            virtual SVNodePtr toNode();
            
            void refreshIn();
            
            void refreshOut();
            
            inline SVFilterGenLUTPtr getFilter(){
                return m_pFilterGenLUT;
            }
            
        protected:
            f32 m_beauty_rad;
            f32 m_shadow;
            f32 m_constrast;
            f32 m_saturation;
            f32 m_acutance;
            f32 m_brightness;
            f32 m_whitebalance;
            f32 m_hightlight;
            f32 m_gamma;
            f32 m_rshift;
            f32 m_gshift;
            f32 m_bshift;
            f32 m_sd_rshift;
            f32 m_sd_gshift;
            f32 m_sd_bshift;
            f32 m_hh_rshift;
            f32 m_hh_gshift;
            f32 m_hh_bshift;
            f32 m_temperature;
            f32 m_tint;
            SVFilterGenLUTPtr m_pFilterGenLUT;
        };
        

    }//!namespace logic
    
}//!namespace sv

#endif //SV_DATAFILTER_H
