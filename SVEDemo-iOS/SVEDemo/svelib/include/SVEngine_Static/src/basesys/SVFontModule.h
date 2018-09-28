//
// SVFontModule.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FONTMODULE_H
#define SV_FONTMODULE_H

#ifdef CONFIG_IS_LOAD_FREETYPE

#include "../event/SVEventProc.h"
typedef struct FT_GlyphSlotRec_ *FT_GlyphSlot;
typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_FaceRec_ *FT_Face;
typedef struct FT_Bitmap_ FT_Bitmap;
typedef signed int FT_Int;

namespace sv {
    
    namespace logic{
        
        class SVFontModule : public SVEventProc {
        public:
            SVFontModule(SVInst* _app);
            
            ~SVFontModule();
            
            FT_Library getFontLib();
            
            FT_Face getFontFace();
            
            s32 getFontPixelSize();
            
            virtual bool procEvent(SVEventPtr _event);
            
        protected:
            FT_Library      m_fontLib;
            FT_Face         m_fontFace;
            s32             m_fontPixelSize;
            s32             m_fontCharSize;
        };
        
    }//!namespace logic
    
}//!namespace sv


#endif
#endif //SV_FONTMODULE_H
