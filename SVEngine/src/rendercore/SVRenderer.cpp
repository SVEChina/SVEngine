//
// SVRenderer.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderer.h"
#include "../app/SVInst.h"
#include "../work/SVTdCore.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTextureIOS.h"
#include "SVRenderMgr.h"
#include "SVRenderTarget.h"
#include "SVRenderTexture.h"
#include "SVRObjBase.h"
#include "SVRenderState.h"

SVRenderer::SVRenderer(SVInstPtr _app)
:SVGBaseEx(_app)
,m_pRenderTex(nullptr)
,m_pRState(nullptr)
,m_inWidth(256)
,m_inHeight(256)
,m_outWidth(256)
,m_outHeight(256){
    m_resLock = MakeSharedPtr<SVLock>();
    for(s32 i=E_TEX_MAIN ;i<E_TEX_END;i++) {
        m_svTex[i] = nullptr;
    }
}

SVRenderer::~SVRenderer(){
    m_resLock = nullptr;
}

void SVRenderer::init(s32 _w,s32 _h){
    m_inWidth = _w;
    m_inHeight = _h;
}

void SVRenderer::destroy(){
    m_pRenderTex = nullptr;
    for(s32 i=E_TEX_MAIN ;i<E_TEX_END;i++) {
        m_svTex[i] = nullptr;
    }
    clearRes();
    m_stack_proj.destroy();
    m_stack_view.destroy();
    m_stack_vp.destroy();
    m_resLock = nullptr;
}

//
void SVRenderer::renderBegin() {
}

//
void SVRenderer::renderEnd() {
    clearMatStack();
}

//获取状态
SVRenderStatePtr SVRenderer::getState(){
    return m_pRState;
}

//重置状态
void SVRenderer::resetState() {
    if(m_pRState){
        m_pRState->resetState();
    }
}

void SVRenderer::resize(s32 _w,s32 _) {
}

void SVRenderer::clearRes() {
    m_resLock->lock();
    for(s32 i=0;i<m_robjList.size();i++) {
        SVRObjBasePtr t_robj = m_robjList[i];
        t_robj->destroy(nullptr);
    }
    m_robjList.destroy();
    m_resLock->unlock();
}

void SVRenderer::addRes(SVRObjBasePtr _res) {
    m_resLock->lock();
    m_robjList.append(_res);
    m_resLock->unlock();
}

void SVRenderer::removeRes(SVRObjBasePtr _res) {
    m_resLock->lock();
    for(s32 i=0;i<m_robjList.size();i++) {
        SVRObjBasePtr t_robj = m_robjList[i];
        if(t_robj == _res) {
            t_robj->destroy(nullptr);
            m_robjList.removeForce(i);
            break;
        }
    }
    m_resLock->unlock();
}

//移除不使用的资源
void SVRenderer::removeUnuseRes() {
    m_resLock->lock();
    //小心复值引用计数会加 1！！！！！！！！！！！！！！ 晓帆。。
    for(s32 i=0;i<m_robjList.size();) {
        if(m_robjList[i].use_count() == 1) {
            m_robjList[i]->destroy(nullptr);
            m_robjList.remove(i);
        }else{
            i++;
        }
    }
    m_robjList.reserveForce(m_robjList.size());
    m_resLock->unlock();
}

SVRenderTexturePtr SVRenderer::getRenderTexture() {
    return m_pRenderTex;
}

SVTexturePtr SVRenderer::getSVTex(SVTEXTYPE _type){
    if(E_TEX_END == _type) {
        return nullptr;
    }
    return m_svTex[_type];
}

bool SVRenderer::hasSVTex(SVTEXTYPE _type) {
    if( m_svTex[_type] )
        return true;
    return false;
}

//创建内置纹理 有问题后期删掉
SVTexturePtr SVRenderer::createSVTex(SVTEXTYPE _type,s32 _w,s32 _h,s32 _formate, bool _enableMipMap) {
//    SVTexturePtr t_tex = MakeSharedPtr<SVTexture>(mApp);;
//    SVString t_str("");
//    t_str.printf("intexture_%d",s32(_type));
//    #if defined(SV_ANDROID)
//    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, GL_RGBA, GL_RGBA,_enableMipMap);
//    #else
//    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, GL_RGBA, GL_BGRA,_enableMipMap);
//    #endif
//    mApp->getRenderMgr()->pushRCmdCreate(t_tex);
//    m_svTex[_type] = t_tex;
//    return m_svTex[_type];
    return nullptr;
}

