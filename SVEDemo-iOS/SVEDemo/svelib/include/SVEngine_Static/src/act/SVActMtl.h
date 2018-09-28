//
// SVActMtl.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTIONMTL_H
#define SV_ACTIONMTL_H

#include "SVActTime.h"
#include "../mtl/SVMtlCore.h"

//替换材质的action

namespace sv {
    
    namespace logic {
        
        class SVActMtl : public SVActTime {
        public:
            SVActMtl(SVInst *_app);
            
            ~SVActMtl();
            
            void enter(SVNodePtr _node);
            
            void exit(SVNodePtr _node);
            
        protected:
            SVMtlCorePtr m_pNewMtl;
            SVMtlCorePtr m_pOldMtl;
        };
        
    }//!namespace node
    
}//!namespace sv




#endif //SV_ACTIONMTL_H
