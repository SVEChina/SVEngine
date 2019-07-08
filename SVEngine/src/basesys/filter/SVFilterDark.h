//
// SVFilterDark.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILTERDARK_H
#define SV_FILTERDARK_H

#include "SVFilterBase.h"

namespace sv {
    namespace logic {
        
        class SVFilterDark : public SVFilterBase {
        public:
            SVFilterDark(SVInst *_app);
            
            ~SVFilterDark();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            void setDarkIntensity(f32 _intensity);
            
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                        RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        protected:
            SVMtlDarkPtr m_mtlDark;
            f32 m_intensity;
        };
        
    };//!namespace logic
}//!namespace sv


#endif /* SV_FILTERDARK_H */
