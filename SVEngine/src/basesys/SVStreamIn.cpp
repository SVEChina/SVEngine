//
// SVStreamIn.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVStreamIn.h"
#include "SVTransGPU.h"
#include "SVTransCPU.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../basesys/SVSceneMgr.h"
#include "../node/SVNode.h"
#include "../node/SVCameraNode.h"
#include "../node/SVSpineNode.h"
#include "../node/SVScene.h"
#include "../node/SVIOSInstreamNode.h"
#include "../node/SVFacePointNode.h"
#include "../node/SVFaceShapeNode.h"
#include "../node/SVShapeVariedNode.h"
#include "../node/SVSpriteNode.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderScene.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
//
StreamInCore::StreamInCore(SVInst* _app)
:SVGBase(_app){
    m_trans = nullptr;
    m_showNode = nullptr;
    m_tt = E_TEX_END;
    m_useGPU = true;
    m_texResult = nullptr;
    m_tex0 = nullptr;
    m_tex1 = nullptr;
    m_tex2 = nullptr;
}

StreamInCore::~StreamInCore() {
    m_trans = nullptr;
    m_showNode = nullptr;
    m_tt = E_TEX_END;
    m_texResult = nullptr;
    m_tex0 = nullptr;
    m_tex1 = nullptr;
    m_tex2 = nullptr;
}

//
void StreamInCore::init(s32 _w,s32 _h,PICFORMATE _fromate,f32 _angle,bool _show) {
    for(s32 t_tt = E_TEX_INSTREAM ;t_tt<E_TEX_OUTSTREAM; t_tt++ ) {
        if(!mApp->getRenderer()->getSVTex( SVTEXTYPE(t_tt) ) ){
            init(_w,_h,_fromate,_angle,_show,SVTEXTYPE(t_tt) );
            return ;
        }
    }
}

void StreamInCore::init(s32 _w,s32 _h,PICFORMATE _fromate,f32 _angle,bool _show,SVTEXTYPE _tex){
    m_tt = _tex;
    //创建稳定的合成后的流
    m_formate = _fromate;
    m_texResult = mApp->getRenderer()->createSVTex(m_tt,_w,_h,GL_RGBA);
    //创建输入流
    if(_fromate == SV_PF_GRAY8) {
        //m_gray
        m_tex0 = mApp->getTexMgr()->createUnctrlTexture(_w,_h,GL_LUMINANCE,GL_LUMINANCE);
    }else if(_fromate == SV_PF_YV12) {
        //m_texY
        m_tex0 = mApp->getTexMgr()->createUnctrlTexture(_w, _h, GL_LUMINANCE,GL_LUMINANCE);
        //m_texUV
        m_tex1 = mApp->getTexMgr()->createUnctrlTexture(_w/2, _h/2,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA);
    }else if(_fromate == SV_PF_I420) {
        m_tex0 = mApp->getTexMgr()->createUnctrlTexture(_w, _h, GL_LUMINANCE, GL_LUMINANCE);
        m_tex1 = mApp->getTexMgr()->createUnctrlTexture(_w,_h,  GL_LUMINANCE, GL_LUMINANCE);
        m_tex2 = mApp->getTexMgr()->createUnctrlTexture(_w, _h, GL_LUMINANCE, GL_LUMINANCE);
    }else if(_fromate == SV_PF_NV12) {
        //m_texY
        m_tex0 = mApp->getTexMgr()->createUnctrlTexture(_w,  _h, GL_LUMINANCE, GL_LUMINANCE);
        //m_texUV
        m_tex1 = mApp->getTexMgr()->createUnctrlTexture(_w / 2,_h / 2, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA);
    }else if(_fromate == SV_PF_NV21) {
        //m_texY
        m_tex0 = mApp->getTexMgr()->createUnctrlTexture(_w, _h, GL_LUMINANCE, GL_LUMINANCE);
        //m_texUV
        m_tex1 = mApp->getTexMgr()->createUnctrlTexture(_w / 2, _h / 2, GL_LUMINANCE_ALPHA, GL_LUMINANCE_ALPHA);
    }else if(_fromate == SV_PF_BGRA) {
        m_tex0 = mApp->getTexMgr()->createUnctrlTexture(_w,_h,GL_RGBA,GL_RGBA);
    }else if(_fromate == SV_PF_RGBA) {
        m_tex0 = mApp->getTexMgr()->createUnctrlTexture(_w,_h,GL_RGBA,GL_RGBA);
    }
    //转换方式
    if(m_useGPU) {
        m_trans = MakeSharedPtr<SVTransGPU>(mApp,m_tex0,m_tex1,m_tex2,m_texResult);
        m_trans->init(_w, _h,_angle,_fromate,m_tt);
    } else {
        
    }
    //
    if(_show && m_trans) {
#ifdef SV_IOS
        m_showNode = MakeSharedPtr<SVIOSInstreamNode>(mApp);
        SVIOSInstreamNodePtr tmpNode = std::dynamic_pointer_cast<SVIOSInstreamNode>(m_showNode);
        if(tmpNode){
            tmpNode->init(m_tt);
        }
#endif
        
#ifdef SV_ANDROID
        m_showNode = MakeSharedPtr<SVSpriteNode>(mApp,(f32)_h,(f32)_w);
        SVSpriteNodePtr tmpNode = std::dynamic_pointer_cast<SVSpriteNode>(m_showNode);
        if(tmpNode){
            tmpNode->setTexture(m_tt);
            tmpNode->setRSType(RST_SKY);
        }
#endif
        
    }
}

