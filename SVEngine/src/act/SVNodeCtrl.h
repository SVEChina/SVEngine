//
// SVNodeCtrl.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_NODECTRL_H
#define SV_NODECTRL_H

#include "SVActDef.h"
#include "../base/SVGBase.h"
#include "../base/SVVec3.h"
#include "../node/SVNodeDeclare.h"

namespace sv {
    
    namespace logic {
        
        class SVNodeCtrl : public SVGBase {
        public:
            SVNodeCtrl(SVInst* _app);
            
            ~SVNodeCtrl();
            
            virtual bool run(SVNodePtr _nodePtr, f32 dt);  //循环action
            
        protected:
            
        };

        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_NODECTRL_H
