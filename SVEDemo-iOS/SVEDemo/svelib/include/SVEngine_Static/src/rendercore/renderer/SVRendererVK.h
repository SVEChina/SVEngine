//
// SVRendererVK.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERERVULKAN_H
#define SV_RENDERERVULKAN_H

#include "SVRendererBase.h"

namespace sv {
    
    namespace render{
        
        class SVRendererVK : public SVRendererBase {
        public:
            SVRendererVK(SVInst* _app);
            
            ~SVRendererVK();
            
            //资源区
        public:
            //纹理资源
            u32 applyTexID();
            
            void returnTexID(u32 _id);
            
            //VertexBuf
            u32 applyVBO();
            
            void returnVBO(u32 _id);
            
            //shader-vs
            u32 applyVS();
            
            void returnVS(u32 _id);
            
            //shader-fs
            u32 applyFS();
            
            void returnFS(u32 _id);
            
            //shader-programme
            u32 applyProgramme();
            
            void returnProgramme(u32 _id);
            
        protected:
            
        };
        
    }//!namespace render
    
}//!namespace sv





//
#endif //SV_RENDERERVULKAN_H
