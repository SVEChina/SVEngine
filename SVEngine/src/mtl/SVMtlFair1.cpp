//
// SVMtlFair1.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlFair1.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalParam.h"
#include "../rendercore/SVRendererBase.h"

//
SVMtlFilterBrilaterl::SVMtlFilterBrilaterl(SVInst *_app)
:SVMtlCore(_app,"brilateralfilter") {
    m_texelWidthOffset = 0.0f;
    m_texelHeightOffset = 0.0f;
    m_distanceNormalizationFactor = 5.0f;
    m_paramDirty = true;
}

SVMtlFilterBrilaterl::SVMtlFilterBrilaterl(SVMtlFilterBrilaterl *_mtl)
:SVMtlCore(_mtl){
    m_texelWidthOffset =_mtl->m_texelWidthOffset;
    m_texelHeightOffset = _mtl->m_texelHeightOffset;
    m_distanceNormalizationFactor = _mtl->m_distanceNormalizationFactor;
    m_paramDirty = _mtl->m_paramDirty;
}

SVMtlFilterBrilaterl::~SVMtlFilterBrilaterl() {
}

SVMtlCorePtr SVMtlFilterBrilaterl::clone() {
    return PointerSharedPtr<SVMtlFilterBrilaterl>(new SVMtlFilterBrilaterl(this));
}

void SVMtlFilterBrilaterl::setOffset(f32 _offw,f32 _offh) {
   // if( (m_texelWidthOffset!=_offw) || (m_texelHeightOffset!=_offh)) {
        m_texelWidthOffset = _offw;
        m_texelHeightOffset = _offh;
        m_paramDirty = true;
    //}
}

void SVMtlFilterBrilaterl::setDistance(f32 _dis) {
    if(m_distanceNormalizationFactor!=_dis) {
        m_distanceNormalizationFactor = _dis;
        m_paramDirty = true;
    }
}

void SVMtlFilterBrilaterl::_submitMtl(SVRendererBasePtr _render) {
    if(m_paramDirty) {
        m_paramDirty = false;
        f32 tt_w = m_texelWidthOffset /mApp->m_pGlobalParam->m_inner_width;
        f32 tt_h = m_texelHeightOffset /mApp->m_pGlobalParam->m_inner_height;
        _render->submitUniformf("texelWidthOffset", tt_w);
        _render->submitUniformf("texelHeightOffset", tt_h);
        _render->submitUniformf("distanceNormalizationFactor", m_distanceNormalizationFactor);
        _render->submitUniformf("hlafWidth", 0.5/mApp->m_pGlobalParam->m_inner_width);
        _render->submitUniformf("hlafHeight", 0.5/mApp->m_pGlobalParam->m_inner_height);
    }
}

//
SVMtlFilterBrilaterl2::SVMtlFilterBrilaterl2(SVInst *_app)
:SVMtlCore(_app,"brilateralfilter02") {
    m_texelWidthOffset = 0;
    m_texelHeightOffset = 0;
    m_distanceNormalizationFactor = 5.0f;
    m_paramDirty = true;
}

SVMtlFilterBrilaterl2::SVMtlFilterBrilaterl2(SVMtlFilterBrilaterl2 *_mtl)
:SVMtlCore(_mtl){
    m_texelWidthOffset =_mtl->m_texelWidthOffset;
    m_texelHeightOffset = _mtl->m_texelHeightOffset;
    m_distanceNormalizationFactor = _mtl->m_distanceNormalizationFactor;
    m_paramDirty = _mtl->m_paramDirty;
}

SVMtlFilterBrilaterl2::~SVMtlFilterBrilaterl2() {
}

SVMtlCorePtr SVMtlFilterBrilaterl2::clone() {
    return PointerSharedPtr<SVMtlFilterBrilaterl2>(new SVMtlFilterBrilaterl2(this));
}

//有问题
void SVMtlFilterBrilaterl2::setOffset(f32 _offw,f32 _offh) {
   // if( (m_texelWidthOffset!=_offw) || (m_texelHeightOffset!=_offh)) {
        m_texelWidthOffset = _offw;
        m_texelHeightOffset = _offh;
        m_paramDirty = true;
   // }
}

void SVMtlFilterBrilaterl2::setDistance(f32 _dis) {
    if(m_distanceNormalizationFactor!=_dis) {
        m_distanceNormalizationFactor = _dis;
        m_paramDirty = true;
    }
}

void SVMtlFilterBrilaterl2::_submitMtl(SVRendererBasePtr _render) {
    if(m_paramDirty) {
        m_paramDirty = false;
        f32 tt_w = m_texelWidthOffset /mApp->m_pGlobalParam->m_inner_width;
        f32 tt_h = m_texelHeightOffset /mApp->m_pGlobalParam->m_inner_height;
        _render->submitUniformf("texelWidthOffset", tt_w);
        _render->submitUniformf("texelHeightOffset", tt_h);
        _render->submitUniformf("distanceNormalizationFactor", m_distanceNormalizationFactor);
        _render->submitUniformf("hlafWidth", 0.5/mApp->m_pGlobalParam->m_inner_width);
        _render->submitUniformf("hlafHeight", 0.5/mApp->m_pGlobalParam->m_inner_height);
    }
}

