//
//  SVBrightnessMtl.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVMtlFilterEx.h"
#include "../mtl/SVTexMgr.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlBrightnessContrast::SVMtlBrightnessContrast(SVInst *_app)
:SVMtlCore(_app,"BrightnessContrast") {
    m_brightness=0.0f;
    m_contrast=0.0f;
}

SVMtlBrightnessContrast::SVMtlBrightnessContrast(SVMtlBrightnessContrast* _mtl)
:SVMtlCore(_mtl){
    m_brightness=_mtl->m_brightness;
    m_contrast=_mtl->m_contrast;
}

SVMtlBrightnessContrast::~SVMtlBrightnessContrast(){
    
}

SVMtlCorePtr SVMtlBrightnessContrast::clone(){
    return PointerSharedPtr<SVMtlBrightnessContrast>(new SVMtlBrightnessContrast(this));
}

void SVMtlBrightnessContrast::setBrightness(f32 _smooth){
    m_brightness=_smooth;
}

void SVMtlBrightnessContrast::setContrast(f32 _smooth){
    m_contrast=_smooth;
}

void SVMtlBrightnessContrast::_submitMtl(SVRendererBasePtr _render) {
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf("brightness", m_brightness);
    _render->submitUniformf("contrast", m_contrast);
}



SVMtlSaturation::SVMtlSaturation(SVInst *_app)
:SVMtlCore(_app,"saturation") {
    m_saturation=0.0f;
}

SVMtlSaturation::SVMtlSaturation(SVMtlSaturation* _mtl)
:SVMtlCore(_mtl){
    m_saturation=_mtl->m_saturation;
}

SVMtlSaturation::~SVMtlSaturation(){
    
}

SVMtlCorePtr SVMtlSaturation::clone(){
    return PointerSharedPtr<SVMtlSaturation>(new SVMtlSaturation(this));
}

void SVMtlSaturation::setSaturation(f32 _smooth){
    m_saturation=_smooth;
}


void SVMtlSaturation::_submitMtl(SVRendererBasePtr _render) {
    SVMtlCore::_submitMtl(_render);
       _render->submitUniformf("saturation", m_saturation);
}

SVMtlVibrance::SVMtlVibrance(SVInst *_app)
:SVMtlCore(_app,"vibrance") {
    m_vibrance=0.0f;
}

SVMtlVibrance::SVMtlVibrance(SVMtlVibrance* _mtl)
:SVMtlCore(_mtl){
    m_vibrance=_mtl->m_vibrance;
}

SVMtlVibrance::~SVMtlVibrance(){
    
}

SVMtlCorePtr SVMtlVibrance::clone(){
    return PointerSharedPtr<SVMtlVibrance>(new SVMtlVibrance(this));
}

void SVMtlVibrance::setVibrance(f32 _smooth){
    m_vibrance=_smooth;
}


void SVMtlVibrance::_submitMtl(SVRendererBasePtr _render) {
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf("vibrance", m_vibrance);
}

SVMtlHSL::SVMtlHSL(SVInst *_app)
:SVMtlCore(_app,"HSL2"){
     m_saturation=0.0f;
     m_Hue=0.0f;
     m_Lightness=0.0f;
     m_HueRadius=0.0f;
}

SVMtlHSL::SVMtlHSL(SVMtlHSL* _mtl)
:SVMtlCore(_mtl){
}

SVMtlHSL::~SVMtlHSL(){
}

SVMtlCorePtr SVMtlHSL::clone(){
    return PointerSharedPtr<SVMtlHSL>(new SVMtlHSL(this));
}

void SVMtlHSL::setSaturation(f32 _smooth){
    m_saturation=_smooth;
}

void SVMtlHSL::setHue(f32 _smooth){
    m_Hue=_smooth;
}

void SVMtlHSL::setLightness(f32 _smooth){
    m_Lightness=_smooth;
}

void SVMtlHSL::setHueRadius(f32 _smooth){
    m_HueRadius=_smooth;
}

void SVMtlHSL::_submitMtl(SVRendererBasePtr _render){
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf("m_Hue", m_Hue);
    _render->submitUniformf("m_Saturation", m_saturation);
    _render->submitUniformf("m_Lightness", m_Lightness);
    _render->submitUniformf("m_HueRadius", m_HueRadius);
}
