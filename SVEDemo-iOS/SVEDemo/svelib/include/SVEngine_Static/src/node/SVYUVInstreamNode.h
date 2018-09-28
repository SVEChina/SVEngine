//
// SVYUVInstreamNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_YUVINSTRESMNODE_H
#define SV_YUVINSTRESMNODE_H

#include "SVNode.h"

namespace sv {

    namespace node {
        class SVYUVInstreamNode : public SVNode {
        public:
            SVYUVInstreamNode(SVInst *_app);

            virtual ~SVYUVInstreamNode();

            void init(s32 _formate, s32 _w, s32 _h, f32 _angle);

            void destroy();

            void pushData(u8 *_srcPtr, s32 width, s32 height, s32 pixelFormat,f32 _angle);

            void update(f32 dt);

            void render();

        protected:
            SVTexturePtr m_texY;
            SVTexturePtr m_texUV;
            SVTexturePtr m_texU;
            SVTexturePtr m_texV;
            f32 m_angle;
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            bool  m_beload;
            SVMtlCorePtr m_pMtl;
        };
    }//!namespace node

}//!namespace sv

#endif //ANDROID_SVYUVINSTRESMNODE_H
