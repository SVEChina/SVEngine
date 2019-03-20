//
//  SVFilterGenLUT.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVFilterGenLUT.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/renderer/SVRendererBase.h"
#include "../../mtl/SVMtlFilterEx.h"
#include "../../rendercore/SVRenderMgr.h"


SVFilterGenLUT::SVFilterGenLUT(SVInst *_app)
:SVFilterBase(_app){
    m_type=SV_FUNC_BRIGHTNESSCONTRAST;
    m_name="SVFilterGenLUT";
    m_BCMtl=nullptr;//brightness contrast
    m_SaturationMtl=nullptr;
    m_colorBalanceMtl=nullptr;
    m_VibranceMtl=nullptr;
    m_hslMtl=nullptr;
    m_shadowHighlightMtl=nullptr;
    m_whiteBalanceMtl=nullptr;
    m_gammaMtl=nullptr;
    m_exposureMtl=nullptr;
    m_GradientMapPass=nullptr;
    //
    m_genParam = MakeSharedPtr<SVGenLUTParam>();
    m_genParam->reset();
}

SVFilterGenLUT::~SVFilterGenLUT(){
    m_genParam = nullptr;
}

bool SVFilterGenLUT::create(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    SVTexturePtr inTex= mApp->getTexMgr()->getTextureSync("svres/lookup.png", true);
    //
    s32 t_w = inTex->getwidth();
    s32 t_h = inTex->getheight();
    t_renderer->createSVTex(E_TEX_FILTER_GENLUT_OUT, t_w, t_h, GL_RGBA);
    t_renderer->createSVTex(E_TEX_FILTER_GENLUT_H1, t_w, t_h, GL_RGBA);
    t_renderer->createSVTex(E_TEX_FILTER_GENLUT_H2, t_w, t_h, GL_RGBA);
    t_renderer->createSVTex(E_TEX_FILTER_GENLUT_H3, 256, 1, GL_RGBA);
    t_renderer->createSVTex(E_TEX_FILTER_GENLUT_H4, 256, 1, GL_RGBA);
    
    
    //增加pass
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->create(t_w,t_h);
    m_pPassNode->setname("SVFilterGenLUT");
    
    //创建材质
    m_BCMtl=MakeSharedPtr<SVMtlBrightnessContrast>(mApp);
    m_BCMtl->setTexcoordFlip(1.0f, 1.0f);
    
    SVMtlCorePtr t_mtl_back=MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    
    m_SaturationMtl=MakeSharedPtr<SVMtlSaturation>(mApp);
    m_SaturationMtl->setTexcoordFlip(1.0f, 1.0f);
    
    m_VibranceMtl=MakeSharedPtr<SVMtlVibrance>(mApp);
    m_VibranceMtl->setTexcoordFlip(1.0f, 1.0f);
    
    m_colorBalanceMtl=MakeSharedPtr<SVMtlColorBalance>(mApp);
    m_colorBalanceMtl->setTexcoordFlip(1.0f, 1.0f);
    
    m_hslMtl=MakeSharedPtr<SVMtlHSL>(mApp);
    m_hslMtl->setTexcoordFlip(1.0, 1.0);
    
    m_shadowHighlightMtl=MakeSharedPtr<SVMtlShadowHighlight>(mApp);
    m_shadowHighlightMtl->setTexcoordFlip(1.0, 1.0);
    
    m_whiteBalanceMtl = MakeSharedPtr<SVMtlWhiteBalance>(mApp);
    m_whiteBalanceMtl->setTexcoordFlip(1.0, 1.0);
    
    m_gammaMtl = MakeSharedPtr<SVMtlGamma>(mApp);
    m_gammaMtl->setTexcoordFlip(1.0, 1.0);
    
    m_exposureMtl = MakeSharedPtr<SVMtlExposure>(mApp);
    m_exposureMtl->setTexcoordFlip(1.0, 1.0);
    
    SVMtlCorePtr t_curveMtl = MakeSharedPtr<SVMtlCore>(mApp,"curveRgba");
    t_curveMtl->setTexcoordFlip(1.0, 1.0);
   
    SVPassPtr m_pass = MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_BCMtl);
    m_pass->setInTex(0,inTex);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H1);
    m_pPassNode->addPass(m_pass);
    
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_SaturationMtl);
    m_pass->setInTex(0,E_TEX_FILTER_GENLUT_H1);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H2);
    m_pPassNode->addPass(m_pass);
   
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_VibranceMtl);
    m_pass->setInTex(0,E_TEX_FILTER_GENLUT_H2);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H1);
    m_pPassNode->addPass(m_pass);
    
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_hslMtl);
    m_pass->setInTex(0, E_TEX_FILTER_GENLUT_H1);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H2);
    m_pPassNode->addPass(m_pass);
    
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_colorBalanceMtl);
    m_pass->setInTex(0, E_TEX_FILTER_GENLUT_H2);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H1);
    m_pPassNode->addPass(m_pass);
    
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_shadowHighlightMtl);
    m_pass->setInTex(0, E_TEX_FILTER_GENLUT_H1);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H2);
    m_pPassNode->addPass(m_pass);
    
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_shadowHighlightMtl);
    m_pass->setInTex(0, E_TEX_FILTER_GENLUT_H1);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H2);
    m_pPassNode->addPass(m_pass);
    
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_whiteBalanceMtl);
    m_pass->setInTex(0, E_TEX_FILTER_GENLUT_H2);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H1);
    m_pPassNode->addPass(m_pass);
    
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_gammaMtl);
    m_pass->setInTex(0, E_TEX_FILTER_GENLUT_H1);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H2);
    m_pPassNode->addPass(m_pass);
    
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(m_exposureMtl);
    m_pass->setInTex(0, E_TEX_FILTER_GENLUT_H2);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H1);
    m_pPassNode->addPass(m_pass);
    
    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(t_curveMtl);
    m_pass->setInTex(0, E_TEX_FILTER_GENLUT_H1);
    m_pass->setInTex(1 ,E_TEX_FILTER_GENLUT_H3);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_H2);
    m_pPassNode->addPass(m_pass);
    
    SVMtlCorePtr t_mtl_rgba=MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl_rgba->setTexcoordFlip(1.0f, 1.0f);
    m_GradientMapPass=MakeSharedPtr<SVPass>();
    m_GradientMapPass->setMtl(t_mtl_rgba);
    m_GradientMapPass->setInTex(0,E_TEX_FILTER_GENLUT_H2);
    m_GradientMapPass->setInTex(1,E_TEX_FILTER_GENLUT_H4);
    m_GradientMapPass->setOutTex(E_TEX_FILTER_GENLUT_H1);
    m_pPassNode->addPass(m_GradientMapPass);

    m_pass=MakeSharedPtr<SVPass>();
    m_pass->setMtl(t_mtl_back);
    m_pass->setInTex(0,E_TEX_FILTER_GENLUT_H1);
    m_pass->setOutTex(E_TEX_FILTER_GENLUT_OUT);
    m_pPassNode->addPass(m_pass);

    return true;
}

