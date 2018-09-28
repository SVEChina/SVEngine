//
// SVRenderTarget.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERTARGET_H
#define SV_RENDERTARGET_H

#include "SVFboObject.h"

namespace sv {
    
    namespace render{
        
        class SVRenderTarget : public SVFboObject {
        public:
            SVRenderTarget(SVInst *_app,s32 _w, s32 _h,bool _depth,bool _stencil);
            
            ~SVRenderTarget();
            
            void create(SVRendererBasePtr _renderer);
            
            void destroy(SVRendererBasePtr _renderer);
            
            int getColorID();
            
        private:
            s32 m_w;
            s32 m_h;
            bool m_depth;
            bool m_stencil;
        };
        
        //
        class SVRenderTargetDefault : public SVRenderTarget {
        public:
            SVRenderTargetDefault(SVInst *_app,s32 _w, s32 _h);
            
            ~SVRenderTargetDefault();
            
            void create(SVRendererBasePtr _renderer);
            
            void destroy(SVRendererBasePtr _renderer);
        private:
            s32 m_w;
            s32 m_h;
        };
        
        //
        class SVRenderTargetOut : public SVRenderTarget {
        public:
            SVRenderTargetOut(SVInst *_app,s32 _w, s32 _h,u32 _fboid,u32 _colorID);
            
            ~SVRenderTargetOut();
            
            void create(SVRendererBasePtr _renderer);
            
            void destroy(SVRendererBasePtr _renderer);
        private:
            s32 m_w;
            s32 m_h;
            u32 m_fboid;
            u32 m_colorID;
        };

        class SVRenderTargetOutTexture : public SVRenderTarget {
        public:
            SVRenderTargetOutTexture(SVInst *_app,s32 _w, s32 _h,u32 _texId);

            ~SVRenderTargetOutTexture();

            void create(SVRendererBasePtr _renderer);

            void destroy(SVRendererBasePtr _renderer);
        private:
            s32 m_w;
            s32 m_h;
            u32 m_texid;
        };
        
    }//!namespace render
    
}//!namespace sv



#endif //SV_RENDERTARGET_H
