//
// SVTransTexNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_TRANSTEXNODE_H
#define SV_TRANSTEXNODE_H

#include "SVFrameOutNode.h"

namespace sv {

    namespace node {
        class SVTransTexNode : public SVFrameOutNode {
        public:
            SVTransTexNode(SVInst *_app);

            virtual ~SVTransTexNode();

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


#endif //ANDROID_SVTRANSTEXNODE_H
