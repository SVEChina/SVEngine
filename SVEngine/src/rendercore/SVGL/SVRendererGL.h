//
// SVRendererGL.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERERGL_H
#define SV_RENDERERGL_H

#include "../renderer/SVRendererBase.h"
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
            //提交shader
            virtual void submitShader(u32 _shader);
            //提交纹理
            virtual void submitTex(u32 _channel,TexUnit& _unit);
            //提交unifrom matrix
            virtual void submitUniformMatrix(cptr8 _name,f32* _data);
            //提交unifrom i1
            virtual void submitUniformi(cptr8 _name,s32 _data);
            //提交unifrom i2
            virtual void submitUniformi2(cptr8 _name,s32 _data1,s32 _data2);
            //提交unifrom i3
            virtual void submitUniformi3(cptr8 _name,s32 _data1,s32 _data2,s32 _data3);
            //提交unifrom i4
            virtual void submitUniformi4(cptr8 _name,s32 _data1,s32 _data2,s32 _data3,s32 _data4);
            //提交unifrom f1
            virtual void submitUniformf(cptr8 _name,f32 _data);
            //提交unifrom f2
            virtual void submitUniformf2(cptr8 _name,f32 _data1,f32 _data2);
            //提交unifrom f3
            virtual void submitUniformf3(cptr8 _name,f32 _data1,f32 _data2,f32 _data3);
            //提交unifrom f4
            virtual void submitUniformf4(cptr8 _name,f32 _data1,f32 _data2,f32 _data3,f32 _data4);
            //提交unifrom s32 v1
            virtual void submitUniformi1v(cptr8 _name,s32* _data,s32 _size = 1);
            //提交unifrom s32 v2
            virtual void submitUniformi2v(cptr8 _name,s32* _data,s32 _size = 1);
            //提交unifrom s32 v3
            virtual void submitUniformi3v(cptr8 _name,s32* _data,s32 _size = 1);
            //提交unifrom s32 v4
            virtual void submitUniformi4v(cptr8 _name,s32* _data,s32 _size = 1);
            //提交unifrom f32 v1
            virtual void submitUniformf1v(cptr8 _name,f32* _data,s32 _size = 1);
            //提交unifrom f32 v2
            virtual void submitUniformf2v(cptr8 _name,f32* _data,s32 _size = 1);
            //提交unifrom f32 v3
            virtual void submitUniformf3v(cptr8 _name,f32* _data,s32 _size = 1);
            //提交unifrom f32 v4
            virtual void submitUniformf4v(cptr8 _name,f32* _data,s32 _size = 1);
            //提交融合参数
            virtual void submitBlend(SVBlendParam& _param);
            //提交模板参数
            virtual void submitStencil(SVStencilParam& _param);
            //提交深度参数
            virtual void submitDepth(SVDepthParam& _param);
            //提交隐藏面参数
            virtual void submitCull(SVCullFaceParam& _param);
            //提交zfighting
            virtual void submitZOff(SVZOffParam& _param);
            //提交线宽
            virtual void submitLineWidth(f32 _width);
            //提交点大小
            virtual void submitPointSize(f32 _size);
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
            void svPushViewPort(u32 _x,u32 _y,u32 _w,u32 _h);
            
            void svPopViewPort();
            
        protected:
            s32 m_glVersion;
        };

    }//!namespace render
    
}//!namespace sv




//
#endif //SV_RENDERERGL_H
