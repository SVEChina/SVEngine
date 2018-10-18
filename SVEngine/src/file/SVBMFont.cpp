//
// SVBMFont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBMFont.h"
#include "../core/SVBMFontConf.h"
#include "../base/SVPreDeclare.h"
SVBMFont::SVBMFont(SVInst *_app)
:SVFont(_app) {

}

SVBMFont::~SVBMFont() {

}

void SVBMFont::init(SVFontConfPtr _fontConf){
    SVBMFontConfPtr pBMFontConf = DYN_TO_SHAREPTR(SVBMFontConf, _fontConf);
//    if (pBMFontConf->m_fontDefMap.empty())
//        return;
//    
//    u64 numGlyphs = pBMFontConf->m_characterSet.size();
//    if (numGlyphs == 0)
//        return;
//    
//    if (pBMFontConf->m_commonHeight == 0)
//        return;
//    s32 originalFontSize = pBMFontConf->m_fontSize;
//    f32 originalLineHeight = pBMFontConf->m_commonHeight;
//    f32 factor = 0.0f;
//    if (fabsf(pBMFontConf->m_fontSize - originalFontSize) < FLT_EPSILON) {
//        factor = 1.0f;
//    }else {
//        factor = pBMFontConf->m_fontSize / originalFontSize;
//    }
//    m_lineHeight = originalLineHeight * factor;
//    SVMap<s32, SVBMFONTDEF>::Iterator it = pBMFontConf->m_fontDefMap.begin();
//    while ( it!=pBMFontConf->m_fontDefMap.end() ) {
//        SVBMFONTDEF& fontDef = it->data;
//        SVFONTLETTERDEFINITION tempDefinition;
//        
//        SVRect tempRect;
//        tempRect = fontDef.rect;
////        tempRect = CC_RECT_PIXELS_TO_POINTS(tempRect);
//        
//        tempDefinition.offsetX  = fontDef.xOffset;
//        tempDefinition.offsetY  = fontDef.yOffset;
//        
//        tempDefinition.u        = tempRect.m_lb_pt.x;
//        tempDefinition.v        = tempRect.m_rt_pt.y;
//        
//        tempDefinition.width    = tempRect.m_rt_pt.x - tempRect.m_lb_pt.x;
//        tempDefinition.height   = tempRect.m_lb_pt.y - tempRect.m_rt_pt.y;
//        
//        tempDefinition.textureID = 0;
//        
//        tempDefinition.validDefinition = true;
//        tempDefinition.xAdvance = fontDef.xAdvance;
//        
//        if (65535 < fontDef.charID) {
//            SV_LOG_ERROR("Warning: 65535 < fontDef.charID (%u), ignored", fontDef.charID);
//        } else {
//            addLetterDefinition(fontDef.charID, tempDefinition);
//        }
//        it++;
//    }
    
//    Texture2D *tempTexture = Director::getInstance()->getTextureCache()->addImage(_configuration->getAtlasName());
//    if (!tempTexture) {
//        CC_SAFE_RELEASE(tempAtlas);
//        return nullptr;
//    }
//
//    // add the texture
//    tempAtlas->addTexture(tempTexture, 0);
//
//    // done
//    return tempAtlas;
}
