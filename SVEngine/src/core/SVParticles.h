//
// SVParticles.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARTICLES_H
#define SV_PARTICLES_H

#include "../base/SVObject.h"
#include "../base/SVArray.h"
#include "../base/SVVec3.h"
#include "../base/SVMat4.h"
#include "../base/SVArray.h"
#include "../base/SVRandom.h"
#include "../base/SVBounds.h"
#include "../core/SVVertDef.h"
#include "../third/rapidjson/rapidjson.h"
#include "../third/rapidjson/document.h"

namespace sv {
    
    namespace util{
        
        // 粒子类型
        enum PARTICLETYPE {
            TYPE_BILLBOARD = 0,
            TYPE_FLAT,
            TYPE_POINT,
            TYPE_LENGTH,
            TYPE_RANDOM,
            TYPE_ROUTE,
            TYPE_CHAIN,
            NUM_TYPES,
        };
        
        // 粒子发射器类型
        enum EMITTERTYPE {
            EMITTER_POINT = 0,
            EMITTER_SPHERE,
            EMITTER_CYLINDER,
            EMITTER_BOX,
            EMITTER_RANDOM,
            EMITTER_SPARK,
            NUM_EMITTERS,
        };
        
        // deflectors
        enum {
            DEFLECTOR_REFLECTOR = 0,
            DEFLECTOR_CLIPPER,
        };
        
        struct Particle {
            FVec3 position;                      // position
            FVec3 old_position;                  // old position
            FVec3 parameters;                    // parameters
            FVec3 velocity;                      // velocity
            FVec4 old_color;                     // old color
            FVec4 color;                         // color
            f32 angle;                           // angle
            f32 rotation;                        // rotation
            f32 radius;                          // radius
            f32 growth;                          // growth
            f32 life;                            // life
            f32 ilife;                           // inverse life
            s32 orientation;                     // orientation
        };
        
        // field
        struct WorldField {
            FMat4 transform;            // transformation
            FVec3 hsize;                // half size
            FVec3 velocity;             // velocity
            f32 damping;                // damping
        };
        
        // force
        struct WorldForce {
            FVec3 position;             // position
            FVec3 axis;                 // axis
            f32 iradius;                // inverse radius
            f32 attenuation;            // attenuation
            f32 attractor;              // attractor
            f32 rotator;                // rotator
        };
        
        // noise
        struct WorldNoise {
            FMat4 itransform;           // transformation
            f32 force;                  // noise force
            //Image *image;             / image pointer
        };
        
        // contact
        struct WorldContact {
            FVec3 point;                // point
            FVec3 normal;               // normal
            f32 time;                   // time of impact
            f32 depth;                  // penetration depth
            f32 restitution;            // coefficient of restitution
            f32 roughness;              // coefficient of roughness
            void *data;                 // data pointer
        };

        //强制
        struct Force {
            FMat4 transform;                    // transformation
            FVec3 position;                     // force position
            FVec3 axis;                         // force axis
            s32 attached;                       // attached flag
            f32 radius;                         // radius
            f32 iradius;                        // inverse radius
            f32 attenuation;                    // attenuation
            f32 attractor;                      // attractor
            f32 rotator;                        // rotator
        };
        //噪声
        struct Noise {
            FMat4 transform;                     // transformation
            FMat4 itransform;                    // inverse transformation
            FVec3 offset;                        // noise offset
            FVec3 step;                          // noise step
            s32 attached;                        // attached flag
            f32 force;                           // noise force
            f32 scale;                           // noise scale
            s32 frequency;                       // frequency
            s32 size;                            // noise size
            //Image *image;                      // image pointer
        };
        //扭曲
        struct Deflector {
            FMat4 transform;                        // transformation
            FVec3 size;                            // deflector size
            s32 type;                            // deflector type
            s32 attached;                        // attached flag
            f32 restitution;                    // restitution
            f32 roughness;                    // roughness
        };
        
