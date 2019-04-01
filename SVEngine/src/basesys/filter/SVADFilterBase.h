//
// SVADFilterBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILTERADBASE_H
#define SV_FILTERADBASE_H

#include "SVFilterBase.h"

namespace sv {
    namespace logic {
        
        class SVADFilterBase : public SVFilterBase {
        public:
            SVADFilterBase(SVInst *_app, SVMtlCorePtr _mtl);
            
            ~SVADFilterBase();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            void setFilterMtl(SVMtlADFilterBasePtr _mtl);
            
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                        RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        protected:
            SVMtlCorePtr m_mtl;
        };
        
    };//!namespace logic
}//!namespace sv


#endif /* SV_FILTERADBASE_H */
