//
// SVOpRender.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPERATERENDER_H
#define SV_OPERATERENDER_H

#include "SVOpBase.h"
#include "../base/SVMat4.h"
namespace sv {
    
#ifdef SV_IOS
    
    //创建Metal渲染器
    class SVOpCreateRenderderMTL : public SVOpBase {
    public:
        SVOpCreateRenderderMTL(SVInst *_app);
        
    protected:
        void _process(f32 dt);
        s32 m_w;
        s32 m_h;
    };
    //
    class SVOpCreateRenderderGLIOS : public SVOpBase {
    public:
        SVOpCreateRenderderGLIOS(SVInst *_app,
                                 s32 _ver,
                                 void *_context,
                                 s32 _w,
                                 s32 _h);
        
    protected:
        void _process(f32 dt);
        //
        void* m_glContext;
        s32 m_glVersion;
        s32 m_w;
        s32 m_h;
    };
#endif

#ifdef SV_WIN
    //
    class SVOpCreateRenderderGLWIN : public SVOpBase {
    public:
        SVOpCreateRenderderGLWIN(SVInst *_app);
        
    protected:
        void _process(f32 dt);
    };
#endif
    
#ifdef SV_OSX
    //
    class SVOpCreateRenderderGLOSX : public SVOpBase {
    public:
        SVOpCreateRenderderGLOSX(SVInst *_app,
                                 s32 _ver,
                                 void*_context,
                                 void* _pixelFormate,
                                 s32 _w,
                                 s32 _h);
        
    protected:
        void _process(f32 dt);
        
        void* m_glContext;
        void* m_pixelFormate;
        s32 m_glVersion;
        s32 m_w;
        s32 m_h;
    };
#endif
    
#ifdef SV_ANDROID
    
    //创建VK渲染器
    class SVOpCreateRenderderVK : public SVOpBase {
    public:
        SVOpCreateRenderderVK(SVInst *_app);
        
    protected:
        void _process(f32 dt);
        s32 m_w;
        s32 m_h;
    };
    
    //
    class SVOpCreateRenderderGLAND : public SVOpBase {
    public:
        SVOpCreateRenderderGLAND(SVInst *_app,
                                 s32 _ver,
                                 void*_context,
                                 void* _windows,
                                 s32 _w,
                                 s32 _h);
        
    protected:
        void _process(f32 dt);
        void* m_glContext;
        void* m_windows;
        s32 m_glVersion;
        s32 m_w;
        s32 m_h;
    };
#endif
    
    //销毁渲染器
    class SVOpDestroyRenderder : public SVOpBase {
    public:
        SVOpDestroyRenderder(SVInst *_app);
        
    protected:
        void _process(f32 dt);
    };
    
    //改变渲染器尺寸
    class SVOpResizeRenderder : public SVOpBase {
    public:
        SVOpResizeRenderder(SVInst *_app,s32 _w,s32 _h);
        
    protected:
        void _process(f32 dt);
        s32 m_width;
        s32 m_height;
    };
    
    //设置GL渲染目标
    class SVOpSetRenderTarget : public SVOpBase {
    public:
        SVOpSetRenderTarget(SVInst *_app);
        
        void setTargetParam(s32 _w,s32 _h,u32 _frameID,u32 _colorID, bool _mirror);
        
    protected:
        void _process(f32 dt);
        
        s32 m_width;
        s32 m_height;
        bool m_mirror;
        u32 m_frameID;
        u32 m_colorID;
    };
    
    //设置渲染目标 -- 纹理
    class SVOpSetRenderTextureTarget : public SVOpBase {
    public:
        SVOpSetRenderTextureTarget(SVInst *_app);

        void setTargetParam(s32 _w,s32 _h,u32 _textureId, bool _mirror);

    protected:
        void _process(f32 dt);

        s32 m_width;
        s32 m_height;
        bool m_mirror;
        u32 m_texId;
    };

    //设置GL渲染目标
    class SVOpDestroyRenderTarget : public SVOpBase {
    public:
        SVOpDestroyRenderTarget(SVInst *_app);
        
    protected:
        void _process(f32 dt);
    };
    
    class SVOpSetRenderMirror: public SVOpBase{
    public:
        SVOpSetRenderMirror(SVInst *_app);
        
        void setTargetParam(bool _mirror);
    protected:
        void _process(f32 dt);
        
        bool m_mirror;
    };
    
    //更新默认矩阵栈里的矩阵
    class SVOpRefreshDefMat: public SVOpBase{
    public:
        SVOpRefreshDefMat(SVInst *_app, FMat4 _viewMat, FMat4 _projMat, FMat4 _vpMat);
        
    protected:
        void _process(f32 dt);
        FMat4 m_viewMat;
        FMat4 m_projMat;
        FMat4 m_vpMat;
    };
    
}//!namespace sv




#endif //SV_OPERATERENDER_H
