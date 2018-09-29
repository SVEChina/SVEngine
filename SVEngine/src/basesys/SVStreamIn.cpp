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
#include "../node/SVFaceImageUsingMoveNode.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../mtl/SVTexture.h"

//
StreamInCore::StreamInCore(SVInst* _app)
:SVGBase(_app){
    m_trans = nullptr;
    m_showNode = nullptr;
    m_tt = E_TEX_END;
}

StreamInCore::~StreamInCore() {
    m_trans = nullptr;
    m_showNode = nullptr;
    m_tt = E_TEX_END;
}

void StreamInCore::init(s32 _w,s32 _h,PICFORMATE _fromate,f32 _angle,bool _show) {
    for(s32 t_tt = E_TEX_INSTREAM ;t_tt<E_TEX_OUTSTREAM; t_tt++ ) {
        if(!mApp->getRenderer()->getSVTex( SVTEXTYPE(t_tt) ) ){
            //创建内置纹理
            m_tt = SVTEXTYPE(t_tt);
            if(_fromate == SV_PF_BGRA) {
                mApp->getRenderer()->createSVTex(m_tt,_w,_h,GL_RGBA);
            }else {
                mApp->getRenderer()->createSVTex(m_tt,_w,_h,GL_RGBA);
            }
            //创建可视节点

            if(_show) {
                m_showNode = MakeSharedPtr<SVIOSInstreamNode>(mApp);
                SVIOSInstreamNodePtr tmpNode = std::dynamic_pointer_cast<SVIOSInstreamNode>(m_showNode);
                if(tmpNode){
                    tmpNode->init(m_tt);
                }
            }
            //创建转换器
            m_trans = MakeSharedPtr<SVTransGPU>(mApp);
            m_trans->init(_w, _h,_angle,_fromate,m_tt);
            return;
        }
    }
}

void StreamInCore::destroy() {
    //
    if(m_showNode) {
        m_showNode->removeFromParent();
        m_showNode = nullptr;
    }
    //
    if(mApp->getRenderer()) {
        mApp->getRenderer()->destroySVTex(m_tt);
    }
    //
    m_trans = nullptr;
    m_tt = E_TEX_END;
}

//
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

void SVStreamIn::createInStream(cptr8 _name,s32 _type,PICFORMATE _formate,s32 _w,s32 _h,f32 _angle) {
    if(!mApp->getRenderer()) {
        return ;
    }
    //默认要摘掉
    m_streamLock->lock();
    //PICFORMATE
    StreamInCorePtr t_incore =MakeSharedPtr<StreamInCore>(mApp);
    t_incore->init(_w,_h,_formate,_angle,true);
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
        if(t_streamin->m_showNode) {
            SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
            if(t_sc){
                t_sc->addNode(t_streamin->m_showNode);
            }
        }
    }
}

void SVStreamIn::unactive(cptr8 _name) {
    TEXMAP::Iterator it = m_TexMap.find(_name);
    if(it!=m_TexMap.end()){
        StreamInCorePtr t_streamin = it->data;
        if(t_streamin->m_showNode) {
            t_streamin->m_showNode->removeFromParent();
        }
    }
}

void SVStreamIn::update(f32 _dt) {
    //转换更新
    TEXMAP::Iterator it = m_TexMap.begin();
    while(it!=m_TexMap.end()){
        StreamInCorePtr t_streamin = it->data;
        if(t_streamin->m_trans) {
            t_streamin->m_trans->update(_dt);
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
        if(t_streamin->m_trans) {
            t_streamin->m_trans->pushData(_srcPtr);
        }
    }
    m_streamLock->unlock();
}
