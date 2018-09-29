#ifndef SV_PARTICLE_WORLD_H
#define SV_PARTICLE_WORLD_H

#include "SVNode.h"
#include "../base/SVBounds.h"
#include "../base/SVVec3.h"
#include "../core/SVParticles.h"

namespace sv {
    
    namespace node{
    
        //
        class SVParticlesWorld : public SVParticlesWorldBase {
        public:
            SVParticlesWorld();
            
            virtual ~SVParticlesWorld();
            
            virtual void setEmitterEnabled(s32 enable);

            virtual s32 getFields(SVArray<WorldField> &fields);
    
            virtual s32 getForces(SVArray<WorldForce> &forces);
    
            virtual s32 getNoises(SVArray<WorldNoise> &noises);

            virtual s32 getIntersection(const FVec3 &p0,const FVec3 &p1,WorldContact &contact);

            virtual s32 getCollision(const FVec3 &point,const FVec3 &velocity,f32 radius,SVArray<WorldContact> &contacts,f32 ifps);

            virtual s32 getRandomPoint(FVec3 &ret_point,FVec3 &ret_normal,FVec3 &ret_velocity);
            
        private:
            SVParticlesNodePtr m_particleNode;
        };
        
    }//!namespace node
    
}//!namespace sv

#endif /* SV_PARTICLE_WORLD_H */
