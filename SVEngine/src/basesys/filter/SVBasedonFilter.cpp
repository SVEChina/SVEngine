//
// SVBasedonFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBasedonFilter.h"
#include "../../core/SVPass.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../node/SVMultPassNode.h"
#include "../../mtl/SVMtlBasedOn.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../rendercore/SVRenderMgr.h"

SVBasedonFilter::SVBasedonFilter(SVInst *_app)
:SVFilterBase(_app){
    m_baseOnmtl = nullptr;
    m_acutancemtl = nullptr;
    m_shadows=0.0;
    m_highlights=0.0;
    m_contrast=0.0;
    m_saturation=0.0;
    m_acutance=0.0;
    m_brightness=0.0;
    m_whitening=0.0;
    m_gamma=0.0;
    m_redShift=0.0;
    m_greenShift=0.0;
    m_blueShift=0.0;
    m_sdredShift=0.0;
    m_sdgreenShift=0.0;
    m_sdblueShift=0.0;
    m_hhredShift=0.0;
    m_hhgreenShift=0.0;
    m_hhblueShift=0.0;
    m_temperature=0.0;
    m_tint=0.0;
}

SVBasedonFilter::~SVBasedonFilter(){
    destroy();
}

//创建
bool SVBasedonFilter::create() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    s32 t_w =  mApp->m_pGlobalParam->m_inner_width;
    s32 t_h =  mApp->m_pGlobalParam->m_inner_height;
    SVTexturePtr tex = t_renderer->getSVTex(E_TEX_HELP0);
    if(!tex){
        tex = t_renderer->createSVTex(E_TEX_HELP0,t_w, t_h, GL_RGBA);
    }
    //创建材质
    m_acutancemtl=MakeSharedPtr<SVMtlAcutance>(mApp);
    m_acutancemtl->setTexcoordFlip(1.0f, 1.0f);
    m_acutancemtl->setTexSizeIndex(0,0.5/t_w,0.5/ t_h);
    m_acutancemtl->setTexSizeIndex(1,0.5/255.0,0.0);
    
    m_baseOnmtl=MakeSharedPtr<SVMtlBasedOn>(mApp);
    m_baseOnmtl->setTexcoordFlip(1.0f, 1.0f);
    m_baseOnmtl->setTexSizeIndex(0,0.5/t_w,0.5/ t_h);
    m_baseOnmtl->setTexSizeIndex(1,0.5/255.0,0.0);
    //创建多passnode
    m_pPassNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pPassNode->setname("SVfilterBaseOn");
    m_pPassNode->create(t_w, t_h);
    m_pPassNode->setRSType(RST_IMGFILTER);
    //创建pass
    SVPassPtr t_pass1 = MakeSharedPtr<SVPass>();
    t_pass1->setMtl(m_baseOnmtl);
    t_pass1->setInTex(0,E_TEX_MAIN);
    SVTexturePtr teximg = mApp->getTexMgr()->getTextureSync("svres/filterimg/facewhitefilter.png", true);
    t_pass1->setInTex(1,teximg);
    t_pass1->setOutTex(E_TEX_HELP0);
    t_pass1->mTag = "filter-baseon";
    m_pPassNode->addPass(t_pass1);
    //
    SVPassPtr t_pass2 = MakeSharedPtr<SVPass>();
    t_pass2->setMtl(m_acutancemtl);
    t_pass2->setInTex(0, E_TEX_HELP0);
    t_pass2->setOutTex(E_TEX_MAIN);
    t_pass2->mTag = "write-back";
    m_pPassNode->addPass(t_pass2);
    return true;
}


void SVBasedonFilter::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
    if (m_baseOnmtl) {
        if(_type==E_SHADOWS_FILTER){
            setShadows(-_smooth/100.0);
        }else if(_type==E_CONTRAST_FILTER){
            setContrast(_smooth/100.0);
        }else if(_type==E_SATURATION_FILTER){
            setSaturation(_smooth/100.0);
        }else if(_type==E_BRIGHTNESS_FILTER){
            setBrightness(_smooth/100.0);
        }else if(_type==E_WHITENING_FILTER){
            setWhitening(_smooth/100.0);
        }else if(_type==E_HIGHLIGHTS_FILTER){
            setHighlights(_smooth/100.0);
        }else if(_type==E_GAMMA_FILTER){
            if(_smooth==-100){
                setGamma(-1.0);
            }else{
                setGamma((_smooth+100.0)/100.0);
            }
        }else if(_type==E_REDSHIFT_FILTER){
            setRedShift(_smooth);
        }else if(_type==E_GREENSHIFT_FILTER){
            setGreenShift(_smooth);
        }else if(_type==E_BLUESHIFT_FILTER){
            setBlueShift(_smooth);
        }else if(_type==E_SDREDSHIFT_FILTER){
            setSDRedShift(_smooth);
        }else if(_type==E_SDGREENSHIFT_FILTER){
            setSDGreenShift(_smooth);
        }else if(_type==E_SDBLUESHIFT_FILTER){
            setSDBlueShift(_smooth);
        }else if(_type==E_HHREDSHIFT_FILTER){
            setHHRedShift(_smooth);
        }else if(_type==E_HHGREENSHIFT_FILTER){
            setHHGreenShift(_smooth);
        }else if(_type==E_HHBLUESHIFT_FILTER){
            setHHBlueShift(_smooth);
        }else if(_type==E_TEMPERATURE_FILTER){
            float _temperature=(_smooth+100)/100.0*5000;
            _temperature=_temperature < 5000 ? 0.0004 * (_temperature-5000.0) : 0.00006 * (_temperature-5000.0);
            setTemperature(_temperature);
        }else if(_type==E_TINT_FILTER){
            setTint(_smooth);
        }
    }
    if(m_acutancemtl){
        if(_type==E_ACUTANCE_FILTER){
            setAcutance(_smooth/100.0);
        }
    }
}

