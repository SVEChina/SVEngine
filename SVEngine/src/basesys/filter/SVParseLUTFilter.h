//
//  SVParseLUTFilter.h
//  SVEngine
//
//  Created by 徐子昱 on 2018/10/23.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_PARSELUTFILTER_H
#define SV_PARSELUTFILTER_H

#include "../../file/SVParseData.h"

namespace sv {
    
    namespace logic{
        
        class SVParseLUTFilter : public SVParseData {
        public:
            static SVFilterBasePtr parseLUT(SVInst *_app,cptr8 _path, s32 resid);
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif /* SV_PARSELUTFILTER_H */
