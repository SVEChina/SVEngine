//
// SVRecycleModule.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RECYCLESYS_H
#define SV_RECYCLESYS_H

#include "../event/SVEventProc.h"

namespace sv {
    
    namespace logic {
        
        class SVRecycleModule : public SVEventProc {
        public:
            SVRecycleModule(SVInst* _app);
            
            ~SVRecycleModule();
            
            virtual bool procEvent(SVEventPtr _event);
        };

    }//!namespace logic
}//!namespace sv



#endif //SV_RECYCLESYS_H
