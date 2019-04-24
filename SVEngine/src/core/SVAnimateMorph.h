//
// SVAnimateSkin.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ANIMATE_MORPH_H
#define SV_ANIMATE_MORPH_H

#include "../base/SVObject.h"
#include "../base/SVVec3.h"


namespace sv {
    
    namespace util{
        
        /*
            morph 动画
         */
        
        class SVAnimateMorph : public SVObject{
        public:
            SVAnimateMorph();

            ~SVAnimateMorph();

        };

    }//namespace util
    
}//!namespace sv


#endif //SV_ANIMATE_MORPH_H
