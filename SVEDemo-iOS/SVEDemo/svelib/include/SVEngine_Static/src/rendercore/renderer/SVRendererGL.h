//
// SVRendererGL.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERERGL_H
#define SV_RENDERERGL_H

#include "SVRendererBase.h"
#include "../../core/SVVertDef.h"

namespace sv {
    
    namespace render{
        
        // GL渲染器
        
        class SVRendererGL : public SVRendererBase {
        public:
            SVRendererGL(SVInst* _app);
            
            ~SVRendererGL();
            
#if defined SV_IOS
            virtual void init(s32 _ver,void* _context,s32 _w,s32 _h);
#elif defined SV_ANDROID
            virtual void init(s32 _ver,void * _windows,void* context,s32 _w,s32 _h);
#elif defined SV_OSX
            virtual void init(s32 _ver,void* _context,void* _pixelFormate,s32 _w,s32 _h);
#endif
            virtual void destroy();
            
            virtual void resize(s32 _w,s32 _);
            
        public:
            //基本API
            //FBO
            void svBindFrameBuffer(u32 _id);
            
            void svBindClearColor(u32 _id);
            
            //颜色缓存
            void svBindColorBuffer(u32 _id);
            
            //顶点缓存
            void svBindVertexBuffer(u32 _id);
            
            //索引缓存
            void svBindIndexBuffer(u32 _id);
            
            //顶点格式更新
            void svUpdateVertexFormate(VFTYPE _vf);
            
            //视口
            void svViewPort(u32 _x,u32 _y,u32 _w,u32 _h);
            
        protected:
            s32 m_glVer;
            //渲染状态
            SVRenderStateGLPtr m_pRenderState;
        };

    }//!namespace render
    
}//!namespace sv




//
#endif //SV_RENDERERGL_H