void SVFilterGenLUT::destroy(){
    //
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        t_renderer->destroySVTex(E_TEX_FILTER_GENLUT_OUT);
        t_renderer->destroySVTex(E_TEX_FILTER_GENLUT_H1);
        t_renderer->destroySVTex(E_TEX_FILTER_GENLUT_H2);
        t_renderer->destroySVTex(E_TEX_FILTER_GENLUT_H3);
        t_renderer->destroySVTex(E_TEX_FILTER_GENLUT_H4);
    }
    //
    m_BCMtl=nullptr;//brightness contrast
    m_SaturationMtl=nullptr;
    m_colorBalanceMtl=nullptr;
    m_VibranceMtl=nullptr;
    m_hslMtl=nullptr;
    m_whiteBalanceMtl=nullptr;
    m_gammaMtl=nullptr;
    m_exposureMtl=nullptr;
    m_GradientMapPass=nullptr;
    //
    m_genParam->reset();
}

SVTexturePtr SVFilterGenLUT::getOutTex(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        return t_renderer->getSVTex(E_TEX_FILTER_GENLUT_OUT);
    }
    return nullptr;
}

void SVFilterGenLUT::setCurveRgba(ptr8  data,u32 size){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_FILTER_GENLUT_H3);
        t_tex->setTexData(data, size);
    }
}

void SVFilterGenLUT::setGradientMap(ptr8  data,u32 size){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_FILTER_GENLUT_H4);
        t_tex->setTexData(data, size);
    }
}

