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


SVMtlColorBalance::SVMtlColorBalance(SVInst *_app)
:SVMtlCore(_app,"vibrance"){
    
}
    
SVMtlColorBalance::SVMtlColorBalance(SVMtlColorBalance* _mtl)
:SVMtlCore(_mtl){
    
}
    
SVMtlColorBalance::~SVMtlColorBalance(){
    
}
    
SVMtlCorePtr SVMtlColorBalance::clone(){
     return PointerSharedPtr<SVMtlColorBalance>(new SVMtlColorBalance(this));
}

void SVMtlColorBalance::_submitMtl(SVRendererBasePtr _render){
    
}


SVMtlHSL::SVMtlHSL(SVInst *_app)
:SVMtlCore(_app,"HSL2"){
     m_saturationRed=1.0;
     m_HueRed=0.0;
     m_LightnessRed=1.0;
    
     m_saturationYellow=1.0;
     m_HueYellow=0.0;
     m_LightnessYellow=1.0;
    
     m_saturationGreen=1.0;
     m_HueGreen=0.0;
     m_LightnessGreen=1.0;
    
     m_saturationMagenta=1.0;
     m_HueMagenta=0.0;
     m_LightnessMagenta=1.0;
    
     m_saturationBlue=1.0;
     m_HueBlue=0.0;
     m_LightnessBlue=1.0;
    
     m_saturationCyan=1.0;
     m_HueCyan=0.0;
     m_LightnessCyan=1.0;
}

SVMtlHSL::SVMtlHSL(SVMtlHSL* _mtl)
:SVMtlCore(_mtl){
}

SVMtlHSL::~SVMtlHSL(){
}

SVMtlCorePtr SVMtlHSL::clone(){
    return PointerSharedPtr<SVMtlHSL>(new SVMtlHSL(this));
}

void SVMtlHSL::_submitMtl(SVRendererBasePtr _render){
    SVMtlCore::_submitMtl(_render);
    
    _render->submitUniformf("m_saturationRed", m_saturationRed);
    _render->submitUniformf("m_HueRed", m_HueRed);
    _render->submitUniformf("m_LightnessRed", m_LightnessRed);
    
    _render->submitUniformf("m_saturationYellow", m_saturationYellow);
    _render->submitUniformf("m_HueYellow", m_HueYellow);
    _render->submitUniformf("m_LightnessYellow", m_LightnessYellow);
    
    _render->submitUniformf("m_saturationGreen", m_saturationGreen);
    _render->submitUniformf("m_HueGreen", m_HueGreen);
    _render->submitUniformf("m_LightnessGreen", m_LightnessGreen);
    
    _render->submitUniformf("m_saturationMagenta", m_saturationMagenta);
    _render->submitUniformf("m_HueMagenta", m_HueMagenta);
    _render->submitUniformf("m_LightnessMagenta", m_LightnessMagenta);
    
    _render->submitUniformf("m_saturationBlue", m_saturationBlue);
    _render->submitUniformf("m_HueBlue", m_HueBlue);
    _render->submitUniformf("m_LightnessBlue", m_LightnessBlue);
    
    _render->submitUniformf("m_saturationCyan", m_saturationCyan);
    _render->submitUniformf("m_HueCyan", m_HueCyan);
    _render->submitUniformf("m_LightnessCyan", m_LightnessCyan);

}
