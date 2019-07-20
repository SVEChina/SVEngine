//
// SVOpOutCamera.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPERATE_OUTCAMERA_H
#define SV_OPERATE_OUTCAMERA_H

#include "SVOpBase.h"

namespace sv {
    
    //创建ios相机
    class SVOpCreateIOSInstream : public SVOpBase {
    public:
        SVOpCreateIOSInstream(SVInst* _app,cptr8 _name,PICFORMATE _format,s32 _w,s32 _h,f32 _angle, bool _show = false);
        
        void _process(f32 _dt);
        
    protected:
        SVString m_name;
        PICFORMATE m_formate;
        s32 m_width;
        s32 m_height;
        f32 m_angle;
        bool m_show;
    };
    
    //创建ios相机
    class SVOpCreateIOSTexIDInstream : public SVOpBase {
    public:
        SVOpCreateIOSTexIDInstream(SVInst* _app,cptr8 _name, u32 _tex0ID, u32 _tex1ID, u32 _tex2ID, PICFORMATE _format,s32 _w,s32 _h,f32 _angle, bool _show = false);
        
        void _process(f32 _dt);
        
    protected:
        SVString m_name;
        PICFORMATE m_formate;
        s32 m_width;
        s32 m_height;
        f32 m_angle;
        u32 m_tex0ID;
        u32 m_tex1ID;
        u32 m_tex2ID;
        bool m_show;
    };
    
    //销毁ios相机
    class SVOpDestroyIOSInstream : public SVOpBase {
    public:
        SVOpDestroyIOSInstream(SVInst* _app,cptr8 _name);
        
        void _process(f32 _dt);
    protected:
        SVString m_name;
    };
    
    //创建ios输出流
    class SVOpCreateIOSOutstream : public SVOpBase {
    public:
        SVOpCreateIOSOutstream(SVInst* _app,cptr8 _name,s32 _format, s32 _width, s32 _height, s32 _steamType = 20);
        
        void _process(f32 _dt);
        
    protected:
        SVString m_name;
        s32 m_formate;
        s32 m_streamType;
        s32 m_width;
        s32 m_height;
    };
    
    //销毁ios输出流
    class SVOpDestroyIOSOutstream : public SVOpBase {
    public:
        SVOpDestroyIOSOutstream(SVInst* _app,cptr8 _name);
        
        void _process(f32 _dt);
    protected:
        SVString m_name;
    };
    
    //打开ios输出流
    class SVOpOpenIOSOutstream : public SVOpBase {
    public:
        SVOpOpenIOSOutstream(SVInst* _app, cb_out_stream _cb);
        
        void _process(f32 _dt);
    protected:
        cb_out_stream m_cb;
    };
    
    //关闭ios输出流
    class SVOpCloseIOSOutstream : public SVOpBase {
    public:
        SVOpCloseIOSOutstream(SVInst* _app);
        
        void _process(f32 _dt);
    };
    
    //创建一个andriod相机
    class SVOpCreateAndriodCamera: public SVOpBase {
    public:
        SVOpCreateAndriodCamera(SVInst* _app,s32 _format,s32 _w,s32 _h,f32 _angle);
        
        void _process(f32 _dt);
        
    protected:
        s32 m_formate;
        s32 m_width;
        s32 m_height;
        f32 m_angle;
    };
    
    //销毁android相机
    class SVOpDestroyAndoidCamera : public SVOpBase {
    public:
        SVOpDestroyAndoidCamera(SVInst* _app,cptr8 _name);
        
        void _process(f32 _dt);
    protected:
        SVString m_name;
    };
   
    //创建一个纹理相机
    class SVOpCreateTextureCamera : public SVOpBase {
    public:
        SVOpCreateTextureCamera(SVInst* _app,s32 _format,s32 _w,s32 _h,f32 _angle);
        
        void _process(f32 _dt);
        
    protected:
        s32 m_formate;
        s32 m_width;
        s32 m_height;
        f32 m_angle;
    };

    //销毁ios输出流
    class SVOpDestroyTextureOutstream : public SVOpBase {
    public:
        SVOpDestroyTextureOutstream(SVInst* _app,cptr8 _name);

        void _process(f32 _dt);
    protected:
        SVString m_name;
    };


    //创建一个流相机
    class SVOpCreateStreamCamera : public SVOpBase {
    public:
        SVOpCreateStreamCamera(SVInst* _app,cptr8 _name,s32 _format,s32 _w,s32 _h,f32 _angle);
        
        void _process(f32 _dt);
        
    protected:
        s32 m_formate;
        s32 m_width;
        s32 m_height;
        f32 m_angle;
        SVString m_name;
    };


    //销毁一个相机流

    class SVOpDestroyStreamCamera : public SVOpBase {
    public:
        SVOpDestroyStreamCamera(SVInst* _app,cptr8 _name);

        void _process(f32 _dt);
    protected:
        SVString m_name;
    };
    
}//!namespace sv



#endif //SV_OPERATE_OUTCAMERA_H
