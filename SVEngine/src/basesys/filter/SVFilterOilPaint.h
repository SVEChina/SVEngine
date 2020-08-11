//
// SVFilterOilPaint.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILTEROILPAINT_H
#define SV_FILTEROILPAINT_H

#include "SVFilterBase.h"

namespace sv {
    namespace logic {
        
        class SVFilterOilPaint : public SVFilterBase {
        public:
            SVFilterOilPaint(SVInst *_app);
            
            ~SVFilterOilPaint();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                        RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };
        
    };//!namespace logic
}//!namespace sv


#endif /* SV_FILTEROILPAINT_H */
