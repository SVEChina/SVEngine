//
// SVActionMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTIONMGR_H
#define SV_ACTIONMGR_H

#include "../basesys/SVSysBase.h"
#include "SVActDef.h"

namespace sv{
    namespace logic {
        
        class SVActionMgr : public SVSysBase {
        public:
            SVActionMgr(SVInst* _app);
            
            ~SVActionMgr();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            SVActionUnitPtr addAction(SVActBasePtr _action, SVNodePtr _node);
            
            SVActionUnitPtr addAction(SVActionUnitPtr _actUnit);
            
            bool removeAction(SVActionUnitPtr _actUnit);
            
            bool removeAllActions();
        protected:
            SVLockPtr m_lock;
            typedef SVArray<SVActionUnitPtr> SVANIPOOL;
            SVANIPOOL m_aniPool;
        };
    }//!namespace logic
    
}//!namespace sv





#endif //SV_ACTIONMGR_H
