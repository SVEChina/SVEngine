//
// SVPhysicsBodyCloth.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BODIES_PHYSICSBODYCLOTH_H
#define SV_BODIES_PHYSICSBODYCLOTH_H
#include "SVPhysicsBodySoft.h"
#include "../../base/SVVec3.h"
namespace sv {
    
    namespace logic {
        
        class SVPhysicsBodyCloth : public SVPhysicsBodySoft {
        public:
            SVPhysicsBodyCloth(SVInst* _app, btSoftBodyWorldInfo& _worldInfo, const FVec3& _corner00,
                               const FVec3& _corner10,
                               const FVec3& _corner01,
                               const FVec3& _corner11,
                               s32 _resx,
                               s32 _resy,
                               s32 _fixeds = 1 + 2 + 4 + 8,
                               bool gendiags = true);
            //根据已有的mesh顶点创建
            SVPhysicsBodyCloth(SVInst* _app, btSoftBodyWorldInfo& _worldInfo, const f32 *_vertices, const s32 *_indices, s32 _ntrangles,
            bool randomizeConstraints = true);
            
            ~SVPhysicsBodyCloth();
            
            virtual void init();
            
            virtual void destroy();
            
            virtual void update(f32 _dt);
            
            void *getFaceVertexData();
            
            u32   getFaceVertexDataSize();
            
            u32   getFaceVertexCount();
        private:
            void _updateFaceVertexData();
        protected:
            u32 m_vertexCount;
            SVDataSwapPtr m_pVertexData;
        };
    }//!namespace logic
}//!namespace sv

#endif //SV_BODIES_PHYSICSBODYCLOTH_H
