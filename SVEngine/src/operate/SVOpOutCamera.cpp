//
// SVOpOutCamera.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpOutCamera.h"
#include "../app/SVInst.h"
#include "../node/SVScene.h"
#include "../node/SVIOSInstreamNode.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVStreamIn.h"
#include "../basesys/SVStreamOut.h"
#include "../rendercore/SVRenderDef.h"
//设置ios相机
SVOpCreateIOSInstream::SVOpCreateIOSInstream(SVInst *_app, cptr8 _name, PICFORMATE _format, s32 _w, s32 _h,f32 _angle, bool _show)
        : SVOpBase(_app) {
    m_name = _name;
    m_formate = _format;
    m_width = _w;
    m_height = _h;
    m_angle = _angle;
    m_show = _show;
}

void SVOpCreateIOSInstream::_process(f32 _dt) {
#ifdef SV_IOS
    //创建一个ios相机节点 挂在场景中
    SVStreamInPtr t_cam_stream = mApp->getBasicSys()->getStreamIn();
    if(t_cam_stream){
        t_cam_stream->createInStream(m_name.c_str(),0,m_formate,m_width,m_height,m_angle, m_show);
        t_cam_stream->active(m_name.c_str());
    }
#endif
}

//设置ios相机
SVOpCreateIOSTexIDInstream::SVOpCreateIOSTexIDInstream(SVInst *_app, cptr8 _name, u32 _tex0ID, u32 _tex1ID, u32 _tex2ID, PICFORMATE _format, s32 _w, s32 _h,f32 _angle, bool _show)
: SVOpBase(_app) {
    m_name = _name;
    m_formate = _format;
    m_width = _w;
    m_height = _h;
    m_angle = _angle;
    m_tex0ID = _tex0ID;
    m_tex1ID = _tex1ID;
    m_tex2ID = _tex2ID;
    m_show = _show;
}

void SVOpCreateIOSTexIDInstream::_process(f32 _dt) {
#ifdef SV_IOS
    //创建一个ios相机节点 挂在场景中
    SVStreamInPtr t_cam_stream = mApp->getBasicSys()->getStreamIn();
    if(t_cam_stream){
        t_cam_stream->createInTextureStream(m_name.c_str(), m_tex0ID, m_tex1ID, m_tex2ID, 0, m_formate, m_width, m_height, m_angle, m_show);
        t_cam_stream->active(m_name.c_str());
    }
#endif
}

//删除ios相机
SVOpDestroyIOSInstream::SVOpDestroyIOSInstream(SVInst *_app, cptr8 _name)
        : SVOpBase(_app) {
    m_name = _name;
}

void SVOpDestroyIOSInstream::_process(f32 _dt) {
#ifdef SV_IOS
    SVStreamInPtr t_cam_stream = mApp->getBasicSys()->getStreamIn();
    if(t_cam_stream){
        t_cam_stream->unactive(m_name.c_str());
        t_cam_stream->destroyInStream(m_name.c_str());
    }
#endif
}


SVOpCreateIOSOutstream::SVOpCreateIOSOutstream(SVInst *_app, cptr8 _name, s32 _format,  s32 _width, s32 _height, s32 _steamType)
: SVOpBase(_app) {
    m_name = _name;
    m_formate = _format;
    m_streamType = _steamType;
    m_width = _width;
    m_height = _height;
}

void SVOpCreateIOSOutstream::_process(f32 _dt) {
#ifdef SV_IOS
    SVStreamOutPtr t_streamout = mApp->getBasicSys()->getStreamOut();
    if( t_streamout ) {
        t_streamout->createOutStream(m_name.c_str(), 0, m_formate, m_width, m_height, RENDERSTREAMTYPE(m_streamType));
    }
#endif
}


SVOpDestroyIOSOutstream::SVOpDestroyIOSOutstream(SVInst *_app, cptr8 _name)
        : SVOpBase(_app) {
    m_name = _name;
}

void SVOpDestroyIOSOutstream::_process(f32 _dt) {
#ifdef SV_IOS
    SVStreamOutPtr t_streamout = mApp->getBasicSys()->getStreamOut();
    if( t_streamout ) {
        t_streamout->unactive();
        t_streamout->destroyOutStream();
    }
#endif
}


