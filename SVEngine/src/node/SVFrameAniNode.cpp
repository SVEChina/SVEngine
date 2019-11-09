//
// SVFrameAniNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFrameAniNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtl2D.h"
#include "../mtl/SVTexMgr.h"
#include "../core/SVGeoGen.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderMgr.h"

SVFrameAniNode::SVFrameAniNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVFrameAniNode";
    m_rsType = RST_SOLID_3D;
    m_canSelect = false;
    m_accTime = 0.0f;
    m_totalTime = 90.0f;
    m_pActTex = nullptr;
    m_pMesh = nullptr;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_width = 850;
    m_height = 1000;
    setSize(m_width,m_height);
    m_loop = true;
}

SVFrameAniNode::~SVFrameAniNode() {
    m_pActTex = nullptr;
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
}

//
void SVFrameAniNode::setSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_pMesh = SVGeoGen::genRect(mApp, m_width, m_height, 0, 0, m_width, m_height,m_aabbBox);
}

f32 SVFrameAniNode::getRelativeWidth(){
    return m_width;
}

f32 SVFrameAniNode::getRelativeHeight(){
    return m_height;
}

void SVFrameAniNode::update(f32 dt) {
    SVNode::update(dt);
    //时间更新
    m_accTime += dt;
    if(m_loop) {
        if(m_accTime>m_totalTime) {
            m_accTime = 0.0f;
        }
    }else{
        if(m_accTime>m_totalTime) {
            m_accTime = m_totalTime;
        }
    }
    //预先加载
    _preload();
    //根据时间计算激活纹理
    m_pActTex = _selectTex(m_accTime);
    if(!m_pActTex) {
        m_pActTex = mApp->getTexMgr()->getSVETexture();
    }
    //卸载
    _unload();
    //更新材质
    if (m_pRenderObj && m_pMesh ) {
        //创建新的材质
        SVMtl2DPtr t_mtl = MakeSharedPtr<SVMtl2D>(mApp, "normal2d_c");
        t_mtl->setModelMatrix(m_absolutMat.get());
        t_mtl->setTexcoordFlip(1.0f, -1.0f);
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
        t_mtl->setAlpha(1.0f);
        t_mtl->setTexture(0,m_pActTex);
        t_mtl->update(dt);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(t_mtl);
        m_pMtl = t_mtl;
    }
}

void SVFrameAniNode::render() {
    if (m_visible && m_pRenderObj ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        m_pRenderObj->pushCmd(t_rs, m_rsType, "SVFrameAniNode");
    }
    SVNode::render();
}

//播放控制
void SVFrameAniNode::play() {
}

void SVFrameAniNode::pause() {
}

void SVFrameAniNode::stop() {
}

SVTexturePtr SVFrameAniNode::_selectTex(f32 _time) {
    //二分法查目标
    s32 t_ct =m_framePool.size();
    if(t_ct<0)
        return nullptr;
    m_curFrame = s32(_time*12.0f);
    if(m_curFrame>=t_ct)
        return nullptr;
     m_framePool[m_curFrame].m_pTex = mApp->getTexMgr()->getTextureSync( m_framePool[m_curFrame].m_pTexName.c_str(),true,true);
//    for(s32 i=0;i<2;i++) {
//        s32 t_aim_index = (m_curFrame + i)%t_ct;
//        m_framePool[t_aim_index].m_pTex = mApp->getTexMgr()->getTextureSync( m_framePool[t_aim_index].m_pTexName.c_str(),true,true);
//    }
    return m_framePool[m_curFrame].m_pTex;
}

void SVFrameAniNode::_preload() {
    //预加载
//    s32 m_curFrame; //当前帧
//    s32 m_aimFrame; //目标帧
//    s32 m_preFrame; //预先帧
}

void SVFrameAniNode::_unload() {
    //卸载当前帧就可以，用一帧，卸一帧
    if(m_curFrame>=0 && m_curFrame<m_framePool.size()) {
        m_framePool[m_curFrame].m_pTex = nullptr;
    }
}

void SVFrameAniNode::pushFrame(cptr8 _texname) {
    FrameTex t_ftex;
    t_ftex.m_pTexName = _texname;
    t_ftex.m_pTex = nullptr;
    m_framePool.append(t_ftex);
}

void SVFrameAniNode::clearFrame() {
    for(s32 i=0;i<m_framePool.size();i++) {
        m_framePool[i].m_pTex = nullptr;
    }
    m_framePool.destroy();
}