//
SVMtlGaussian::SVMtlGaussian(SVInst *_app)
:SVMtlCore(_app,"gaussian") {
    m_radius = 1.0f;
    m_paramDirty = true;
}

SVMtlGaussian::SVMtlGaussian(SVMtlGaussian *_mtl)
:SVMtlCore(_mtl){
    m_radius = _mtl->m_radius;
    m_paramDirty = _mtl->m_paramDirty;
}

SVMtlGaussian::~SVMtlGaussian() {
}

SVMtlCorePtr SVMtlGaussian::clone() {
    return PointerSharedPtr<SVMtlGaussian>(new SVMtlGaussian(this));
}

void SVMtlGaussian::setRadius(f32 _radius) {
    if(m_radius!=_radius) {
        m_radius = _radius;
        m_paramDirty = true;
    }
}

void SVMtlGaussian::_submitMtl(SVRendererBasePtr _render) {
    if(m_paramDirty) {
        m_paramDirty = false;
        f32 t_w = mApp->m_pGlobalParam->m_inner_width;
        f32 t_h = mApp->m_pGlobalParam->m_inner_height;
        _render->submitUniformf("texelWidthOffset", t_w);
        _render->submitUniformf("texelHeightOffset", t_h);
        _render->submitUniformf("radius", m_radius);
    }
}

SVMtlBlurFairGus::SVMtlBlurFairGus(SVInst *_app)
:SVMtlCore(_app,"blur_fair_gus") {
    
}

SVMtlBlurFairGus::SVMtlBlurFairGus(SVMtlBlurFairGus *_mtl)
:SVMtlCore(_mtl){
    
}

SVMtlBlurFairGus::~SVMtlBlurFairGus(){
    
}

void SVMtlBlurFairGus::setSmooth(f32 _smooth){
    
}

void SVMtlBlurFairGus::_submitMtl(SVRendererBasePtr _render){
    
}

SVMtlCorePtr SVMtlBlurFairGus::clone(){
     return PointerSharedPtr<SVMtlBlurFairGus>(new SVMtlBlurFairGus(this));
}

SVMtlBlur::SVMtlBlur(SVInst *_app)
:SVMtlCore(_app,"blur") {
    m_radius = 1.0f;
    m_paramDirty = true;
}

SVMtlBlur::SVMtlBlur(SVMtlBlur *_mtl)
:SVMtlCore(_mtl){
    m_radius = _mtl->m_radius;
    m_paramDirty = _mtl->m_paramDirty;
}

SVMtlBlur::~SVMtlBlur() {
}

SVMtlCorePtr SVMtlBlur::clone() {
    return PointerSharedPtr<SVMtlBlur>(new SVMtlBlur(this));
}

void SVMtlBlur::setRadius(f32 _radius) {
    if(m_radius!=_radius) {
        m_radius = _radius;
        m_paramDirty = true;
    }
}

void SVMtlBlur::_submitMtl(SVRendererBasePtr _render) {
    if(m_paramDirty) {
        m_paramDirty = false;
        f32 t_w = mApp->m_pGlobalParam->m_inner_width;
        f32 t_h = mApp->m_pGlobalParam->m_inner_height;
        _render->submitUniformf("texelWidthOffset", t_w);
        _render->submitUniformf("texelHeightOffset", t_h);
        _render->submitUniformf("softenStrength", m_radius);
    }
}

SVMtlBlurFair::SVMtlBlurFair(SVInst *_app)
:SVMtlCore(_app,"blur_fair_hipass"){

}
    
SVMtlBlurFair::SVMtlBlurFair(SVMtlBlurFair *_mtl)
:SVMtlCore(_mtl){
    m_blurAlpha = _mtl->m_blurAlpha;
}
    
SVMtlBlurFair::~SVMtlBlurFair(){
}

void SVMtlBlurFair::_submitMtl(SVRendererBasePtr _render){
    _render->submitUniformf("blurAlpha", m_blurAlpha);
}

void SVMtlBlurFair::setSmooth(f32 _smooth){
    m_blurAlpha=_smooth;
}

SVMtlCorePtr SVMtlBlurFair::clone(){
    return PointerSharedPtr<SVMtlBlurFair>(new SVMtlBlurFair(this));
}

SVMtlFairLtraLow::SVMtlFairLtraLow(SVInst *_app)
:SVMtlCore(_app,"beauty02"){
     m_smooth=0.0;
}

SVMtlFairLtraLow::SVMtlFairLtraLow(SVMtlFairLtraLow *_mtl)
:SVMtlCore(_mtl){
    m_smooth=_mtl->m_smooth;
}

SVMtlFairLtraLow::~SVMtlFairLtraLow(){
}

void SVMtlFairLtraLow::setSmooth(f32 _smooth){
    m_smooth=_smooth;
}

void SVMtlFairLtraLow::_submitMtl(SVRendererBasePtr _render){
    _render->submitUniformf("softenStrength", m_smooth);
}

SVMtlCorePtr SVMtlFairLtraLow::clone(){
    return PointerSharedPtr<SVMtlFairLtraLow>(new SVMtlFairLtraLow(this));
}

