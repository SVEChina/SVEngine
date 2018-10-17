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

    m_saturation=0.0f;
    m_brightness=0.0f;
    m_contrast=0.0f;
    m_vibrance=0.0f;
    
    m_HSLSaturationRed=1.0f;
    m_HSLLightnessRed=1.0f;
    m_HSLHueRed=0.0f;
    
    m_HSLSaturationYellow=1.0f;
    m_HSLLightnessYellow=1.0f;
    m_HSLHueYellow=0.0f;
    
    m_HSLSaturationGreen=1.0f;
    m_HSLLightnessGreen=1.0f;
    m_HSLHueGreen=0.0f;
    
    m_HSLSaturationMagenta=1.0f;
    m_HSLLightnessMagenta=1.0f;
    m_HSLHueMagenta=0.0f;
    
    m_HSLSaturationBlue=1.0f;
    m_HSLLightnessBlue=1.0f;
    m_HSLHueBlue=0.0f;
    
    m_HSLSaturationCyan=1.0f;
    m_HSLLightnessCyan=1.0f;
    m_HSLHueCyan=0.0f;
    
    m_redShift=0.0f;
    m_greenShift=0.0f;
    m_blueShift=0.0f;
    m_sdredShift=0.0f;
    m_sdgreenShift=0.0f;
    m_sdblueShift=0.0f;
    m_hhredShift=0.0f;
    m_hhgreenShift=0.0f;
    m_hhblueShift=0.0f;
}

SVFilterGenLUT::~SVFilterGenLUT(){
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
    m_pass->setMtl(t_mtl_back);
    m_pass->setInTex(0,E_TEX_FILTER_GENLUT_H2);
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
    }
    //
    m_BCMtl=nullptr;//brightness contrast
    m_SaturationMtl=nullptr;
    m_colorBalanceMtl=nullptr;
    m_VibranceMtl=nullptr;
    m_hslMtl=nullptr;

    m_saturation=0.0f;
    m_brightness=0.0f;
    m_contrast=0.0f;
    m_vibrance=0.0f;
    
    m_HSLSaturationRed=0.0f;
    m_HSLLightnessRed=0.0f;
    m_HSLHueRed=0.0f;

    m_HSLSaturationYellow=0.0f;
    m_HSLLightnessYellow=0.0f;
    m_HSLHueYellow=0.0f;

    m_HSLSaturationGreen=0.0f;
    m_HSLLightnessGreen=0.0f;
    m_HSLHueGreen=0.0f;

    m_HSLSaturationMagenta=0.0f;
    m_HSLLightnessMagenta=0.0f;
    m_HSLHueMagenta=0.0f;

    m_HSLSaturationBlue=0.0f;
    m_HSLLightnessBlue=0.0f;
    m_HSLHueBlue=0.0f;

    m_HSLSaturationCyan=0.0f;
    m_HSLLightnessCyan=0.0f;
    m_HSLHueCyan=0.0f;
    
    m_redShift=0.0f;
    m_greenShift=0.0f;
    m_blueShift=0.0f;
    m_sdredShift=0.0f;
    m_sdgreenShift=0.0f;
    m_sdblueShift=0.0f;
    m_hhredShift=0.0f;
    m_hhgreenShift=0.0f;
    m_hhblueShift=0.0f;
    
    m_shadow=0.0;
    m_Highlight=0.0;
}

SVTexturePtr SVFilterGenLUT::getOutTex(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer) {
        return t_renderer->getSVTex(E_TEX_FILTER_GENLUT_OUT);
    }
    return nullptr;
}

void SVFilterGenLUT::update(f32 dt){
    m_BCMtl->setBrightness(m_brightness);
    m_BCMtl->setContrast(m_contrast);
    m_SaturationMtl->setSaturation(m_saturation);
    m_VibranceMtl->setVibrance(m_vibrance);
    
    m_hslMtl->setSaturationRed(m_HSLSaturationRed);
    m_hslMtl->setHueRed(m_HSLHueRed);
    m_hslMtl->setLightnessRed(m_HSLLightnessRed);
    
    m_hslMtl->setSaturationYellow(m_HSLSaturationYellow);
    m_hslMtl->setHueYellow(m_HSLHueYellow);
    m_hslMtl->setLightnessYellow(m_HSLLightnessYellow);
    
    m_hslMtl->setSaturationGreen(m_HSLSaturationGreen);
    m_hslMtl->setHueGreen(m_HSLHueGreen);
    m_hslMtl->setLightnessGreen(m_HSLLightnessGreen);
    
    m_hslMtl->setSaturationMagenta(m_HSLSaturationMagenta);
    m_hslMtl->setHueMagenta(m_HSLHueMagenta);
    m_hslMtl->setLightnessMagenta(m_HSLLightnessMagenta);
    
    m_hslMtl->setSaturationBlue(m_HSLSaturationBlue);
    m_hslMtl->setHueBlue(m_HSLHueBlue);
    m_hslMtl->setLightnessBlue(m_HSLLightnessBlue);
    
    m_hslMtl->setSaturationCyan(m_HSLSaturationCyan);
    m_hslMtl->setHueCyan(m_HSLHueCyan);
    m_hslMtl->setLightnessCyan(m_HSLLightnessCyan);
    
    m_colorBalanceMtl->setRedShift(m_redShift);
    m_colorBalanceMtl->setGreenShift(m_greenShift);
    m_colorBalanceMtl->setBlueShift(m_blueShift);
    m_colorBalanceMtl->setSDRedShift(m_sdredShift);
    m_colorBalanceMtl->setSDGreenShift(m_sdgreenShift);
    m_colorBalanceMtl->setSDBlueShift(m_sdblueShift);
    m_colorBalanceMtl->setHHRedShift(m_hhredShift);
    m_colorBalanceMtl->setHHGreenShift(m_hhgreenShift);
    m_colorBalanceMtl->setHHBlueShift(m_hhblueShift);
    
    m_shadowHighlightMtl->setShadow(m_shadow);
    m_shadowHighlightMtl->setHighlight(m_Highlight);
}

void SVFilterGenLUT::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                         RAPIDJSON_NAMESPACE::Value &_objValue) {
 
}

void SVFilterGenLUT::fromJSON(RAPIDJSON_NAMESPACE::Value &item) {

}
