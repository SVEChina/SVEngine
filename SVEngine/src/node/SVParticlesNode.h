#ifndef SV_PARTICLENODE_H
#define SV_PARTICLENODE_H

#include "SVNode.h"
#include "../base/SVBounds.h"
#include "../base/SVVec3.h"
#include "../core/SVParticles.h"

namespace sv {
    
    namespace node{
        //
        class SVParticlesNode : public SVNode {
        public:
            SVParticlesNode(SVInst *_app);
            
            ~SVParticlesNode();
            
            void testInit();
            
            // particles
            SVParticlesPtr getParticles();

            void clearParticles() const;
            
            s32 getNumParticles() const;
            
            FVec3 getParticlePosition(s32 num) const;

            void getParticleTransforms(SVArray<FMat4> &transforms) const;
            //
            void setPhysicalMask(s32 mask);
            
            s32 getPhysicalMask() const;
            //************************************** Emitter *****************************
            void setEmitterSync(s32 sync);
            
            s32 getEmitterSync() const;
            
            void setEmitterVelocity(const FVec3 &velocity);
            
            const FVec3 &getEmitterVelocity() const;

            void addEmitterSpark(const FVec3 &point,const FVec3 &normal,const FVec3 &velocity) const;
            
            //Image *getNoiseImage(s32 num) const;
         
            //force
            void setForceTransform(s32 num,const FMat4 &transform) const;
            
            FMat4 getForceTransform(s32 num) const;
            
            //****************************** Noises********************************************
            void setNoiseTransform(s32 num,const FMat4 &transform) const;
            
            FMat4 getNoiseTransform(s32 num) const;
            
            //********************************* Deflectors *********************************
            void setDeflectorTransform(s32 num,const FMat4 &transform) const;
            
            FMat4 getDeflectorTransform(s32 num) const ;

            // world offset vector
            const FVec3 &getWorldOffset() const;
            
            // random point
            virtual s32 getRandomPoint(FVec3 &ret_point,FVec3 &ret_normal,FVec3 &ret_velocity,s32 surface);
            
            // surface info
            virtual s32 getNumTriangles(s32 surface) const;
            
            // surface bounds
            virtual const SVBoundBox &getSVBoundBox(s32 surface) const;
            
            //
            virtual const SVBoundSphere &getSVBoundSphere(s32 surface) const;
            
            // bounds
            virtual const SVBoundBox &getSVBoundBox() const;
            //
            virtual const SVBoundSphere &getSVBoundSphere() const;
            //
            virtual void renderVisualizer();
            
        protected:
            //更新
            virtual void update(f32 ifps);

            //渲染
            virtual void render();
            
            // update transformation
            virtual void update_transform();

            SVParticlesPtr m_pParticles;           // particles pointer
            
            SVParticlesWorldPtr m_pParticlesWorld;    // particles world
            
            s32 physical_mask;                        // physical mask
            
            s32 emitter_sync;                         // emitter synchronization
            
            FVec3 emitter_velocity;                    // emitter velocity
            
            FVec3 world_offset;                        // world offset
            
            s32 num_triangles;                        // number of triangles
            
            SVBoundBox bound_box;                     // bounding box
            
            SVBoundSphere bound_sphere;               // bounding sphere
            //
            SVMtlParticleAniPtr m_mtl_particle;
            SVRenderMeshPtr m_pMesh;
            SVRenderObjectPtr m_pRenderObj;
            SVDataSwapPtr m_pVertData;
            SVDataSwapPtr m_pIndexData;
            SVTexturePtr m_diffuse;
            SVTexturePtr m_atten;
            
        public:
            //序列化接口
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };

    }//!namespace node
    
}//!namespace sv

#endif /* SV_PARTICLENODE_H */
