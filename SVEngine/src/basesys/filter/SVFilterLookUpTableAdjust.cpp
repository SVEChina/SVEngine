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
    m_passVibrance=nullptr;
    m_passBC=nullptr;
    m_passBack=nullptr;
    outTex=nullptr;
    inTex=nullptr;
    m_saturation=0.0f;
    m_brightness=0.0f;
    m_contrast=0.0f;
    m_vibrance=0.0f;
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
    
    m_passBack=MakeSharedPtr<SVPass>();
    m_passBack->setMtl(t_mtl_back);
    m_passBack->setInTex(0,tex01);
    m_passBack->setOutTex(outTex);
    
    m_pPassNode->addPass(m_passBC);
    m_pPassNode->addPass(m_passSaturation);
     m_pPassNode->addPass(m_passVibrance);
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
}

void SVFilterLookUpTableAdjust::update(f32 dt){
    m_BCMtl->setBrightness(m_brightness);
    m_BCMtl->setContrast(m_contrast);
    m_SaturationMtl->setSaturation(m_saturation);
    m_VibranceMtl->setVibrance(m_vibrance);
    
}
