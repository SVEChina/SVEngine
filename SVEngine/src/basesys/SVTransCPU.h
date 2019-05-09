//
// SVTransCPU.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_TRANS_CPU_H
#define SV_TRANS_CPU_H

#include "SVTrans.h"

namespace sv {

    namespace logic {
        
        class SVTransCPU : public SVTrans {
        public:
            SVTransCPU(SVInst *_app);
            
            virtual ~SVTransCPU();
            
            void init(s32 _w, s32 _h,f32 _angle,PICFORMATE _picformate,SVTEXTYPE _tt);
            
            void destroy();
            
            void update(f32 dt);
            
            void render();
        };
        
    } //!namespace logic
    
    

}//!namespace sv

#endif //SV_TRANS_CPU_H
