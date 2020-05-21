//
// SVMtlBasedOn.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlBasedOn.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderer.h"

SVMtlBasedOn::SVMtlBasedOn(SVInst *_app)
:SVMtlCore(_app,"baseFilterAdjustment") {
    //
    m_BaseOnFlag = MTL_F_BASEON_NONE;
    //
    m_shadows=0.0;
    m_highlights=0.0;
    m_contrast=0.0;
    m_saturation=0.0;
    m_brightness=0.0;
    m_whitening=0.0;
    m_gamma=0.0;
    m_redShift=0.0;
    m_greenShift=0.0;
    m_blueShift=0.0;
    m_hhblueShift=0.0;
    m_hhredShift=0.0;
    m_hhgreenShift=0.0;
    m_sdredShift=0.0;
    m_sdblueShift=0.0;
    m_sdgreenShift=0.0;
}

SVMtlBasedOn::SVMtlBasedOn(SVMtlBasedOn* _mtl)
:SVMtlCore(_mtl){
    m_BaseOnFlag = _mtl->m_BaseOnFlag;
    //
    m_shadows=_mtl->m_shadows;
    m_highlights=_mtl->m_highlights;
    m_contrast=_mtl->m_contrast;
    m_saturation=_mtl->m_saturation;
    m_brightness=_mtl->m_brightness;
    m_whitening=_mtl->m_whitening;
    m_gamma=_mtl->m_gamma;
    m_redShift=_mtl->m_redShift;
    m_greenShift=_mtl->m_greenShift;
    m_blueShift=_mtl->m_blueShift;
    m_hhblueShift=_mtl->m_hhblueShift;
    m_hhredShift=_mtl->m_hhredShift;
    m_hhgreenShift=_mtl->m_hhgreenShift;
    m_sdredShift=_mtl->m_sdredShift;
    m_sdblueShift=_mtl->m_sdblueShift;
    m_sdgreenShift=_mtl->m_sdgreenShift;
}

SVMtlBasedOn::~SVMtlBasedOn(){
}

SVMtlCorePtr SVMtlBasedOn::clone() {
    //return MakeSharedPtr<SVMtlBasedOn>(THIS_TO_SHAREPTR(SVMtlBasedOn));
    return PointerSharedPtr<SVMtlBasedOn>(new SVMtlBasedOn(this));
}

void SVMtlBasedOn::setShadows(f32 _shadows) {
    if(m_shadows!=_shadows){
        m_BaseOnFlag |= MTL_F_BASEON_SHADOW;
        m_shadows = _shadows;
    }
}

void SVMtlBasedOn::setHighlights(f32 _highlights) {
    if(m_highlights!=_highlights){
        m_BaseOnFlag |= MTL_F_BASEON_HL;
        m_highlights = _highlights;
    }
}

void SVMtlBasedOn::setContrast(f32 _contrast) {
    if(m_contrast!=_contrast){
        m_BaseOnFlag |= MTL_F_BASEON_CONSTRAST;
        m_contrast = _contrast;
    }
}

void SVMtlBasedOn::setSaturation(f32 _saturation) {
    if(m_saturation!=_saturation){
        m_BaseOnFlag |= MTL_F_BASEON_SATURATION;
        m_saturation = _saturation;
    }
}

void SVMtlBasedOn::setBrightness(f32 _brightness) {
    if(m_brightness!=_brightness){
        m_BaseOnFlag |= MTL_F_BASEON_BRIGHTNESS;
        m_brightness= _brightness;
    }
}

void SVMtlBasedOn::setWhitening(f32 _whitening) {
    if(m_whitening!=_whitening){
        m_BaseOnFlag |= MTL_F_BASEON_WHITENING;
        m_whitening= _whitening;
    }
}

void SVMtlBasedOn::setGamma(f32 _gamma) {
    if(m_gamma!=_gamma){
        m_BaseOnFlag |= MTL_F_BASEON_GAMMA;
        m_gamma= _gamma;
    }
}

void SVMtlBasedOn::setRedShift(f32 redShift) {
    if(m_redShift!=redShift){
        m_BaseOnFlag |= MTL_F_BASEON_REDSHIFT;
        m_redShift= redShift;
    }
}

void SVMtlBasedOn::setGreenShift(f32 _greenShift) {
    if(m_greenShift!=_greenShift){
        m_BaseOnFlag |= MTL_F_BASEON_GREENSHIFT;
        m_greenShift= _greenShift;
    }
}

void SVMtlBasedOn::setBlueShift(f32 _blueShift) {
    if(m_blueShift!=_blueShift){
        m_BaseOnFlag |= MTL_F_BASEON_BLUESHIFT;
        m_blueShift= _blueShift;
    }
}

void SVMtlBasedOn::setSDRedShift(f32 redShift) {
    if(m_sdredShift!=redShift){
        m_BaseOnFlag |= MTL_F_BASEON_SDREDSHIFT;
        m_sdredShift= redShift;
    }
}

void SVMtlBasedOn::setSDGreenShift(f32 _greenShift) {
    if(m_sdgreenShift!=_greenShift){
        m_BaseOnFlag |= MTL_F_BASEON_SDGREENSHIFT;
        m_sdgreenShift= _greenShift;
    }
}