        struct Distance {
            f32 distance;                        // distance to the camera
            s32 index;                           // particle index
        };
        
        //火花
        struct Spark {
            FVec3 point;                            // spark point
            FVec3 normal;                           // spark normal
            FVec3 velocity;                         // spark velocity
        };
        //接触点
        struct Contact {
            FVec3 point;                         // contact point
            FVec3 normal;                        // contact normal
            FVec3 velocity;                      // contact velocity
            void *data;                          // contact data pointer
        };
        
        //
        class SVParticlesWorldBase :public SVObject {
        public:
            SVParticlesWorldBase() {
            }

            ~SVParticlesWorldBase() {
            }
            
            // emitter
            virtual void setEmitterEnabled(s32 enable){ };
            
            // fields
            virtual s32 getFields(SVArray<WorldField> &fields){ return 0; };
            
            // forces
            virtual s32 getForces(SVArray<WorldForce> &forces){ return 0; };
            
            // noises
            virtual s32 getNoises(SVArray<WorldNoise> &noises){ return 0; };
            
            // intersection
            virtual s32 getIntersection(const FVec3 &p0,const FVec3 &p1,WorldContact &contact){ return 0; };
            
            // collision
            virtual s32 getCollision(const FVec3 &point,const FVec3 &velocity,f32 radius,SVArray<WorldContact> &contacts,f32 ifps){ return 0; };
            
            // random point
            virtual s32 getRandomPoint(FVec3 &ret_point,FVec3 &ret_normal,FVec3 &ret_velocity){ return 0; };
        };
        
        //一堆粒子算法
        class SVParticles :public SVObject {
            friend struct ParticlesDistanceCompare;
            friend class SVParticlesNode;
        public:
            SVParticles();
            
            virtual ~SVParticles();

            // clear particles
            void clear();
            
            // update particles
            virtual void update(f32 ifps);
            
            //更新发射器
            void updateEmitter();
            
            // render destroy
            virtual void destroy();
            
            // render particles
            virtual s32 render(const FMat4 &modelview,const FVec3 &camera);
            
            //世界
            void setWorld(SVParticlesWorldBasePtr _world);
            
            SVParticlesWorldBasePtr getWorld();
        
        public:
            //数据
            V3_PARTICLE* pVertex;
            
            u32 m_vertexBufNum;

            //
            s32 getParticleNum();
            
            // random seed
            void setSeed(u32 seed);
            u32 getSeed() const;

            // particles type
            void setType(s32 type);
            s32 getType() const;
            
            // warming particles
            void setWarming(s32 warming);
            s32 getWarming() const;
            
            // depth sorting
            void setDepthSort(s32 depth_sort);
            s32 getDepthSort() const;
            
            // orientation x
            void setVariationX(s32 variation);
            s32 getVariationX() const;
            
            // orientation y
            void setVariationY(s32 variation);
            s32 getVariationY() const;
            
            // texture atlas
            void setTextureAtlas(s32 atlas);
            s32 getTextureAtlas() const;
            
            // world intersection
            void setIntersection(s32 intersection);
            s32 getIntersection() const;
            
            // world collision
            void setCollision(s32 collision);
            s32 getCollision() const;
            
            // interaction culling
            void setCulling(s32 culling);
            s32 getCulling() const;
            
            // spawn rate
            void setSpawnRate(f32 rate);
            f32 getSpawnRate() const;
            
            // spawn scale
            void setSpawnScale(f32 scale);
            f32 getSpawnScale() const;
            
            // spawn threshold
            void setSpawnThreshold(f32 threshold);
            f32 getSpawnThreshold() const;
            
            // world mass
            void setWorldMass(f32 mass);
            f32 getWorldMass() const;
            
            // length stretch
            void setLengthStretch(f32 stretch);
            f32 getLengthStretch() const;
            
            // linear damping
            void setLinearDamping(f32 damping);
            f32 getLinearDamping() const;
            
