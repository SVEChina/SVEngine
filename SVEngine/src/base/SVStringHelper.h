//
// SVStringHelper.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_JSONTOOL_H
#define SV_JSONTOOL_H

#include "SVObject.h"
namespace sv {
    
    namespace logic{
        
        //数据基准类型
        class SVStringHelper : public SVObject{
        public:
            SVStringHelper();
            
            ~SVStringHelper();
            
            static SVString jsonFormat(const SVString inString);
            
            static void encrypt(c8 *_str, s32 *_keys, u32 _keyCount);
            
            static void decrypt(c8 *_str, s32 *_keys, u32 _keyCount);
        };
        
        
    }//!namespace logic
    
}//!namespace sv

#endif //SV_JSONTOOL_H