SVOpOpenIOSOutstream::SVOpOpenIOSOutstream(SVInst *_app, cb_out_stream _cb)
        : SVOpBase(_app) {
    m_cb = _cb;
}

void SVOpOpenIOSOutstream::_process(f32 _dt) {
#ifdef SV_IOS
    SVStreamOutPtr t_streamout = mApp->getBasicSys()->getStreamOut();
    if( t_streamout ) {
        t_streamout->setStreamOutCB(m_cb);
        t_streamout->active();
    }
#endif
}

SVOpCloseIOSOutstream::SVOpCloseIOSOutstream(SVInst *_app)
        : SVOpBase(_app) {
}

void SVOpCloseIOSOutstream::_process(f32 _dt) {
#ifdef SV_IOS
    SVStreamOutPtr t_streamout = mApp->getBasicSys()->getStreamOut();
    if( t_streamout ) {
        t_streamout->unactive();
    }
#endif
}

//创建安卓相机
SVOpCreateAndriodCamera::SVOpCreateAndriodCamera(SVInst *_app, s32 _format, s32 _w, s32 _h,
                                                 f32 _angle)
        : SVOpBase(_app) {
    m_formate = _format;
    m_width = _w;
    m_height = _h;
    m_angle = _angle;
}

void SVOpCreateAndriodCamera::_process(f32 _dt) {
    m_formate = GL_RGBA;


}

SVOpDestroyAndoidCamera::SVOpDestroyAndoidCamera(SVInst *_app, cptr8 _name)
        : SVOpBase(_app) {
    m_name = _name;
}

void SVOpDestroyAndoidCamera::_process(f32 _dt) {

}

//创建纹理相机
SVOpCreateTextureCamera::SVOpCreateTextureCamera(SVInst *_app, s32 _format, s32 _w, s32 _h,
                                                 f32 _angle)
        : SVOpBase(_app) {
    m_formate = _format;
    m_width = _w;
    m_height = _h;
    m_angle = _angle;
}

void SVOpCreateTextureCamera::_process(f32 _dt) {
    SVStreamOutPtr t_streamout = mApp->getBasicSys()->getStreamOut();
    if( t_streamout ) {
        t_streamout->createOutStream(nullptr,1,GL_RGBA, m_width, m_height);
        t_streamout->active();
    }
}


SVOpDestroyTextureOutstream::SVOpDestroyTextureOutstream(SVInst *_app, cptr8 _name): SVOpBase(_app) {
    m_name = _name;
}

void SVOpDestroyTextureOutstream::_process(f32 _dt) {
    SVStreamOutPtr t_streamout = mApp->getBasicSys()->getStreamOut();
    if( t_streamout ) {
        t_streamout->unactive();
        t_streamout->destroyOutStream();
    }
}


//创建流相机
SVOpCreateStreamCamera::SVOpCreateStreamCamera(SVInst *_app, cptr8 _name,s32 _format, s32 _w, s32 _h,
                                               f32 _angle)
        : SVOpBase(_app) {
    m_formate = _format;
    m_width = _w;
    m_height = _h;
    m_angle = _angle;
    m_name = _name;
}

void SVOpCreateStreamCamera::_process(f32 _dt) {
    SVStreamInPtr t_cam_stream = mApp->getBasicSys()->getStreamIn();
    if (t_cam_stream) {
        t_cam_stream->createInStream(m_name.c_str(), 1,(PICFORMATE)m_formate, m_width, m_height, m_angle,true);
        t_cam_stream->active(m_name.c_str());
    }
}

SVOpDestroyStreamCamera::SVOpDestroyStreamCamera(SVInst *_app, cptr8 _name)
        : SVOpBase(_app) {
    m_name = _name;
}

void SVOpDestroyStreamCamera::_process(f32 _dt) {
    SVStreamInPtr t_cam_stream = mApp->getBasicSys()->getStreamIn();
    if (t_cam_stream) {
        t_cam_stream->unactive(m_name.c_str());
        t_cam_stream->destroyInStream(m_name.c_str());
    }
}


