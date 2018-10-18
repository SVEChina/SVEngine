//
// SVFont.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFont.h"
#include "../core/SVFontConf.h"
SVFont::SVFont(SVInst *_app)
:SVGBase(_app) {

}

SVFont::~SVFont() {
    m_letterDefinitions.clear();
}

void SVFont::init(SVFontConfPtr _fontConf){
    
}

void SVFont::addLetterDefinition(s32 _char, SVFONTLETTERDEFINITION &_letterDefinition){
    m_letterDefinitions.append(_char, _letterDefinition);
}

bool SVFont::getLetterDefinitionForChar(s32 _char, SVFONTLETTERDEFINITION &_letterDefinition){
    SVMap<s32, SVFONTLETTERDEFINITION>::Iterator it = m_letterDefinitions.find(_char);
    if( it!=m_letterDefinitions.end() ) {
        _letterDefinition = it->data;
        return _letterDefinition.validDefinition;
    }
    return false;
}

void  SVFont::setLineHeight(f32 _lineHeight){
    m_lineHeight = _lineHeight;
}

f32 SVFont::getLineHeight(){
    return m_lineHeight;
}