            // angular damping
            void setAngularDamping(f32 damping);
            f32 getAngularDamping() const;
            
            // growth damping
            void setGrowthDamping(f32 damping);
            f32 getGrowthDamping() const;
            
            // interaction restitution
            void setRestitution(f32 restitution);
            f32 getRestitution() const;
            
            // interaction roughness
            void setRoughness(f32 roughness);
            f32 getRoughness() const;
            
            //发射器
            void setEmitterType(s32 type);
            s32 getEmitterType() const;
            
            void setEmitterEnabled(s32 enable);
            s32 isEmitterEnabled() const;
            
            void setEmitterBased(s32 based);
            s32 isEmitterBased() const;
            
            void setEmitterShift(s32 shift);
            s32 isEmitterShift() const;
            
            void setEmitterContinuous(s32 continuous);
            s32 isEmitterContinuous() const;
            
            void setEmitterSequence(s32 sequence);
            s32 getEmitterSequence() const;
            
            void setEmitterLimit(s32 limit);
            s32 getEmitterLimit() const;
            
            void setEmitterSize(const FVec3 &size);
            const FVec3 &getEmitterSize() const;
            
            void setEmitterDirection(const FVec3 &direction);
            const FVec3 &getEmitterDirection() const;
            
            void setEmitterSpread(const FVec3 &spread);
            const FVec3 &getEmitterSpread() const;
            
            void setEmitterVelocity(const FVec3 &velocity);
            const FVec3 &getEmitterVelocity() const;
            
            void setEmitterTransform(const FMat4 &transform);
            const FMat4 &getEmitterTransform() const;
            
            void addEmitterSpark(const FVec3 &point,const FVec3 &normal,const FVec3 &velocity);
            
            // dynamic
            void setDelay(f32 mean,f32 spread);
            f32 getDelayMean() const;
            f32 getDelaySpread() const;
            
            void setPeriod(f32 mean,f32 spread);
            f32 getPeriodMean() const;
            f32 getPeriodSpread() const;
            
            void setDuration(f32 mean,f32 spread);
            f32 getDurationMean() const;
            f32 getDurationSpread() const;
            
            void setLife(f32 mean,f32 spread);
            f32 getLifeMean() const;
            f32 getLifeSpread() const;
            
            void setVelocity(f32 mean,f32 spread);
            f32 getVelocityMean() const;
            f32 getVelocitySpread() const;
            
            void setAngle(f32 mean,f32 spread);
            f32 getAngleMean() const;
            f32 getAngleSpread() const;
            
            void setRotation(f32 mean,f32 spread);
            f32 getRotationMean() const;
            f32 getRotationSpread() const;
            
            void setRadius(f32 mean,f32 spread);
            f32 getRadiusMean() const;
            f32 getRadiusSpread() const;
            
            void setFade(f32 _fade);
            f32 getFade();
            
            void setGrowth(f32 mean,f32 spread);
            f32 getGrowthMean() const;
            f32 getGrowthSpread() const;
            
            // gravity
            void setGravity(const FVec3 &gravity);
            const FVec3 &getGravity() const;
            
            // forces
            s32 addForce();
            void removeForce(s32 num);
            s32 getNumForces() const;
            
            void setForceAttached(s32 num,s32 attached);
            s32 isForceAttached(s32 num) const;
            
            void setForceTransform(s32 num,const FMat4 &transform);
            const FMat4 &getForceTransform(s32 num) const;
            
            void setForceRadius(s32 num,f32 radius);
            f32 getForceRadius(s32 num) const;
            
            void setForceAttenuation(s32 num,f32 attenuation);
            f32 getForceAttenuation(s32 num) const;
            
            void setForceAttractor(s32 num,f32 attractor);
            f32 getForceAttractor(s32 num) const;
            
            void setForceRotator(s32 num,f32 rotator);
            f32 getForceRotator(s32 num) const;
            
