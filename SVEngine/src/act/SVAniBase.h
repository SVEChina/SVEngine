//
// SVAniBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ANIBASE_H
#define SV_ANIBASE_H

#include "../base/SVGBase.h"
#include "SVActDef.h"

namespace sv{
    
    namespace logic {
        
        class SVAniBase : public SVGBase {
        public:
            SVAniBase(SVInst* _app);
            
            ~SVAniBase();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void enter();
            
            virtual void exit();
            
            virtual bool isEnd();
            
            virtual void update(f32 _dt);
            
            void removeFromActionMgr();

        };
        
    }//!namespace logic
}//!namespace sv





#endif //SV_ANIBASE_H
