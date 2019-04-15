//
// SVRResGL.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RESMETAL_MESH_H
#define SV_RESMETAL_MESH_H

#include "../SVRObjBase.h"
#import <Metal/MTLTexture.h>
#import <Metal/MTLBuffer.h>

namespace sv {

    namespace render{

        //Buffer资源
        class SVRResMetalBuf: public SVRObjBase {
        public:
            SVRResMetalBuf(SVInst* _app);
            
            virtual ~SVRResMetalBuf();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
        protected:
            id<MTLBuffer> m_pVerBuf;
        };
        
        //mesh
        class SVResMetalRenderMesh : public SVRResMetalBuf {
        public:
            SVResMetalRenderMesh(SVInst* _app);
            
            ~SVResMetalRenderMesh();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual void render();
        };

        
    }//!namespace render

}//!namespace sv



//
#endif //SV_RRESMETAL_MESH_H
