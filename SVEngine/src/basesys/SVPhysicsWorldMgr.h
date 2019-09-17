//
// SVPhysicsWorldMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BASESYS_PHYSICSWORLDMGR_H
#define SV_BASESYS_PHYSICSWORLDMGR_H

#include "../basesys/SVSysBase.h"
namespace sv{
        class SVPhysicsWorldMgr : public SVSysBase {
        public:
            SVPhysicsWorldMgr(SVInst* _app);
            
            ~SVPhysicsWorldMgr();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            void setRigidWorld(SVPhysicsWorldPtr _world);
            
            void setSoftWorld(SVPhysicsSoftRigidWorldPtr _world);
            
            SVPhysicsWorldPtr getRigidWorld();
            
            SVPhysicsSoftRigidWorldPtr getSoftWorld();
        protected:
            SVPhysicsWorldPtr m_rigidWorld;
            SVPhysicsSoftRigidWorldPtr m_softWorld;
        };
}//!namespace sv
#endif //SV_BASESYS_PHYSICSWORLDMGR_H
