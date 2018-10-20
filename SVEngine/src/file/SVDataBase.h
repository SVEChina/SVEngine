//
// SVDataNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DATABASE_H
#define SV_DATABASE_H

#include "../base/SVGBase.h"
#include "SVParseDef.h"

namespace sv {
    
    namespace logic{
        
        enum MODELDATATYPE{
            MODELDATA_NONE = 0,
            MODELDATA_SPINE,
            MODELDATA_BITFONT,
            MODELDATA_SPRITE,
            MODELDATA_MASK,
            MODELDATA_FILTER,
            MODELDATA_DEFORM,
            MODELDATA_BMFONT
        };
        
        //数据基准类型
        class SVDataBase : public SVGBase{
        public:
            SVDataBase(SVInst *_app);
            
            ~SVDataBase();
            
            static SVString jsonFormat(const SVString inString);
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);

        protected:
            
            void _toJsonData(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &locationObj);
            
            void _fromJsonData(RAPIDJSON_NAMESPACE::Value &item);
            
            MODELDATATYPE m_dataType;
        };
        
        
    }//!namespace logic
    
}//!namespace sv

#endif //SV_DATABASE_H
