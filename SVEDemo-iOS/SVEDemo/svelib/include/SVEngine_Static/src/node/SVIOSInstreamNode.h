//
// SVIOSInstreamNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_IOSINSTREAMNODE_H
#define SV_IOSINSTREAMNODE_H

#include "SVNode.h"

#ifdef SV_IOS

#include "VideoToolBox/VideoToolbox.h"
#include <CoreVideo/CVOpenGLESTextureCache.h>

namespace sv {
    
    namespace node{
        
        class SVIOSInstreamNode : public SVNode {
        public:
            SVIOSInstreamNode(SVInst *_app);
            
            virtual ~SVIOSInstreamNode();
            
            void init(s32 _formate,s32 _w,s32 _h,f32 _angle);
            
            void destroy();
            
            void pushData(u8* _srcPtr,s32 width,s32 height,s32 pixelFormat);
            
            void update(f32 dt);
            
            void render();
            
        protected:
            SVTextureIOSPtr m_tex;
            f32 m_angle;
            SVRenderObjectPtr m_pRenderObj;
            SVRenderMeshPtr m_pMesh;
            SVMtlCorePtr m_pMtl;
        };

    }//!namespace node
    
}//!namespace sv


#endif

#endif //SV_IOSCAMSTREAMNODE_H