//创建内置纹理 有问题后期删掉
SVTexturePtr SVRenderer::createSVTex(SVTEXTYPE _type,s32 _w,s32 _h,s32 _informate,s32 _daformate, bool _enableMipMap) {
//    SVTexturePtr t_tex = MakeSharedPtr<SVTexture>(mApp);;
//    SVString t_str("");
//    t_str.printf("intexture_%d",s32(_type));
//    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, _informate, _daformate,_enableMipMap);
//    mApp->getRenderMgr()->pushRCmdCreate(t_tex);
//    m_svTex[_type] = t_tex;
//    return m_svTex[_type];
    return nullptr;
}

//创建内置纹理 IOS
SVTexturePtr SVRenderer::createSVTexIOS(SVTEXTYPE _type,s32 _w,s32 _h,s32 _formate, bool _enableMipMap) {
#if defined( SV_IOS )
//    SVTexturePtr t_tex = MakeSharedPtr<SVTextureIOS>(mApp);;
//    SVString t_str("");
//    t_str.printf("intexture_%d",s32(_type));
//    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, GL_RGBA, GL_BGRA,_enableMipMap);
//    mApp->getRenderMgr()->pushRCmdCreate(t_tex);
//    m_svTex[_type] = t_tex;
//    return m_svTex[_type];
#endif
    return nullptr;
}

SVTexturePtr SVRenderer::createSVTexIOS(SVTEXTYPE _type,s32 _w,s32 _h,s32 _informate,s32 _daformate, bool _enableMipMap) {
#if defined( SV_IOS )
//    SVTexturePtr t_tex = MakeSharedPtr<SVTextureIOS>(mApp);
//    SVString t_str("");
//    t_str.printf("intexture_%d",s32(_type));
//    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, _informate, _daformate,_enableMipMap);
//    mApp->getRenderMgr()->pushRCmdCreate(t_tex);
//    m_svTex[_type] = t_tex;
//    return m_svTex[_type];
#endif
    return nullptr;
}

//
void SVRenderer::destroySVTex(SVTEXTYPE _type) {
    m_svTex[_type] = nullptr;
}

//视口
void SVRenderer::svPushViewPort(u32 _x,u32 _y,u32 _w,u32 _h) {
    VPParam t_pm;
    t_pm.m_x = _x;
    t_pm.m_y = _y;
    t_pm.m_width = _w;
    t_pm.m_height = _h;
    m_vpStack.push(t_pm);
}
//退出视口
void SVRenderer::svPopViewPort() {
    m_vpStack.pop();
}

//
void SVRenderer::pushProjMat(FMat4 _mat){
    FMat4 mat4 = _mat;
    m_stack_proj.push(mat4);
}
FMat4 SVRenderer::getProjMat(){
    FMat4 mat4Proj = m_stack_proj.top();
    return mat4Proj;
}
void SVRenderer::popProjMat(){
    m_stack_proj.pop();
}
//
void SVRenderer::pushViewMat(FMat4 _mat){
    FMat4 mat4 = _mat;
    m_stack_view.push(mat4);
}
FMat4 SVRenderer::getViewMat(){
    FMat4 mat4View = m_stack_view.top();;
    return mat4View;
}
void SVRenderer::popViewMat(){
    m_stack_view.pop();
}
//
void SVRenderer::pushVPMat(FMat4 _mat){
    FMat4 mat4 = _mat;
    m_stack_vp.push(mat4);
}
FMat4 SVRenderer::getVPMat(){
    FMat4 mat4VP = m_stack_vp.top();;
    return mat4VP;
}
void SVRenderer::popVPMat(){
    m_stack_vp.pop();
}

void SVRenderer::clearMatStack(){
    m_stack_proj.clear();
    m_stack_view.clear();
    m_stack_vp.clear();
}
