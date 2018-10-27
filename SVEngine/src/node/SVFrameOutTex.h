//
// SVFrameOutTex.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FRAMEOUT_TEX_H
#define SV_FRAMEOUT_TEX_H

#include "SVFrameOutNode.h"

namespace sv {

    namespace node {
        
        //
        class SVFrameOutTex : public SVFrameOutNode {
        public:
            SVFrameOutTex(SVInst *_app);

            virtual ~SVFrameOutTex();

            void update(f32 dt);

            void render();

            s32 getTexId();

        private:
            SVRenderTexturePtr m_fbo;
            SVRenderMeshPtr m_pMesh;
            SVMtlCorePtr m_pMtl;
        };
    }
}


#endif //SV_FRAMEOUT_TEX_H
