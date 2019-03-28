//
// SVMtlParamBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlParamBase.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVFboObject.h"

SVMtlParam::SVMtlParam(){
}

SVMtlParam::~SVMtlParam(){
}

void SVMtlParam::reset(){
}

//
SVMatrixParam::SVMatrixParam(){
    reset();
}

SVMatrixParam::~SVMatrixParam(){
}

void SVMatrixParam::reset(){
    memset(m_mat_model, 0, sizeof(f32) * 16);
    memset(m_mat_view, 0, sizeof(f32) * 16);
    memset(m_mat_project, 0, sizeof(f32) * 16);
    memset(m_mat_vp, 0, sizeof(f32) * 16);
    m_mat_model[0] = m_mat_model[5] = m_mat_model[10] = m_mat_model[15] = 1.0f;
    m_mat_view[0] = m_mat_view[5] = m_mat_view[10] = m_mat_view[15] = 1.0f;
    m_mat_project[0] = m_mat_project[5] = m_mat_project[10] = m_mat_project[15] = 1.0f;
    m_mat_vp[0] = m_mat_vp[5] = m_mat_vp[10] = m_mat_vp[15] = 1.0f;
}

void SVMatrixParam::copy(SVMatrixParam& _param){
    memcpy(m_mat_model,_param.m_mat_model,sizeof(f32)*16);
    memcpy(m_mat_view,_param.m_mat_view,sizeof(f32)*16);
    memcpy(m_mat_project,_param.m_mat_project,sizeof(f32)*16);
    memcpy(m_mat_vp,_param.m_mat_vp,sizeof(f32)*16);
}

//

TexUnit::TexUnit(){
    m_texForm = E_TEX_END;
    m_min_filter = E_T_FILTER_LINEAR;
    m_mag_filter = E_T_FILTER_LINEAR;
    m_s_wrap = E_T_WRAP_CLAMP_TO_EDAGE;
    m_t_wrap = E_T_WRAP_CLAMP_TO_EDAGE;
    m_pTex = nullptr;
};

TexUnit::~TexUnit(){
    m_pTex = nullptr;
}

void TexUnit::reset(){
    m_pTex = nullptr;
    memset(m_texcoordFlip, 1, sizeof(f32) * 2);
}

void TexUnit::copy(TexUnit& _texunit){
    m_pTex = _texunit.m_pTex;
    m_texcoordFlip[0] = _texunit.m_texcoordFlip[0];
    m_texcoordFlip[1] = _texunit.m_texcoordFlip[1];
    m_texForm = _texunit.m_texForm;
    m_min_filter = _texunit.m_min_filter;
    m_mag_filter = _texunit.m_mag_filter;
    m_s_wrap = _texunit.m_s_wrap;
    m_t_wrap = _texunit.m_t_wrap;
}

//纹理参数
SVTextureParam::SVTextureParam(){
    reset();
}

SVTextureParam::~SVTextureParam(){
    reset();
}

void SVTextureParam::setTexture(u32 _index,SVTexturePtr _tex){
    if(_index<0 ||_index>=MAX_TEXUNIT)
        return;
    m_texUnit[_index].m_pTex = _tex;
}

void SVTextureParam::setTexture(u32 _index,SVTEXTYPE _from){
    if(_index<0 ||_index>=MAX_TEXUNIT)
        return;
    m_texUnit[_index].m_texForm = _from;
}

void SVTextureParam::setTexClip(u32 _index,f32 _x,f32 _y){
    if(_index<0 ||_index>=MAX_TEXUNIT)
        return;
    m_texUnit[_index].m_texcoordFlip[0] = _x;
    m_texUnit[_index].m_texcoordFlip[1] = _y;
}

//设置纹理环绕
void SVTextureParam::setTexWrapS(u32 _index,s32 _type) {
    if(_index<0 ||_index>=MAX_TEXUNIT)
        return;
    m_texUnit[_index].m_s_wrap = _type;
}

void SVTextureParam::setTexWrapT(u32 _index,s32 _type) {
    if(_index<0 ||_index>=MAX_TEXUNIT)
        return;
    m_texUnit[_index].m_t_wrap = _type;
}

//设置纹理过滤器
void SVTextureParam::setTexFilterMax(u32 _index,s32 _type) {
    if(_index<0 ||_index>=MAX_TEXUNIT)
        return;
    m_texUnit[_index].m_mag_filter = _type;
}

void SVTextureParam::setTexFilterMin(u32 _index,s32 _type) {
    if(_index<0 ||_index>=MAX_TEXUNIT)
        return;
    m_texUnit[_index].m_min_filter = _type;
}