f32 SVBasedonFilter::getFilterParam(SVFILTERITEMTYPE _type){
        if(_type==E_SHADOWS_FILTER){
            return getShadows();
        }else if(_type==E_CONTRAST_FILTER){
            return getContrast();
        }else if(_type==E_SATURATION_FILTER){
            return getSaturation();
        }else if(_type==E_BRIGHTNESS_FILTER){
            return getBrightness();
        }else if(_type==E_WHITENING_FILTER){
            return getWhitening();
        }else if(_type==E_HIGHLIGHTS_FILTER){
            return getHighlights();
        }else if(_type==E_GAMMA_FILTER){
            return getGamma();
        }else if(_type==E_REDSHIFT_FILTER){
            return getRedShift();
        }else if(_type==E_GREENSHIFT_FILTER){
            return getGreenShift();
        }else if(_type==E_BLUESHIFT_FILTER){
            return getBlueShift();
        }else if(_type==E_SDREDSHIFT_FILTER){
            return getSDRedShift();
        }else if(_type==E_SDGREENSHIFT_FILTER){
            return getSDGreenShift();
        }else if(_type==E_SDBLUESHIFT_FILTER){
            return getSDBlueShift();
        }else if(_type==E_HHREDSHIFT_FILTER){
            return getHHRedShift();
        }else if(_type==E_HHGREENSHIFT_FILTER){
            return getHHGreenShift();
        }else if(_type==E_HHBLUESHIFT_FILTER){
            return getHHBlueShift();
        }else if(_type==E_TEMPERATURE_FILTER){
            return getTint();
        }else if(_type==E_TINT_FILTER){
            return getTint();
        }else if(_type==E_ACUTANCE_FILTER){
            return getAcutance();
        }
        return 0;
}

//销毁
void SVBasedonFilter::destroy() {
    if(m_pPassNode) {
        m_pPassNode->removeFromParent();
        m_pPassNode=nullptr;
    }
    m_acutancemtl = nullptr;
    m_baseOnmtl = nullptr;
}

void SVBasedonFilter::update(f32 dt){
    if( m_baseOnmtl ){
        m_baseOnmtl->setTexcoordFlip(1.0, 1.0);
        m_baseOnmtl->setShadows(m_shadows);
        m_baseOnmtl->setHighlights(m_highlights);
        m_baseOnmtl->setContrast(m_contrast);
        m_baseOnmtl->setSaturation(m_saturation);
        m_baseOnmtl->setBrightness(m_brightness);
        m_baseOnmtl->setWhitening(m_whitening);
        m_baseOnmtl->setGamma(m_gamma);
        m_baseOnmtl->setRedShift(m_redShift);
        m_baseOnmtl->setGreenShift(m_greenShift);
        m_baseOnmtl->setBlueShift(m_blueShift);
        m_baseOnmtl->setSDRedShift(m_sdredShift);
        m_baseOnmtl->setSDGreenShift(m_sdgreenShift);
        m_baseOnmtl->setSDBlueShift(m_sdblueShift);
        m_baseOnmtl->setHHRedShift(m_hhredShift);
        m_baseOnmtl->setHHGreenShift(m_hhgreenShift);
        m_baseOnmtl->setHHBlueShift(m_hhblueShift);
        m_baseOnmtl->setHHGreenShift(m_hhgreenShift);
        m_baseOnmtl->setHHBlueShift(m_hhblueShift);
        m_baseOnmtl->setTint(m_tint);
        m_baseOnmtl->setTemperature(m_temperature);
    }
    if(m_acutancemtl){
        m_acutancemtl->setAcutance(m_acutance);
    }
}
