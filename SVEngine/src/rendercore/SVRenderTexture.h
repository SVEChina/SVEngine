//
// SVRenderTexture.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERTEXTURE_H
#define SV_RENDERTEXTURE_H

#include "SVFboObject.h"
#include "../mtl/SVMtlDeclare.h"

namespace sv {
    
    namespace render{
        
        class SVRenderTexture : public SVFboObject {
        public:
            SVRenderTexture(SVInst *_app, SVTexturePtr _tex, bool _depth, bool _stencil);
            
            ~SVRenderTexture();
            
            void create(SVRendererPtr _renderer);
            
            void destroy(SVRendererPtr _renderer);
            
            void setTexture(SVTexturePtr _tex);
            
            void refresh();
        private:
            SVTexturePtr m_tex;
            bool m_depth;
            bool m_stencil;
        };
        
    }//!namespace render
    
}//!namespace sv



#endif //SV_RENDERTEXTURE_H
