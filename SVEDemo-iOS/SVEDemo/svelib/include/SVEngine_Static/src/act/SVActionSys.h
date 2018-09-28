//
// SVActionSys.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ACTIONSYS_H
#define SV_ACTIONSYS_H

#include "../basesys/SVSysBase.h"
#include "SVActDef.h"

namespace sv{
    namespace logic {
        
        class SVActionSys : public SVSysBase {
        public:
            SVActionSys(SVInst* _app);
            
            ~SVActionSys();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            bool hasActionUnit(SVActionUnitPtr _actUnit);
            
            void addActionUnit(SVActionUnitPtr _actUnit);
            
            bool removeActionUnit(SVActionUnitPtr _actUnit);
            
            void clearActionUnits();
            
        protected:
            SVLockPtr m_lock;
            typedef SVArray<SVActionUnitPtr> ANIUNITPOOL;
            ANIUNITPOOL m_aniUnitPool;
        };
    }//!namespace logic
    
}//!namespace sv





#endif //SV_ACTIONSYS_H
