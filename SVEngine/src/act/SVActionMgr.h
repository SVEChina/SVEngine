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
            
            void addAni(SVAniBasePtr _aniBase);
            
            bool removeAni(SVAniBasePtr _aniBase);
            
            bool removeAllAni();
            
            //以前写的接口，好多地方在用，先放着吧
            SVActionUnitPtr addAction(SVActBasePtr _action, SVNodePtr _node);
        
        protected:
            SVLockPtr m_lock;
            typedef SVArray<SVAniBasePtr> SVANIPOOL;
            SVANIPOOL m_aniPool;
        };
    }//!namespace logic
    
}//!namespace sv





#endif //SV_ACTIONMGR_H