void SVMtlBasedOn::setSDBlueShift(f32 _blueShift) {
    if(m_sdblueShift!=_blueShift){
        m_BaseOnFlag |= MTL_F_BASEON_SDBLUESHIFT;
        m_sdblueShift= _blueShift;
    }
}

void SVMtlBasedOn::setHHRedShift(f32 redShift) {
    if(m_hhredShift!=redShift){
        m_BaseOnFlag |= MTL_F_BASEON_HHREDSHIFT;
        m_hhredShift= redShift;
    }
}

void SVMtlBasedOn::setHHGreenShift(f32 _greenShift) {
    if(m_hhgreenShift!=_greenShift){
        m_BaseOnFlag |= MTL_F_BASEON_HHGREENSHIFT;
        m_hhgreenShift= _greenShift;
    }
}

void SVMtlBasedOn::setHHBlueShift(f32 _blueShift) {
    if(m_hhblueShift!=_blueShift){
        m_BaseOnFlag |= MTL_F_BASEON_HHBLUESHIFT;
        m_hhblueShift= _blueShift;
    }
}

void SVMtlBasedOn::setTemperature(f32 _temperature){
    if(m_temperature!=_temperature){
        m_BaseOnFlag |= MTL_F_BASEON_TEMPERATURE;
        m_temperature= _temperature;
    }
}

void SVMtlBasedOn::setTint(f32 _tint){
    if(m_tint!=_tint){
        m_BaseOnFlag |= MTL_F_BASEON_TINT;
        m_tint= _tint;
    }
}

void SVMtlBasedOn::_submitMtl(SVRendererPtr _render) {
    if((m_BaseOnFlag&MTL_F_BASEON_SHADOW)>0){
        _render->submitUniformf("shadows", m_shadows);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_HL)>0){
        _render->submitUniformf("highlights", m_highlights);
    }

    if((m_BaseOnFlag&MTL_F_BASEON_CONSTRAST)>0){
        _render->submitUniformf("contrast", m_contrast);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_SATURATION)>0){
        _render->submitUniformf("saturation", m_saturation);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_BRIGHTNESS)>0){
        _render->submitUniformf("brightness", m_brightness);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_WHITENING)>0){
        _render->submitUniformf("whitening", m_whitening);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_GAMMA)>0){
        _render->submitUniformf("gamma", m_gamma);
    }

    if((m_BaseOnFlag&MTL_F_BASEON_REDSHIFT)>0){
        _render->submitUniformf("redShift", m_redShift);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_GREENSHIFT)>0){
        _render->submitUniformf("greenShift", m_greenShift);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_BLUESHIFT)>0){
        _render->submitUniformf("blueShift", m_blueShift);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_SDREDSHIFT)>0){
        _render->submitUniformf("sdredShift", m_sdredShift);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_SDGREENSHIFT)>0){
        _render->submitUniformf("sdgreenShift", m_sdgreenShift);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_SDBLUESHIFT)>0){
        _render->submitUniformf("sdblueShift", m_sdblueShift);
    }

    if((m_BaseOnFlag&MTL_F_BASEON_HHREDSHIFT)>0){
        _render->submitUniformf("hhredShift", m_hhredShift);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_HHGREENSHIFT)>0){
        _render->submitUniformf("hhgreenShift", m_hhgreenShift);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_HHBLUESHIFT)>0){
        _render->submitUniformf("hhblueShift", m_hhblueShift);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_TEMPERATURE)>0){
        _render->submitUniformf("temperature", m_temperature);
    }
    
    if((m_BaseOnFlag&MTL_F_BASEON_TINT)>0){
        _render->submitUniformf("tint", m_tint);
    }
    
    m_BaseOnFlag = MTL_F_BASEON_NONE;
}

SVMtlAcutance::SVMtlAcutance(SVInst *_app)
:SVMtlCore(_app,"acutance") {
    m_BaseOnFlag = MTL_F_BASEON_NONE;
    m_acutance=0.0;
}

SVMtlAcutance::SVMtlAcutance(SVMtlAcutance *_mtl)
:SVMtlCore(_mtl) {
    m_BaseOnFlag = _mtl->m_BaseOnFlag;
    m_acutance=_mtl->m_acutance;
}

SVMtlAcutance::~SVMtlAcutance(){
}

SVMtlCorePtr SVMtlAcutance::clone() {
    return PointerSharedPtr<SVMtlAcutance>(new SVMtlAcutance(this));
}

void SVMtlAcutance::setAcutance(f32 _acutance) {
    if(m_acutance!=_acutance){
        m_BaseOnFlag |= MTL_F_BASEON_ACUTANCE;
        m_acutance = _acutance;
    }
}

void SVMtlAcutance::_submitMtl(SVRendererPtr _render) {
    if((m_BaseOnFlag&MTL_F_BASEON_ACUTANCE)>0){
        _render->submitUniformf("acutance", m_acutance);
    }
    m_BaseOnFlag = MTL_F_BASEON_NONE;
}
