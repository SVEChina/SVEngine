//
// SVSpriteNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVSpriteNode.h"
#include "SVScene.h"
#include "SVCameraNode.h"
#include "../mtl/SVMtlCore.h"
#include "../rendercore/SVRenderMesh.h"
#include "../core/SVGeoGen.h"
#include "../basesys/SVConfig.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../basesys/SVStaticData.h"
//
SVSpriteNode::SVSpriteNode(SVInst *_app)
:SVNode(_app) {
    SVSpriteNode(_app,100,100);
}

SVSpriteNode::SVSpriteNode(SVInst *_app,f32 _w,f32 _h)
:SVNode(_app) {
    ntype = "SVSpriteNode";
    m_inTexType = E_TEX_END;
    m_rsType = RST_ANIMATE;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_canSelect = false;
    m_pTex = nullptr;
    m_pMesh = nullptr;
    setTexcoord(1.0,-1.0);
    setSpriteSize(_w,_h);
}

SVSpriteNode::~SVSpriteNode() {
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
    m_pTex = nullptr;
}

//
void SVSpriteNode::setSpriteSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_pMesh = SVGeoGen::genRect(mApp, m_width, m_height, 0, 0, m_width, m_height,m_aabbBox);
}

void SVSpriteNode::setTexcoord(f32 _x,f32 _y){
    m_texcoordX=_x;
    m_texcoordY=_y;
}

void SVSpriteNode::setMaterial(SVMtlCorePtr _mtl){
    m_pMtl = _mtl;
}

void SVSpriteNode::setTexture(cptr8 _path){
    if (m_pTex) {
        if (strcmp(m_pTex->getname(), _path) != 0) {
            m_pTex = nullptr;
        }
    }
    if (m_pTex == nullptr) {
        m_pTex = mApp->getTexMgr()->getTextureSync(_path,true);
    }
}

cptr8 SVSpriteNode::getTexturePath(){
    if (m_pTex) {
        return m_pTex->getname();
    }
    return "";
}

void SVSpriteNode::setTexture(SVTexturePtr _tex){
    m_pTex = _tex;
}

void SVSpriteNode::setTexture(SVTEXTYPE _textype){
    m_inTexType = _textype;
}

void SVSpriteNode::setMesh(SVRenderMeshPtr _mesh){
    if (_mesh) {
        m_pMesh = _mesh;
    }
}

void SVSpriteNode::update(f32 dt) {
    SVNode::update(dt);
    if (m_pRenderObj && m_pMesh) {
        if(m_pMtl){
            m_pMtl->setBlendEnable(true);
            m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            m_pMtl->setModelMatrix(m_absolutMat.get());
            m_pMtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
            if(m_inTexType == E_TEX_END) {
                m_pMtl->setTexture(0,m_pTex);
            }else{
                SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_inTexType);
                m_pMtl->setTexture(0,t_tex);
            }
            m_pMtl->update(dt);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(m_pMtl);
        }else{
            //创建新的材质
            SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
            t_mtl->setModelMatrix(m_absolutMat.get());
            t_mtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
            t_mtl->setBlendEnable(true);
            t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            if(m_inTexType == E_TEX_END) {
                t_mtl->setTexture(0,m_pTex);
            }else{
                SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_inTexType);
                t_mtl->setTexture(0,t_tex);
            }
            t_mtl->update(dt);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(t_mtl);
        }
    }
}

void SVSpriteNode::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->m_pGlobalParam->m_curScene->getRenderRS();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVSpriteNode");
        }
    }
    SVNode::render();
}
