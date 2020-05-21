//
// SVResMetalFbo.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RESMETAL_FBO_H
#define SV_RESMETAL_FBO_H

#include "SVFboObject.h"
#include "SVResMetalTex.h"
#import <Metal/MTLTexture.h>
#import <Metal/MTLBuffer.h>
#import <Metal/MTLRenderCommandEncoder.h>

namespace sv {

    namespace render{
        
        //////////////////////////////////////////////////////////FBO资源
        class SVResMetalFBO: public SVResFBO {
        public:
            SVResMetalFBO(SVInst* _app);
            
            virtual ~SVResMetalFBO();
            
            virtual void create(SVRendererPtr _renderer);
            
            virtual void destroy(SVRendererPtr _renderer);
            
            id<MTLRenderCommandEncoder> getEncoder();
            
        protected:
            id<MTLRenderCommandEncoder> m_pEncoder;
        };
        
        //外部传入的FBO
        class SVResMetalOutFBO : public SVResMetalFBO {
        public:
            SVResMetalOutFBO(SVInst *_app,u32 _fboid);
            
            ~SVResMetalOutFBO();
            
            virtual void create(SVRendererPtr _renderer);
            
            virtual void destroy(SVRendererPtr _renderer);
        };
        
        //RenderTarget
        class SVResMetalRenderTarget : public SVResMetalFBO {
        public:
            SVResMetalRenderTarget(SVInst *_app,s32 _w, s32 _h,bool _depth,bool _stencil);
            
            ~SVResMetalRenderTarget();
            
            void create(SVRendererPtr _renderer);
            
            void destroy(SVRendererPtr _renderer);
            
            inline int getColorID() { return m_colorID; }
            
        protected:
            u32 m_colorID;
        };
        //
        class SVResMetalRenderTargetOut : public SVResMetalRenderTarget {
        public:
            SVResMetalRenderTargetOut(SVInst *_app,s32 _w, s32 _h,u32 _fboid,u32 _colorID);
            
            ~SVResMetalRenderTargetOut();
            
            void create(SVRendererPtr _renderer);
            
            void destroy(SVRendererPtr _renderer);
        };
        //
        class SVResMetalRenderTexture : public SVResMetalFBO {
        public:
            SVResMetalRenderTexture(SVInst *_app,SVRResMetalTexPtr _tex, bool _depth,bool _stencil);
            
            ~SVResMetalRenderTexture();
            
            void create(SVRendererPtr _renderer);
            
            void destroy(SVRendererPtr _renderer);
            
            void setTexture(SVRResGLTexPtr _tex);
            
            void refresh();
            
        protected:
            void _bindColor();
            
            SVRResMetalTexPtr m_tex;
        };
        
    }//!namespace render

}//!namespace sv



//
#endif //SV_RRESMETAL_FBO_H