//重置
void SVTextureParam::reset(){
    for(s32 i=0;i<MAX_TEXUNIT;i++){
        m_texUnit[i].reset();
    }
}

void SVTextureParam::copy(SVTextureParam& _param){
    SV_LOG_INFO("SVTextureParam::copy begin\n");
    for(s32 i=0;i<MAX_TEXUNIT;i++){
        m_texUnit[i].copy(_param.m_texUnit[i]);
    }
    SV_LOG_INFO("SVTextureParam::copy end\n");
}

//
SVBlendParam::SVBlendParam(){
    reset();
}

SVBlendParam::~SVBlendParam(){
}

void SVBlendParam::reset(){
    enable = false;
    srcParam = GL_ONE;
    dstParam = GL_ONE_MINUS_SRC_ALPHA;
}

void SVBlendParam::copy(SVBlendParam& _param){
    enable = _param.enable;
    srcParam = _param.srcParam;
    dstParam = _param.dstParam;
}

SVCullFaceParam::SVCullFaceParam(){
    reset();
}

SVCullFaceParam::~SVCullFaceParam(){
}

void SVCullFaceParam::reset(){
    enable = false;
    frontFace = GL_CCW;
    cullFace = GL_BACK;
}

void SVCullFaceParam::copy(SVCullFaceParam& _param){
    enable = _param.enable;
    frontFace = _param.frontFace;
    cullFace = _param.cullFace;
}

//
SVDepthParam::SVDepthParam(){
    reset();
}

SVDepthParam::~SVDepthParam(){
}

void SVDepthParam::reset(){
    //
    enable = false;
    clear = false;
    testMethod = GL_LEQUAL;
}

void SVDepthParam::copy(SVDepthParam& _param){
    enable = _param.enable;
    clear = _param.clear;
    testMethod = _param.testMethod;
}

//
SVStencilParam::SVStencilParam(){
    reset();
}

SVStencilParam::~SVStencilParam(){
}

void SVStencilParam::reset(){
    //
    enable = false;
    clear = false;
    passMethod = GL_ALWAYS;
    refValue = 1;
    maskValue = 1;
    sfail = GL_KEEP;
    zfail = GL_INCR;
    zpass = GL_INCR;
}

void SVStencilParam::copy(SVStencilParam& _param){
    enable = _param.enable;
    clear = _param.clear;
    passMethod = _param.passMethod;
    refValue = _param.refValue;
    maskValue = _param.maskValue;
    sfail = _param.sfail;
    zfail = _param.zfail;
    zpass = _param.zpass;
}

//
SVAlphaParam::SVAlphaParam(){
    reset();
}

SVAlphaParam::~SVAlphaParam(){
}

void SVAlphaParam::reset(){
}

void SVAlphaParam::copy(SVAlphaParam& _param){
}

//
SVSizeParam::SVSizeParam(){
    reset();
}

SVSizeParam::~SVSizeParam(){
}

void SVSizeParam::reset(){
    memset(m_tex0size, 0, sizeof(f32) * 2);
    memset(m_tex1size, 0, sizeof(f32) * 2);
    memset(m_tex2size, 0, sizeof(f32) * 2);
    memset(m_tex3size, 0, sizeof(f32) * 2);
    m_ptsize = 5.0f;
    m_linewidth = 5.0f;
}

void SVSizeParam::copy(SVSizeParam& _param){
    m_tex0size[0] = _param.m_tex0size[0];
    m_tex0size[1] = _param.m_tex0size[1];
    m_tex1size[0] = _param.m_tex1size[0];
    m_tex1size[1] = _param.m_tex1size[1];
    m_tex2size[0] = _param.m_tex2size[0];
    m_tex2size[1] = _param.m_tex2size[1];
    m_tex3size[0] = _param.m_tex3size[0];
    m_tex3size[1] = _param.m_tex3size[1];
    m_ptsize = _param.m_ptsize;
    m_linewidth = _param.m_linewidth;
}

//
//
SVZOffParam::SVZOffParam(){
    reset();
}

SVZOffParam::~SVZOffParam(){
}

void SVZOffParam::reset(){
    enable = false;
    m_factor = 0.0f;
    m_unit = 0.0f;
}

void SVZOffParam::copy(SVZOffParam& _param){
    enable = _param.enable;
    m_factor = _param.m_factor;
    m_unit = _param.m_unit;
}


//融合参数
SVBlendModeParam::SVBlendModeParam(){
    reset();
}

SVBlendModeParam::~SVBlendModeParam(){
    
}

void SVBlendModeParam::reset(){
    m_blendmode = 0;
}

void SVBlendModeParam::copy(SVBlendModeParam& _param){
    m_blendmode = _param.m_blendmode;
}









