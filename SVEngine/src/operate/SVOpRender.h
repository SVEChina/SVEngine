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

#endif

#ifdef SV_WIN

#endif
    
#ifdef SV_OSX

#endif
    
#ifdef SV_ANDROID
    
#endif
    

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

    
    class SVOpSetRenderMirror: public SVOpBase{
    public:
        SVOpSetRenderMirror(SVInst *_app);
        
        void setTargetParam(bool _mirror);
    protected:
        void _process(f32 dt);
        
        bool m_mirror;
    };
    
}//!namespace sv




#endif //SV_OPERATERENDER_H
