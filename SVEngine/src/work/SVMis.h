//
// SVThread.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MIS_H
#define SV_MIS_H

//线程基类

#include "../base/SVObject.h"

namespace sv {

    class SVMis: public SVObject {
    public:
        SVMis();
        
        ~SVMis();
        
        //执行一次
        void exec(f32 _dt);

    };
    
//!namespace
}



#endif //SV_MIS_H
