//
// SVPhysicsBodyRigid.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSBODYRIGID_H
#define SV_PHYSICSBODYRIGID_H
#include "SVPhysicsBody.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsBodyRigid : public SVPhysicsBody {
        public:
            SVPhysicsBodyRigid(SVInst* _app);
            
            SVPhysicsBodyRigid(SVInst* _app , SVPhysicsShapePtr _shape);
            
            ~SVPhysicsBodyRigid();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void update(f32 _dt);
            
            void setApplyCentralForce(FVec3 _pos);
            
            void setApplyTorque(FVec3 _pos);
            
            void addConstraint();
            
            void setConstraintXY(f32 _x ,f32 _y ,f32 _z);
            
            void removeConstraint();
            
            void setRestitution(f32 _dis);
            
            void setFriction(f32 _dis);
            
            void setBindNode(bool isbind);
            
            PHYSICSBODYTYPE getType();
            
        protected:
            
            PHYSICSBODYTYPE m_type;
            
            SVPhysicsShapePtr m_pShape;
            
            btDefaultMotionState* m_pMyMotionState;
            
            btPoint2PointConstraint* p2p;
            
            bool m_isBindNode;
            
            s32 m_savedState;
            
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSBODYRIGID_H
