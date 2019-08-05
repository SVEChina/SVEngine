//
// SVJsonTool.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_JSONTOOL_H
#define SV_JSONTOOL_H

#include "../base/SVGBase.h"
namespace sv {
    
    namespace logic{
        
        //数据基准类型
        class SVJsonTool : public SVGBase{
        public:
            SVJsonTool(SVInst *_app);
            
            ~SVJsonTool();
            
            static SVString jsonFormat(const SVString inString);
        };
        
        
    }//!namespace logic
    
}//!namespace sv

#endif //SV_JSONTOOL_H