void SVFilterGenLUT::update(f32 dt){
    m_BCMtl->setBrightness(m_genParam->m_brightness);
    m_BCMtl->setContrast(m_genParam->m_contrast);
    m_SaturationMtl->setSaturation(m_genParam->m_saturation);
    m_VibranceMtl->setVibrance(m_genParam->m_vibrance);
    
    m_hslMtl->setSaturationRed(m_genParam->m_HSLSaturationRed);
    m_hslMtl->setHueRed(m_genParam->m_HSLHueRed);
    m_hslMtl->setLightnessRed(m_genParam->m_HSLLightnessRed);
    
    m_hslMtl->setSaturationYellow(m_genParam->m_HSLSaturationYellow);
    m_hslMtl->setHueYellow(m_genParam->m_HSLHueYellow);
    m_hslMtl->setLightnessYellow(m_genParam->m_HSLLightnessYellow);
    
    m_hslMtl->setSaturationGreen(m_genParam->m_HSLSaturationGreen);
    m_hslMtl->setHueGreen(m_genParam->m_HSLHueGreen);
    m_hslMtl->setLightnessGreen(m_genParam->m_HSLLightnessGreen);
    
    m_hslMtl->setSaturationMagenta(m_genParam->m_HSLSaturationMagenta);
    m_hslMtl->setHueMagenta(m_genParam->m_HSLHueMagenta);
    m_hslMtl->setLightnessMagenta(m_genParam->m_HSLLightnessMagenta);
    
    m_hslMtl->setSaturationBlue(m_genParam->m_HSLSaturationBlue);
    m_hslMtl->setHueBlue(m_genParam->m_HSLHueBlue);
    m_hslMtl->setLightnessBlue(m_genParam->m_HSLLightnessBlue);
    
    m_hslMtl->setSaturationCyan(m_genParam->m_HSLSaturationCyan);
    m_hslMtl->setHueCyan(m_genParam->m_HSLHueCyan);
    m_hslMtl->setLightnessCyan(m_genParam->m_HSLLightnessCyan);
    
    m_colorBalanceMtl->setRedShift(m_genParam->m_redShift);
    m_colorBalanceMtl->setGreenShift(m_genParam->m_greenShift);
    m_colorBalanceMtl->setBlueShift(m_genParam->m_blueShift);
    m_colorBalanceMtl->setSDRedShift(m_genParam->m_sdredShift);
    m_colorBalanceMtl->setSDGreenShift(m_genParam->m_sdgreenShift);
    m_colorBalanceMtl->setSDBlueShift(m_genParam->m_sdblueShift);
    m_colorBalanceMtl->setHHRedShift(m_genParam->m_hhredShift);
    m_colorBalanceMtl->setHHGreenShift(m_genParam->m_hhgreenShift);
    m_colorBalanceMtl->setHHBlueShift(m_genParam->m_hhblueShift);
    
    m_shadowHighlightMtl->setShadow(m_genParam->m_shadow);
    m_shadowHighlightMtl->setHighlight(m_genParam->m_Highlight);
    
    m_whiteBalanceMtl->setTemperature(m_genParam->m_temperature);
    m_whiteBalanceMtl->setTint(m_genParam->m_tint);
    
    m_gammaMtl->setGamma(m_genParam->m_gamma);
    
    m_exposureMtl->setExposure(m_genParam->m_exposure);
}

void SVFilterGenLUT::openGradientMap(){
    SVMtlCorePtr t_mtl=MakeSharedPtr<SVMtlCore>(mApp,"GradientMap");
    t_mtl->setTexcoordFlip(1.0f, 1.0f);
    m_GradientMapPass->setMtl(t_mtl);
    m_GradientMapPass->setInTex(0,E_TEX_FILTER_GENLUT_H2);
    m_GradientMapPass->setInTex(1,E_TEX_FILTER_GENLUT_H4);
}

void SVFilterGenLUT::closeGradientMap(){
    SVMtlCorePtr t_mtl=MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl->setTexcoordFlip(1.0f, 1.0f);
    m_GradientMapPass->setMtl(t_mtl);
    m_GradientMapPass->setInTex(0,E_TEX_FILTER_GENLUT_H2);
    m_GradientMapPass->setInTex(1,E_TEX_FILTER_GENLUT_H4);
}

void SVFilterGenLUT::refreshFData(SVGenLUTParamPtr _param) {
    if(_param && m_genParam) {
        m_genParam->copy(_param);
    }
}
void SVFilterGenLUT::setLUTData(ptr8  data,u32 size){
    m_genParam->m_data.clear();
    m_genParam->m_data.allocate(size);
    m_genParam->m_data.append(data,size);
}