            // noises
            s32 addNoise();
            void removeNoise(s32 num);
            s32 getNumNoises() const;
            
            void setNoiseAttached(s32 num,s32 attached);
            s32 isNoiseAttached(s32 num) const;
            
            void setNoiseTransform(s32 num,const FMat4 &transform);
            const FMat4 &getNoiseTransform(s32 num) const;
            
            void setNoiseOffset(s32 num,const FVec3 &offset);
            const FVec3 &getNoiseOffset(s32 num) const;
            
            void setNoiseStep(s32 num,const FVec3 &step);
            const FVec3 &getNoiseStep(s32 num) const;
            
            void setNoiseForce(s32 num,f32 force);
            f32 getNoiseForce(s32 num) const;
            
            void setNoiseScale(s32 num,f32 scale);
            f32 getNoiseScale(s32 num) const;
            
            void setNoiseFrequency(s32 num,s32 frequency);
            s32 getNoiseFrequency(s32 num) const;
            
            void setNoiseSize(s32 num,s32 size);
            s32 getNoiseSize(s32 num) const;
            
            //Image *getNoiseImage(s32 num);
            
            s32 addDeflector();
            void removeDeflector(s32 num);
            s32 getNumDeflectors() const;
            
            void setDeflectorType(s32 num,s32 type);
            s32 getDeflectorType(s32 num) const;
            
            void setDeflectorAttached(s32 num,s32 attached);
            s32 isDeflectorAttached(s32 num) const;
            
            void setDeflectorTransform(s32 num,const FMat4 &transform);
            const FMat4 &getDeflectorTransform(s32 num) const;
            
            void setDeflectorSize(s32 num,const FVec3 &size);
            const FVec3 &getDeflectorSize(s32 num) const;
            
            void setDeflectorRestitution(s32 num,f32 restitution);
            f32 getDeflectorRestitution(s32 num) const;
            
            void setDeflectorRoughness(s32 num,f32 roughness);
            f32 getDeflectorRoughness(s32 num) const;
            
            // particles
            sv_inline s32 getNumParticles() const {
                return m_particles.size();
            }
            
            sv_inline const FVec3 &getParticlePosition(s32 num) const {
                return m_particles[num].position;
            }
            
            sv_inline const FVec3 &getParticleOldPosition(s32 num) const {
                return m_particles[num].old_position;
            }
            
            sv_inline const FVec3 &getParticleParameters(s32 num) const {
                return m_particles[num].parameters;
            }
            
            sv_inline const FVec3 &getParticleVelocity(s32 num) const {
                return m_particles[num].velocity;
            }
            
            sv_inline f32 getParticleRadius(s32 num) const {
                return m_particles[num].radius;
            }
            
            sv_inline f32 getParticleLife(s32 num) const {
                return m_particles[num].life;
            }
            
            // contact points
            sv_inline s32 getNumContacts() const {
                return old_contacts.size();
            }
            
            sv_inline void *getContactData(s32 num) const {
                return old_contacts[num].data;
            }
            
            sv_inline const FVec3 &getContactPoint(s32 num) const {
                return old_contacts[num].point;
            }
            
            sv_inline const FVec3 &getContactNormal(s32 num) const {
                return old_contacts[num].normal;
            }
            
            sv_inline const FVec3 &getContactVelocity(s32 num) const {
                return old_contacts[num].velocity;
            }
            
            // bounds
            sv_inline const SVBoundBox &getSVBoundBox() const {
                return bound_box;
            }
            
            sv_inline const SVBoundSphere &getSVBoundSphere() const {
                return bound_sphere;
            }
        protected:
            // update particles
            void spawn_particle(Particle &p,f32 k,f32 ifps);
            
            void spawn_particles(s32 num_particles,f32 offset,f32 ifps,f32 time);
            
            virtual void update_particles(SVArray<WorldField> &world_fields,
                                          SVArray<WorldForce> &world_forces,
                                          SVArray<WorldNoise> &world_noises,
                                          s32 iteration,
                                          f32 ifps);
            
