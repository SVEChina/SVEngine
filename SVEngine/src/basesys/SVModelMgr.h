//
// SVModelMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODELMGR_H
#define SV_MODELMGR_H

#include "../basesys/SVSysBase.h"
#include "../base/SVMap.h"
namespace sv {
        class SVModelMgr : public SVSysBase{
        public:
            SVModelMgr(SVInst* _app);
            
            ~SVModelMgr();
            
            void init();
            
            void update(f32 _dt);
            
            void destroy();
            
            void addModel(SVModelPtr _model);
            
            SVModelPtr getModel(SVString _name);
        protected:
            bool _hasModel(SVModelPtr _model);
            void _recycleUnuseModel();
            typedef SVArray<SVModelPtr> MODELPOOL;
            MODELPOOL m_modelPool;
            SVLockPtr m_lock;
        };
}//!namespace sv



#endif //SV_MODELMGR_H