void SVFilterGenLUT::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                         RAPIDJSON_NAMESPACE::Value &_objValue) {
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);
    locationObj.AddMember("saturation",m_genParam->m_saturation, _allocator);
    locationObj.AddMember("brightness",m_genParam->m_brightness, _allocator);
    locationObj.AddMember("contrast",m_genParam->m_contrast, _allocator);
    locationObj.AddMember("vibrance",m_genParam->m_vibrance, _allocator);
    locationObj.AddMember("HSLSaturationRed",m_genParam->m_HSLSaturationRed, _allocator);
    locationObj.AddMember("HSLLightnessRed",m_genParam->m_HSLLightnessRed, _allocator);
    locationObj.AddMember("HSLHueRed",m_genParam->m_HSLHueRed, _allocator);
    locationObj.AddMember("HSLSaturationYellow",m_genParam->m_HSLSaturationYellow, _allocator);
    locationObj.AddMember("HSLLightnessYellow",m_genParam->m_HSLLightnessYellow, _allocator);
    locationObj.AddMember("HSLHueYellow",m_genParam->m_HSLHueYellow, _allocator);
    locationObj.AddMember("HSLSaturationGreen",m_genParam->m_HSLSaturationGreen, _allocator);
    locationObj.AddMember("HSLLightnessGreen",m_genParam->m_HSLLightnessGreen, _allocator);
    locationObj.AddMember("HSLHueGreen",m_genParam->m_HSLHueGreen, _allocator);
    locationObj.AddMember("HSLSaturationMagenta",m_genParam->m_HSLSaturationMagenta, _allocator);
    locationObj.AddMember("HSLLightnessMagenta",m_genParam->m_HSLLightnessMagenta, _allocator);
    locationObj.AddMember("HSLHueMagenta",m_genParam->m_HSLHueMagenta, _allocator);
    locationObj.AddMember("HSLSaturationBlue",m_genParam->m_HSLSaturationBlue, _allocator);
    locationObj.AddMember("HSLLightnessBlue",m_genParam->m_HSLLightnessBlue, _allocator);
    locationObj.AddMember("HSLHueBlue",m_genParam->m_HSLHueBlue, _allocator);
    locationObj.AddMember("HSLSaturationCyan",m_genParam->m_HSLSaturationCyan, _allocator);
    locationObj.AddMember("HSLLightnessCyan",m_genParam->m_HSLLightnessCyan, _allocator);
    locationObj.AddMember("HSLHueCyan",m_genParam->m_HSLHueCyan, _allocator);
    locationObj.AddMember("redShift",m_genParam->m_redShift, _allocator);
    locationObj.AddMember("greenShift",m_genParam->m_greenShift, _allocator);
    locationObj.AddMember("blueShift",m_genParam->m_blueShift, _allocator);
    locationObj.AddMember("sdredShift",m_genParam->m_sdredShift, _allocator);
    locationObj.AddMember("sdgreenShift",m_genParam->m_sdgreenShift, _allocator);
    locationObj.AddMember("sdblueShift",m_genParam->m_sdblueShift, _allocator);
    locationObj.AddMember("hhredShift",m_genParam->m_hhredShift, _allocator);
    locationObj.AddMember("hhgreenShift",m_genParam->m_hhgreenShift, _allocator);
    locationObj.AddMember("hhblueShift",m_genParam->m_hhblueShift, _allocator);
    locationObj.AddMember("shadow",m_genParam->m_shadow, _allocator);
    locationObj.AddMember("Highlight",m_genParam->m_Highlight, _allocator);
    locationObj.AddMember("gamma",m_genParam->m_gamma, _allocator);
    locationObj.AddMember("temperature",m_genParam->m_temperature, _allocator);
    locationObj.AddMember("tint",m_genParam->m_tint, _allocator);
    locationObj.AddMember("exposure",m_genParam->m_exposure, _allocator);
    locationObj.AddMember("name",RAPIDJSON_NAMESPACE::StringRef(m_genParam->m_name.c_str()), _allocator);
    locationObj.AddMember("data",RAPIDJSON_NAMESPACE::StringRef(m_genParam->m_data.c_str(),m_genParam->m_data.size()), _allocator);
    _objValue.AddMember("filterLUT", locationObj, _allocator);  //添加object到Document中
 
}

void SVFilterGenLUT::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {

}
