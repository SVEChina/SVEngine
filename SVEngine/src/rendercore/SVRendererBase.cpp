//
// SVRendererBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRendererBase.h"
#include "../app/SVInst.h"
#include "../base/SVLock.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTextureIOS.h"
#include "SVRenderMgr.h"
#include "SVRenderTarget.h"
#include "SVRenderTexture.h"
#include "SVRObjBase.h"
#include "SVRenderState.h"

SVRendererBase::SVRendererBase(SVInst* _app)
:SVGBase(_app)
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

SVRendererBase::~SVRendererBase(){
    m_resLock = nullptr;
}

void SVRendererBase::init(s32 _ver,void* _windows,void* context,s32 _w,s32 _h){
    m_inWidth = _w;
    m_inHeight = _h;
}

void SVRendererBase::destroy(){
    m_pRenderTex = nullptr;
    for(s32 i=E_TEX_MAIN ;i<E_TEX_END;i++) {
        m_svTex[i] = nullptr;
    }
    clearRes();
    //
    m_stack_proj.destroy();
    m_stack_view.destroy();
    m_stack_vp.destroy();
    //
    m_resLock = nullptr;
}

//
void SVRendererBase::renderBegin() {
}

//
void SVRendererBase::renderEnd() {
    clearMatStack();
}

//获取状态
SVRenderStatePtr SVRendererBase::getState(){
    return m_pRState;
}

//重置状态
void SVRendererBase::resetState() {
    if(m_pRState){
        m_pRState->resetState();
    }
}

void SVRendererBase::resize(s32 _w,s32 _) {
}

void SVRendererBase::clearRes() {
    m_resLock->lock();
    for(s32 i=0;i<m_robjList.size();i++) {
        SVRObjBasePtr t_robj = m_robjList[i];
        t_robj->destroy(nullptr);
    }
    m_robjList.destroy();
    m_resLock->unlock();
}

void SVRendererBase::addRes(SVRObjBasePtr _res) {
    m_resLock->lock();
    m_robjList.append(_res);
    m_resLock->unlock();
}

void SVRendererBase::removeRes(SVRObjBasePtr _res) {
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
void SVRendererBase::removeUnuseRes() {
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

SVRenderTexturePtr SVRendererBase::getRenderTexture() {
    return m_pRenderTex;
}

SVTexturePtr SVRendererBase::getSVTex(SVTEXTYPE _type){
    if(E_TEX_END == _type) {
        return nullptr;
    }
    return m_svTex[_type];
}

bool SVRendererBase::hasSVTex(SVTEXTYPE _type) {
    if( m_svTex[_type] )
        return true;
    return false;
}

//创建内置纹理 有问题后期删掉
SVTexturePtr SVRendererBase::createSVTex(SVTEXTYPE _type,s32 _w,s32 _h,s32 _formate, bool _enableMipMap) {
    SVTexturePtr t_tex = MakeSharedPtr<SVTexture>(mApp);;
    SVString t_str("");
    t_str.printf("intexture_%d",s32(_type));
    #if defined(SV_ANDROID)
    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, GL_RGBA, GL_RGBA,_enableMipMap);
    #else
    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, GL_RGBA, GL_BGRA,_enableMipMap);
    #endif
    mApp->getRenderMgr()->pushRCmdCreate(t_tex);
    m_svTex[_type] = t_tex;
    return m_svTex[_type];
}

//创建内置纹理 有问题后期删掉
SVTexturePtr SVRendererBase::createSVTex(SVTEXTYPE _type,s32 _w,s32 _h,s32 _informate,s32 _daformate, bool _enableMipMap) {
    SVTexturePtr t_tex = MakeSharedPtr<SVTexture>(mApp);;
    SVString t_str("");
    t_str.printf("intexture_%d",s32(_type));
    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, _informate, _daformate,_enableMipMap);
    mApp->getRenderMgr()->pushRCmdCreate(t_tex);
    m_svTex[_type] = t_tex;
    return m_svTex[_type];
}

//创建内置纹理 IOS
SVTexturePtr SVRendererBase::createSVTexIOS(SVTEXTYPE _type,s32 _w,s32 _h,s32 _formate, bool _enableMipMap) {
#if defined( SV_IOS )
    SVTexturePtr t_tex = MakeSharedPtr<SVTextureIOS>(mApp);;
    SVString t_str("");
    t_str.printf("intexture_%d",s32(_type));
    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, GL_RGBA, GL_BGRA,_enableMipMap);
    mApp->getRenderMgr()->pushRCmdCreate(t_tex);
    m_svTex[_type] = t_tex;
    return m_svTex[_type];
#endif
    return nullptr;
}

SVTexturePtr SVRendererBase::createSVTexIOS(SVTEXTYPE _type,s32 _w,s32 _h,s32 _informate,s32 _daformate, bool _enableMipMap) {
#if defined( SV_IOS )
    SVTexturePtr t_tex = MakeSharedPtr<SVTextureIOS>(mApp);
    SVString t_str("");
    t_str.printf("intexture_%d",s32(_type));
    t_tex->init(t_str.c_str(), GL_TEXTURE_2D, _w, _h, _informate, _daformate,_enableMipMap);
    mApp->getRenderMgr()->pushRCmdCreate(t_tex);
    m_svTex[_type] = t_tex;
    return m_svTex[_type];
#endif
    return nullptr;
}

//
void SVRendererBase::destroySVTex(SVTEXTYPE _type) {
    m_svTex[_type] = nullptr;
}

//视口
void SVRendererBase::svPushViewPort(u32 _x,u32 _y,u32 _w,u32 _h) {
    VPParam t_pm;
    t_pm.m_x = _x;
    t_pm.m_y = _y;
    t_pm.m_width = _w;
    t_pm.m_height = _h;
    m_vpStack.push(t_pm);
}
//退出视口
void SVRendererBase::svPopViewPort() {
    m_vpStack.pop();
}

//
void SVRendererBase::pushProjMat(FMat4 _mat){
    FMat4 mat4 = _mat;
    m_stack_proj.push(mat4);
}
FMat4 SVRendererBase::getProjMat(){
    FMat4 mat4Proj = m_stack_proj.top();
    return mat4Proj;
}
void SVRendererBase::popProjMat(){
    m_stack_proj.pop();
}
//
void SVRendererBase::pushViewMat(FMat4 _mat){
    FMat4 mat4 = _mat;
    m_stack_view.push(mat4);
}
FMat4 SVRendererBase::getViewMat(){
    FMat4 mat4View = m_stack_view.top();;
    return mat4View;
}
void SVRendererBase::popViewMat(){
    m_stack_view.pop();
}
//
void SVRendererBase::pushVPMat(FMat4 _mat){
    FMat4 mat4 = _mat;
    m_stack_vp.push(mat4);
}
FMat4 SVRendererBase::getVPMat(){
    FMat4 mat4VP = m_stack_vp.top();;
    return mat4VP;
}
void SVRendererBase::popVPMat(){
    m_stack_vp.pop();
}

void SVRendererBase::clearMatStack(){
    m_stack_proj.clear();
    m_stack_view.clear();
    m_stack_vp.clear();
}
