//
// SVBMFont.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BMFONT_H
#define SV_BMFONT_H

#include "SVFont.h"

namespace sv {
    
    class SVBMFont : public SVFont {
    public:
        SVBMFont(SVInst *_app);
        
        ~SVBMFont();
        
        void init(SVFontConfPtr _fontConf);
    };

    
}//!namespace sv



#endif //SV_BMFONT_H
