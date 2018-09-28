#ifndef SV_PARTICLENODE_H
#define SV_PARTICLENODE_H

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

            virtual s32 getFields(SVArray<Field> &fields);
    
            virtual s32 getForces(SVArray<Force> &forces);
    
            virtual s32 getNoises(SVArray<Noise> &noises);

            virtual s32 getIntersection(const FVec3 &p0,const FVec3 &p1,Contact &contact);

            virtual s32 getCollision(const FVec3 &point,const FVec3 &velocity,f32 radius,SVArray<Contact> &contacts,f32 ifps);

            virtual s32 getRandomPoint(FVec3 &ret_point,FVec3 &ret_normal,FVec3 &ret_velocity);
            
        private:
            SVParticlesNodePtr m_particleNode;
//            ShapeSphere sphere;
//            ShapeCapsule capsule;
        };
        
        //
        class SVParticlesNode : public SVNode {
        public:
            SVParticlesNode(SVInst *_app);
            
            ~SVParticlesNode();
            
            // particles
            SVParticlesPtr getParticles();
            
            // random seed
            void setSeed(u32 seed) const;
            u32 getSeed() const;
            
            // parameters
            void setParticlesType(s32 type) const;
            s32 getParticlesType() const;
            
            // warming particles
            void setWarming(s32 warning) const;
            s32 getWarming() const;
            
            // depth sorting
            void setDepthSort(s32 depth_sort) const;
            s32 getDepthSort() const;
            
            // orientation x
            void setVariationX(s32 variation) const;
            s32 getVariationX() const;
            
            // orientation y
            void setVariationY(s32 variation) const;
            s32 getVariationY() const;
            
            // texture atlas
            void setTextureAtlas(s32 atlas) const;
            s32 getTextureAtlas() const;
            
            // world intersection
            void setIntersection(s32 intersection) const;
            s32 getIntersection() const;
            
            // world collision
            void setCollision(s32 collision) const;
            s32 getCollision() const;
            
            // interaction culling
            void setCulling(s32 culling) const;
            s32 getCulling() const;
            
            // spawn rate
            void setSpawnRate(f32 spawn) const;
            f32 getSpawnRate() const;
            
            // spawn scale
            void setSpawnScale(f32 scale) const;
            f32 getSpawnScale() const;
            
            // spawn threshold
            void setSpawnThreshold(f32 threshold) const;
            f32 getSpawnThreshold() const;
            
            // particles
            void clearParticles() const;
            
            s32 getNumParticles() const;
            
            f32 getParticleRadius(s32 num) const;
            
            FVec3 getParticlePosition(s32 num) const;
            
            const FVec3 &getParticleVelocity(s32 num) const;
            
            void getParticleTransforms(SVArray<FMat4> &transforms) const;
            
            // physical mask
            void setPhysicalMask(s32 mask);
            s32 getPhysicalMask() const;
            
            // physical mass
            void setPhysicalMass(f32 mass) const;
            f32 getPhysicalMass() const;
            
            // length stretch
            void setLengthStretch(f32 stretch) const;
            f32 getLengthStretch() const;
            
            // linear damping
            void setLinearDamping(f32 damping) const;
            f32 getLinearDamping() const;
            
            // angular damping
            void setAngularDamping(f32 damping) const;
            f32 getAngularDamping() const;
            
            // growth damping
            void setGrowthDamping(f32 damping) const;
            f32 getGrowthDamping() const;
            
            // interaction restitution
            void setRestitution(f32 restitution) const;
            f32 getRestitution() const;
            
            // interaction roughness
            void setRoughness(f32 roughness) const;
            f32 getRoughness() const;
            
            // emitter
            void setEmitterType(s32 type) const;
            s32 getEmitterType() const;
            
            void setEmitterEnabled(s32 enable) const;
            s32 isEmitterEnabled() const;
            
            void setEmitterBased(s32 based) const;
            s32 isEmitterBased() const;
            
            void setEmitterShift(s32 shift) const;
            s32 isEmitterShift() const;
            
            void setEmitterContinuous(s32 continuous) const;
            s32 isEmitterContinuous() const;
            
            void setEmitterSequence(s32 sequence) const;
            s32 getEmitterSequence() const;
            
            void setEmitterLimit(s32 limit) const;
            s32 getEmitterLimit() const;
            
            void setEmitterSync(s32 sync);
            s32 getEmitterSync() const;
            
            void setEmitterSize(const FVec3 &size) const;
            const FVec3 &getEmitterSize() const;
            
            void setEmitterDirection(const FVec3 &direction) const;
            const FVec3 &getEmitterDirection() const;
            
            void setEmitterSpread(const FVec3 &spread) const;
            const FVec3 &getEmitterSpread() const;
            
            void setEmitterVelocity(const FVec3 &velocity);
            const FVec3 &getEmitterVelocity() const;
            
            void addEmitterSpark(const FVec3 &point,const FVec3 &normal,const FVec3 &velocity) const;
            
            // dynamic
            void setDelay(f32 mean,f32 spread) const;
            f32 getDelayMean() const;
            f32 getDelaySpread() const;
            
            void setPeriod(f32 mean,f32 spread) const;
            f32 getPeriodMean() const;
            f32 getPeriodSpread() const;
            
            void setDuration(f32 mean,f32 spread) const;
            f32 getDurationMean() const;
            f32 getDurationSpread() const;
            
            void setLife(f32 mean,f32 spread) const;
            f32 getLifeMean() const;
            f32 getLifeSpread() const;
            
            void setVelocity(f32 mean,f32 spread) const;
            f32 getVelocityMean() const;
            f32 getVelocitySpread() const;
            
            void setAngle(f32 mean,f32 spread) const;
            f32 getAngleMean() const;
            f32 getAngleSpread() const;
            
            void setRotation(f32 mean,f32 spread) const;
            f32 getRotationMean() const;
            f32 getRotationSpread() const;
            
            void setRadius(f32 mean,f32 spread) const;
            f32 getRadiusMean() const;
            f32 getRadiusSpread() const;
            
            void setGrowth(f32 mean,f32 spread) const;
            f32 getGrowthMean() const;
            f32 getGrowthSpread() const;
            
            // gravity
            void setGravity(const FVec3 &gravity) const;
            const FVec3 &getGravity() const;
            
            // forces
            s32 addForce() const;
            void removeForce(s32 num) const;
            s32 getNumForces() const;
            
            void setForceAttached(s32 num,s32 attached) const;
            s32 isForceAttached(s32 num) const;
            
            void setForceTransform(s32 num,const FMat4 &transform) const;
            FMat4 getForceTransform(s32 num) const;
            
            void setForceRadius(s32 num,f32 radius) const;
            f32 getForceRadius(s32 num) const;
            
            void setForceAttenuation(s32 num,f32 attenuation) const;
            f32 getForceAttenuation(s32 num) const;
            
            void setForceAttractor(s32 num,f32 attractor) const;
            f32 getForceAttractor(s32 num) const;
            
            void setForceRotator(s32 num,f32 rotator) const;
            f32 getForceRotator(s32 num) const;
            
            // noises
            s32 addNoise() const;
            void removeNoise(s32 num) const;
            s32 getNumNoises() const;
            
            void setNoiseAttached(s32 num,s32 attached) const;
            s32 isNoiseAttached(s32 num) const;
            
            void setNoiseTransform(s32 num,const FMat4 &transform) const;
            FMat4 getNoiseTransform(s32 num) const;
            
            void setNoiseOffset(s32 num,const FVec3 &offset) const;
            const FVec3 &getNoiseOffset(s32 num) const;
            
            void setNoiseStep(s32 num,const FVec3 &step) const;
            const FVec3 &getNoiseStep(s32 num) const;
            
            void setNoiseForce(s32 num,f32 damping) const;
            f32 getNoiseForce(s32 num) const;
            
            void setNoiseScale(s32 num,f32 scale) const;
            f32 getNoiseScale(s32 num) const;
            
            void setNoiseFrequency(s32 num,s32 frequency) const;
            s32 getNoiseFrequency(s32 num) const;
            
            void setNoiseSize(s32 num,s32 size) const;
            s32 getNoiseSize(s32 num) const;
            
            //Image *getNoiseImage(s32 num) const;
            
            // deflectors
            s32 addDeflector() const;
            void removeDeflector(s32 num) const;
            s32 getNumDeflectors() const;
            
            void setDeflectorType(s32 num,s32 type) const;
            s32 getDeflectorType(s32 num) const;
            
            void setDeflectorAttached(s32 num,s32 attached) const;
            s32 isDeflectorAttached(s32 num) const;
            
            void setDeflectorTransform(s32 num,const FMat4 &transform) const;
            FMat4 getDeflectorTransform(s32 num) const;
            
            void setDeflectorSize(s32 num,const FVec3 &size) const;
            const FVec3 &getDeflectorSize(s32 num) const;
            
            void setDeflectorRestitution(s32 num,f32 restitution) const;
            f32 getDeflectorRestitution(s32 num) const;
            
            void setDeflectorRoughness(s32 num,f32 roughness) const;
            f32 getDeflectorRoughness(s32 num) const;
            
            // contact points
            s32 getNumContacts() const;
            
            FVec3 getContactPoint(s32 num) const;
            
            const FVec3 &getContactNormal(s32 num) const;
            
            const FVec3 &getContactVelocity(s32 num) const;
            
//            Object *getContactObject(s32 num) const;
            
            // world offset vector
            const FVec3 &getWorldOffset() const;
            
            // surfaces
            virtual cptr8 getSurfaceName(s32 surface) const;
            
            // compares
            virtual s32 getSequence(const FVec3 &camera,s32 surface);
            virtual f32 getTransparentDistance(const FVec3 &camera,s32 surface);
            
            // random point
            virtual s32 getRandomPoint(FVec3 &ret_point,FVec3 &ret_normal,FVec3 &ret_velocity,s32 surface);
            
            // surface info
            virtual s32 getNumTriangles(s32 surface) const;
            
            // surface bounds
            virtual const SVBoundBox &getSVBoundBox(s32 surface) const;
            virtual const SVBoundSphere &getSVBoundSphere(s32 surface) const;
            
            // update
            virtual void update(f32 ifps);
            
            // bounds
            virtual const SVBoundBox &getSVBoundBox() const;
            virtual const SVBoundSphere &getSVBoundSphere() const;
            
            // render
//            virtual ObjectSurface *render(s32 pass,ObjectSurface *surface);
            
            // render query
            virtual s32 hasQuery() const;
            
            // visualizer
            virtual void renderHandler();
            virtual void renderVisualizer();
            
//            // clone object
//            virtual ObjectParticles *clone() const;
//
//            // load/save world
//            virtual s32 loadWorld(const Xml *xml);
//            virtual s32 saveWorld(Xml *xml) const;
//
//            // save/restore state
//            virtual s32 saveState(const Stream &stream) const;
//            virtual s32 restoreState(const Stream &stream);
            
        private:
            
            // update enabled
            virtual void update_enabled();
            
            // update position
            virtual void update_position();
            
            // update transformation
            virtual void update_transform();
            
//            // world bounds
//            virtual WorldSVBoundBox get_world_bound_box() const;
//
//            virtual WorldSVBoundSphere get_world_bound_sphere() const;
            
            // surfaces
            virtual s32 get_num_surfaces() const;
            
//            // copy object
//            ObjectParticles *copy(ObjectParticles *node) const;
            
            enum {
                SURFACE_PARTICLES = 0,
                NUM_SURFACES,
            };
            
            SVParticlesPtr m_pParticles;           // particles pointer
            
            SVParticlesWorldPtr m_pParticlesWorld;    // particles world
            
            s32 physical_mask;                        // physical mask
            
            s32 emitter_sync;                         // emitter synchronization
            
            FVec3 emitter_velocity;                    // emitter velocity
            
            FVec3 world_offset;                        // world offset
            
            s32 num_triangles;                        // number of triangles
            
            SVBoundBox bound_box;                        // bounding box
            
            SVBoundSphere bound_sphere;                // bounding sphere
        };

        
    }//!namespace node
    
}//!namespace sv

#endif /* SV_PARTICLENODE_H */
