//
//  SVParseDeform.h
//  SVEngine
//
//  Created by 徐子昱 on 2018/10/29.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_PARSEDEFORM_H
#define SV_PARSEDEFORM_H

#include "SVParseData.h"
//spine动画解析

namespace sv {
    
    namespace logic{
        
        class SVParseDeform : public SVParseData {
        public:
            static SVDeformImageMovePtr parseDeform(SVInst *app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path);
        };
        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PARSEDEFORM_H