void StreamInCore::destroy() {
    //
    if(m_showNode) {
        m_showNode->removeFromParent();
        m_showNode = nullptr;
    }
    m_texResult = nullptr;
    m_tex0 = nullptr;
    m_tex1 = nullptr;
    m_tex2 = nullptr;
    m_trans = nullptr;
    m_tt = E_TEX_END;
    if(mApp->getRenderer()) {
        mApp->getRenderer()->destroySVTex(m_tt);
    }
}

void StreamInCore::active() {
    if(m_showNode) {
        SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
        if(t_sc){
            t_sc->addNode(m_showNode);
        }
    }
}

void StreamInCore::unactive() {
    if(m_showNode) {
        m_showNode->removeFromParent();
    }
}

void StreamInCore::update(f32 _dt) {

}

void StreamInCore::pushData(u8* _srcPtr,s32 width,s32 height,s32 pixelFormat,s32 _angle) {
    if(m_trans) {
        m_trans->setAngle(_angle);
    }
    //
    if (m_formate == SV_PF_GRAY8) {

    } else if (m_formate == SV_PF_YV12) {
        m_tex0->setTexData(_srcPtr, width * height);
        m_tex1->setTexData(_srcPtr + width * height, width * height / 2);
    } else if (m_formate == SV_PF_I420) {
//        m_tex0->setTexData(_srcPtr, width * m_height);
//        m_tex1->setTexData(_srcPtr+width*height,width*height/2);
//        m_tex2->setTexData(_srcPtr+width*height,width*height/2);
    } else if (m_formate == SV_PF_NV12) {
        m_tex0->setTexData(_srcPtr, width * height);
        m_tex1->setTexData(_srcPtr + width * height, width * height / 2);
    } else if (m_formate == SV_PF_NV21) {
        m_tex0->setTexData(_srcPtr, width * height);
        m_tex1->setTexData(_srcPtr + width * height, width * height / 2);
    } else if (m_formate == SV_PF_BGRA) {
        m_tex0->setTexData(_srcPtr, width*height*4);
    } else if (m_formate == SV_PF_RGBA) {
        m_tex0->setTexData(_srcPtr, width*height*4);
    } else if (m_formate == SV_PF_RGB) {
    }
    
    //trans render
    if (m_trans) {
        m_trans->update(0.0f);
        m_trans->render();
    }
}

/*
 */
SVStreamIn::SVStreamIn(SVInst *_app)
:SVProcess(_app){
    m_streamLock = MakeSharedPtr<SVLock>();
}

SVStreamIn::~SVStreamIn() {
    TEXMAP::Iterator it = m_TexMap.begin();
    while(it!=m_TexMap.end()){
        StreamInCorePtr t_streamin = it->data;
        t_streamin->destroy();
        it++;
    }
    m_TexMap.clear();
    m_streamLock = nullptr;
}

void SVStreamIn::createInStream(cptr8 _name,s32 _type,PICFORMATE _formate,s32 _w,s32 _h,f32 _angle, bool _show) {
    if(!mApp->getRenderer()) {
        return ;
    }
    //默认要摘掉
    m_streamLock->lock();
    //PICFORMATE
    StreamInCorePtr t_incore =MakeSharedPtr<StreamInCore>(mApp);
    t_incore->init(_w,_h,_formate,_angle,_show);
    m_TexMap.append(_name,t_incore);
    m_streamLock->unlock();
}

void SVStreamIn::createInStream(cptr8 _name,s32 _type,PICFORMATE _formate,s32 _w,s32 _h,f32 _angle,SVTEXTYPE _tex, bool _show){
    if(!mApp->getRenderer()) {
        return ;
    }
    //默认要摘掉
    m_streamLock->lock();
    //PICFORMATE
    StreamInCorePtr t_incore =MakeSharedPtr<StreamInCore>(mApp);
    t_incore->init(_w,_h,_formate,_angle,_show,_tex);
    m_TexMap.append(_name,t_incore);
    m_streamLock->unlock();
}

void SVStreamIn::destroyInStream(cptr8 _name){
    TEXMAP::Iterator it = m_TexMap.find(_name);
    if(it!=m_TexMap.end()){
        it->data->destroy();
        m_TexMap.remove(it);
    }
}

void SVStreamIn::active(cptr8 _name) {
    TEXMAP::Iterator it = m_TexMap.find(_name);
    if(it!=m_TexMap.end()){
        StreamInCorePtr t_streamin = it->data;
        t_streamin->active();
    }
}

void SVStreamIn::unactive(cptr8 _name) {
    TEXMAP::Iterator it = m_TexMap.find(_name);
    if(it!=m_TexMap.end()){
        StreamInCorePtr t_streamin = it->data;
        t_streamin->unactive();
    }
}
void SVStreamIn::update(f32 _dt) {
    //转换更新
    TEXMAP::Iterator it = m_TexMap.begin();
    while(it!=m_TexMap.end()){
        StreamInCorePtr t_streamin = it->data;
        if(t_streamin) {
            t_streamin->update(_dt);
        }
        it++;
    }
}

//推送相机数据
void SVStreamIn::pushStreamData(cptr8 _name,u8* _srcPtr,s32 width,s32 height,s32 pixelFormat,s32 _angle){
    m_streamLock->lock();
    TEXMAP::Iterator it = m_TexMap.find(_name);
    if(it!=m_TexMap.end()){
        StreamInCorePtr t_streamin = it->data;
        t_streamin->pushData(_srcPtr,width,height,pixelFormat,_angle);
    }
    m_streamLock->unlock();
}
