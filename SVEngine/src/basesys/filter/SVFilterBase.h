//
//  SVFilterBase.hpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_FILTER_BASE_H
#define SV_FILTER_BASE_H

#include "SVFilterDef.h"
#include "../../base/SVGBase.h"
#include "../../mtl/SVMtlDeclare.h"
#include "../../node/SVNodeDeclare.h"
#include "../../third/rapidjson/rapidjson.h"

namespace sv {
    
    namespace logic {
        
        class SVFilterBase : public SVGBase {
        public:
            SVFilterBase(SVInst *_app);
            
            ~SVFilterBase();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            virtual SVNodePtr getNode();
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);
            
            //序列化
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
            inline SVString getName(){
                return m_name;
            }
            
            inline SVFILTERFUNCTYPE getType(){
                return m_type;
            }

        protected:
            SVMultPassNodePtr m_pPassNode;
            SVMtlCorePtr m_mtl;
            SVString m_name;
            SVFILTERFUNCTYPE m_type;
        };
        
    };//!namespace logic
    
}//!namespace sv


#endif /* SV_FILTER_BASE_H */
