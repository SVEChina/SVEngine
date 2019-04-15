//
// SVRenderCmd.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderCmd.h"
#include "SVFboObject.h"
#include "SVRenderScene.h"
#include "SVRenderTexture.h"
#include "renderer/SVRendererBase.h"
#include "../base/SVDataSwap.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../event/SVOpEvent.h"
#include "../node/SVScene.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include <sys/time.h>

//
SVRenderCmd::SVRenderCmd() {
    mTag = "SVRenderCmd";
    m_pRenderer = nullptr;
}

SVRenderCmd::~SVRenderCmd() {
    m_pRenderer = nullptr;
}

void SVRenderCmd::setRenderer(SVRendererBasePtr _renderer) {
    m_pRenderer = _renderer;
}

void SVRenderCmd::render() {
}

//创建指令
SVRCmdCreate::SVRCmdCreate(SVRObjBasePtr _robj) {
    m_pRObj = _robj;
}

SVRCmdCreate::~SVRCmdCreate() {
    m_pRObj = nullptr;
    m_pRenderer = nullptr;
}

void SVRCmdCreate::render(){
    if(m_pRObj){
        m_pRObj->create(m_pRenderer);
        if(m_pRenderer) {
            m_pRenderer->addRes(m_pRObj);
        }
    }
}

//普通渲染指令
SVRenderCmdNor::SVRenderCmdNor() {
    m_pMtl  = nullptr;
    m_pMesh = nullptr;
}

SVRenderCmdNor::~SVRenderCmdNor() {
    m_pMtl  = nullptr;
    m_pMesh = nullptr;
}

void SVRenderCmdNor::setMesh(SVRenderMeshPtr _mesh){
    m_pMesh = _mesh;
}

void SVRenderCmdNor::setMaterial(SVMtlCorePtr _mtl){
    m_pMtl = _mtl;
}

void SVRenderCmdNor::render() {
    if (m_pMtl && m_pMesh) {
        if (m_pMtl->submitMtl()) {
            m_pMesh->render(m_pRenderer);
        }
    }
}

//
SVRenderCmdClear::SVRenderCmdClear()
:m_color_r(1.0f)
,m_color_g(1.0f)
,m_color_b(1.0f)
,m_color_a(1.0f){
}

SVRenderCmdClear::~SVRenderCmdClear(){
}

void SVRenderCmdClear::setClearColor(f32 _r,f32 _g,f32 _b,f32 _a) {
    m_color_r = _r;
    m_color_g = _g;
    m_color_b = _b;
    m_color_a = _a;
}

void SVRenderCmdClear::render(){
    glClearColor(m_color_r,m_color_g,m_color_b,m_color_a);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}

//
SVRenderCmdAdapt::SVRenderCmdAdapt()
:m_winWidth(720)
,m_winHeight(1280){
}

SVRenderCmdAdapt::~SVRenderCmdAdapt(){
}

void SVRenderCmdAdapt::setWinSize(s32 _w,s32 _h){
    m_winWidth = _w;
    m_winHeight = _h;
}

void SVRenderCmdAdapt::render(){
    glViewport( 0, 0,m_winWidth,m_winHeight);
    glClearColor(m_color_r,m_color_g,m_color_b,m_color_a);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    SVRenderCmdNor::render();
}

//渲染命令批次
SVRenderCmdPass::SVRenderCmdPass() {
    m_fbo = nullptr;
    m_tex = nullptr;
}

SVRenderCmdPass::~SVRenderCmdPass(){
    m_fbo = nullptr;
    m_tex = nullptr;
    m_pMtl = nullptr;
    m_pMesh = nullptr;
}

void SVRenderCmdPass::setFbo(SVRenderTexturePtr _fbo) {
    m_fbo = _fbo;
}

void SVRenderCmdPass::setTexture(SVTexturePtr _tex) {
    m_tex = _tex;
}

void SVRenderCmdPass::render() {
    if(m_fbo && m_tex ) {
        m_fbo->setTexture(m_tex);
        m_fbo->bind();
        m_fbo->clear();
        if(m_pMtl && m_pMesh) {
            if (m_pMtl->submitMtl()) {
                m_pMesh->render(m_pRenderer);
            }
        }
        m_fbo->unbind();
    }
}

SVRenderCmdPassCollection::SVRenderCmdPassCollection(){
    m_fbo = nullptr;
    m_tex = nullptr;
    m_MtlArray.clear();
    m_MeshArray.clear();
}

SVRenderCmdPassCollection::~SVRenderCmdPassCollection(){
    m_fbo = nullptr;
    m_tex = nullptr;
    m_MtlArray.clear();
    m_MeshArray.clear();
}

void SVRenderCmdPassCollection::render(){
    if(m_fbo && m_tex ) {
        m_fbo->setTexture(m_tex);
        m_fbo->bind();
        m_fbo->clear();
        for(int i=0;i<m_MtlArray.size();i++){
            SVMtlCorePtr t_mtl = m_MtlArray.get(i);
            SVRenderMeshPtr t_mesh = m_MeshArray.get(i);
            if(t_mtl && t_mesh) {
                if (t_mtl->submitMtl()) {
                    t_mesh->render(m_pRenderer);
                }
            }
        }
        m_fbo->unbind();
    }
}

void SVRenderCmdPassCollection::setFbo(SVRenderTexturePtr _fbo){
    m_fbo = _fbo;
}

void SVRenderCmdPassCollection::setTexture(SVTexturePtr _tex){
    m_tex = _tex;
}

void SVRenderCmdPassCollection::addMtlMesh(SVMtlCorePtr _mtl , SVRenderMeshPtr _mesh){
    m_MtlArray.append(_mtl);
    m_MeshArray.append(_mesh);
}

//激活FBO(fbo static)
SVRenderCmdFboBind::SVRenderCmdFboBind(SVFboObjectPtr _fbo) {
    m_fbo = _fbo;
}

SVRenderCmdFboBind::~SVRenderCmdFboBind(){
    m_fbo = nullptr;
}

void SVRenderCmdFboBind::render() {
    if (m_fbo) {
        m_fbo->bind();
    }
}

//不激活FBO(fbo static)
SVRenderCmdFboUnbind::SVRenderCmdFboUnbind(SVFboObjectPtr _fbo) {
    m_fbo = _fbo;
}

SVRenderCmdFboUnbind::~SVRenderCmdFboUnbind(){
    m_fbo = nullptr;
}

void SVRenderCmdFboUnbind::render() {
    if (m_fbo) {
        m_fbo->unbind();
    }
}

