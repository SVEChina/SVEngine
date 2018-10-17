//
// SVFont.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FONT_H
#define SV_FONT_H

#include "../base/SVObject.h"
#include "../base/SVGBase.h"
#include "../base/SVPreDeclare.h"
#include "../base/SVMap.h"
namespace sv {
    struct SVFONTLETTERDEFINITION
    {
        f32 u;
        f32 v;
        f32 width;
        f32 height;
        f32 offsetX;
        f32 offsetY;
        s32 textureID;
        s32 xAdvance;
        bool validDefinition;
    };
    class SVFont : public SVGBase {
    public:
        SVFont(SVInst *_app);
        
        ~SVFont();
        
        void init(SVFontConfPtr _fontConf);
        
        void addLetterDefinition(s32 _char, SVFONTLETTERDEFINITION &_letterDefinition);
        
        bool getLetterDefinitionForChar(s32 _char, SVFONTLETTERDEFINITION &_letterDefinition);
        
        void  setLineHeight(f32 _lineHeight);
        
        f32 getLineHeight();
    protected:
        SVMap<s32, SVFONTLETTERDEFINITION> m_letterDefinitions;
        f32 m_lineHeight;
    };

    
}//!namespace sv



#endif //SV_FONT_H
