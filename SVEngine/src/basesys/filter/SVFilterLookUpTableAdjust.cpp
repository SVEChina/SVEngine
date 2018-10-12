//
//  SVFilterLookUpTableAdjust.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/9/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVFilterLookUpTableAdjust.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/renderer/SVRendererBase.h"
#include "../../mtl/SVMtlFilterEx.h"
#include "../../rendercore/SVRenderMgr.h"


SVFilterLookUpTableAdjust::SVFilterLookUpTableAdjust(SVInst *_app)
:SVFilterBase(_app){
    m_type=SV_FUNC_BRIGHTNESSCONTRAST;
    m_name="SVFilterLookUpTableAdjust";
    m_BCMtl=nullptr;//brightness contrast
    m_SaturationMtl=nullptr;
    m_passSaturation=nullptr;
    m_VibranceMtl=nullptr;
    m_hslMtl=nullptr;
    m_passVibrance=nullptr;
    m_passBC=nullptr;
    m_passBack=nullptr;
    outTex=nullptr;
    inTex=nullptr;
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
}

SVFilterLookUpTableAdjust::~SVFilterLookUpTableAdjust(){
    
}

bool SVFilterLookUpTableAdjust::create(){
    SVRendererBasePtr t_renderer = mApp->getRenderMgr()->getRenderer();
    if(!t_renderer)
        return false;
   
    inTex= mApp->getTexMgr()->getTextureSync("svres/lookup.png", true);
    outTex=mApp->getTexMgr()->createUnctrlTexture(inTex->getwidth(), inTex->getheight(), GL_RGBA,  GL_RGBA);
    SVTexturePtr tex01=mApp->getTexMgr()->createUnctrlTexture(inTex->getwidth(), inTex->getheight(), GL_RGBA,  GL_RGBA);
    SVTexturePtr tex02=mApp->getTexMgr()->createUnctrlTexture(inTex->getwidth(), inTex->getheight(), GL_RGBA,  GL_RGBA);
    //增加pass
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->create(inTex->getwidth(), inTex->getheight());
    m_pPassNode->setname("SVFilterLookUpTableAdjust");
    
    //创建材质
    m_BCMtl=MakeSharedPtr<SVMtlBrightnessContrast>(mApp);
    m_BCMtl->setTexcoordFlip(1.0f, 1.0f);
    
    SVMtlCorePtr t_mtl_back=MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    t_mtl_back->setTexcoordFlip(1.0f, 1.0f);
    
    m_SaturationMtl=MakeSharedPtr<SVMtlSaturation>(mApp);
    m_SaturationMtl->setTexcoordFlip(1.0f, 1.0f);
    
    m_VibranceMtl=MakeSharedPtr<SVMtlVibrance>(mApp);
    m_VibranceMtl->setTexcoordFlip(1.0f, 1.0f);
    
    m_hslMtl=MakeSharedPtr<SVMtlHSL>(mApp);
    m_hslMtl->setTexcoordFlip(1.0, 1.0);
   
    m_passBC    = MakeSharedPtr<SVPass>();
    m_passBC->setMtl(m_BCMtl);
    m_passBC->setInTex(0,inTex);
    m_passBC->setOutTex(tex01);
    
    m_passSaturation=MakeSharedPtr<SVPass>();
    m_passSaturation->setMtl(m_SaturationMtl);
    m_passSaturation->setInTex(0,tex01);
    m_passSaturation->setOutTex(tex02);

    m_passVibrance=MakeSharedPtr<SVPass>();
    m_passVibrance->setMtl(m_VibranceMtl);
    m_passVibrance->setInTex(0,tex02);
    m_passVibrance->setOutTex(tex01);
    
    m_passHSL=MakeSharedPtr<SVPass>();
    m_passHSL->setMtl(m_hslMtl);
    m_passHSL->setInTex(0, tex01);
    m_passHSL->setOutTex(tex02);
    
    m_passBack=MakeSharedPtr<SVPass>();
    m_passBack->setMtl(t_mtl_back);
    m_passBack->setInTex(0,tex02);
    m_passBack->setOutTex(outTex);
    
    m_pPassNode->addPass(m_passBC);
    m_pPassNode->addPass(m_passSaturation);
    m_pPassNode->addPass(m_passVibrance);
    m_pPassNode->addPass(m_passHSL);
    m_pPassNode->addPass(m_passBack);
    
    return true;
}

void SVFilterLookUpTableAdjust::destroy(){
    m_BCMtl=nullptr;//brightness contrast
    m_SaturationMtl=nullptr;
    m_passSaturation=nullptr;
    m_passBC=nullptr;
    m_passBack=nullptr;
    outTex=nullptr;
    inTex=nullptr;
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
}

void SVFilterLookUpTableAdjust::update(f32 dt){
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
    
}
