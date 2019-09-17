//
// SVPhysicsShape.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PHYSICSSHAPE_H
#define SV_PHYSICSSHAPE_H

#include "../../base/SVGBase.h"
#include "../../base/SVVec3.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
namespace sv {
    
    namespace logic {
        enum PHYSICSSHAPETYPE{
            E_PHYSICS_SHAPE_BASE = 0,
            E_PHYSICS_SHAPE_BOX,
            E_PHYSICS_SHAPE_SPHERE
        };
        class SVPhysicsShape : public SVGBase {
        public:
            SVPhysicsShape(SVInst* _app);
            
            ~SVPhysicsShape();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void update(f32 _dt);
            
            btCollisionShape* getShape();
            
            PHYSICSSHAPETYPE getType() const;
            
            inline void setMass(f32 _mass){
                m_mass = _mass;
            }
            
            inline f32 getMass(){
                return m_mass;
            }
            
            inline void setLocalInertia(FVec3 _m_localInertia){
                m_localInertia = _m_localInertia;
            }
            
            inline FVec3 getLocalInertia(){
                return m_localInertia;
            }
            
        protected:
            PHYSICSSHAPETYPE m_type;
            FVec3 m_size;
            f32 m_mass;
            FVec3 m_localInertia;
            btCollisionShape* m_pColShape;
            
        };
            
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PHYSICSSHAPE_H