            virtual void update_particles_bounds();
            
            virtual void update_bounds();
            
            // create render buffers
            void sort_particles(const FVec3 &camera);
            
            void create_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera);
            
            void create_billboard_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera);
            
            void create_oriented_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera);
            
            void create_point_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera);
            
            void create_length_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera);
            
            void create_route_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera);
            
            void create_chain_particles(V3_PARTICLE *vertex,const FMat4 &modelview,const FVec3 &camera);
            
            SVRandom random;                        // random number generator
            
            SVParticlesWorldBasePtr m_pWorld;       // world interaction
            
            s32 frame;                              // 帧数
            
            s8 type;                                // particles type
            s8 warming;                             // warming particles
            s8 depth_sort;                          // sort by depth
            s8 variation_x;                         // orientation x
            s8 variation_y;                         // orientation y
            s8 texture_atlas;                       // texture atlas
            s8 intersection;                        // intersection
            s8 collision;                           // collision
            s8 culling;                             // culling
            
            f32 spawn_rate;                         // 生产速率
            f32 spawn_count;                        // 生产个数
            f32 spawn_scale;                        // 生产缩放
            f32 spawn_threshold;                    // 生产极限值
            f32 delay_time;                         // 延迟时间
            f32 period_time;                        // 时间周期
            f32 duration_time;                      // 持续时间
            
            f32 world_mass;                         // 世界质量
            f32 world_imass;                        // world imass
            f32 length_stretch;                     // 长度拉伸
            f32 linear_damping;                     // 线程阻尼
            f32 angular_damping;                    // 角阻尼
            f32 growth_damping;                     // 生长阻尼
            f32 restitution;                        // 还原 restitution
            f32 roughness;                          // 粗糙度 roughness
            
            s8 emitter_type;                        // emitter type
            s8 emitter_enabled;                     // emitter enabled
            s8 emitter_based;                       // emitter based
            s8 emitter_shift;                       // emitter shift
            s8 emitter_continuous;                  // emitter continuous
            s32 emitter_sequence;                   // emitter sequence
            s32 emitter_limit;                      // emitter limit
            FVec3 emitter_size;                     // emitter size
            FVec3 emitter_direction;                // emitter direction
            FVec3 emitter_spread;                   // emitter spread
            FVec3 emitter_velocity;                 // emitter velocity
            FMat4 emitter_transform;                // emitter transformation
            FMat4 old_emitter_transform;            // emitter old transformation
            
            f32 delay_mean;                       // delay
            f32 delay_spread;
            f32 period_mean;                      // period
            f32 period_spread;
            f32 duration_mean;                    // duration
            f32 duration_spread;
            f32 life_mean;                        // life
            f32 life_spread;
            f32 velocity_mean;                    // velocity
            f32 velocity_spread;
            f32 angle_mean;                       // angle
            f32 angle_spread;
            f32 rotation_mean;                    // rotation
            f32 rotation_spread;
            f32 radius_mean;                      // radius
            f32 radius_spread;
            f32 growth_mean;                      // growth
            f32 growth_spread;
            f32 fade;                             // fade
            
            FVec3 gravity;                          // 重力 gravity

            SVArray<Force> forces;
            SVArray<Noise> noises;
            SVArray<Deflector> deflectors;
            SVArray<Distance> distances;
            SVArray<Particle> m_particles;
            SVArray<s32> remove;
            SVArray<Spark> sparks;                    // spark points
            SVArray<Contact> contacts;                // contacts
            SVArray<Contact> old_contacts;            // old contacts
            
            //包围盒
            SVBoundBox bound_box;                        // bounds
            SVBoundSphere bound_sphere;
            SVBoundBox particles_bound_box;
            
        public:
            //序列化接口
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };
        
    }//namespace util
    
}//namesapce sv



#endif /* SV_NEWPARTICLES_H */
