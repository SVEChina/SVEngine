//
// SVFontProcess.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FONTMODULE_H
#define SV_FONTMODULE_H

#include "SVProcess.h"

#ifdef CONFIG_IS_LOAD_FREETYPE

typedef struct FT_GlyphSlotRec_ *FT_GlyphSlot;
typedef struct FT_LibraryRec_ *FT_Library;
typedef struct FT_FaceRec_ *FT_Face;
typedef struct FT_Bitmap_ FT_Bitmap;
typedef signed int FT_Int;

#endif

namespace sv {
    
    namespace logic{
        
        class SVFontProcess : public SVProcess {
        public:
            SVFontProcess(SVInst* _app);
            
            ~SVFontProcess();
            
            virtual bool procEvent(SVEventPtr _event);
            
            void getAllBMFontNames(SVArray<SVString> &_array);
            
            void removeBMFont(cptr8 _name);
            
            SVBMFontPtr getBMFont(cptr8 _name);
            
            void loadBMFont(cptr8 _path);
#ifdef CONFIG_IS_LOAD_FREETYPE
            
            FT_Library getFontLib();
            
            FT_Face getFontFace();
            
            s32 getFontPixelSize();
            
        protected:
            FT_Library      m_fontLib;
            FT_Face         m_fontFace;
            s32             m_fontPixelSize;
            s32             m_fontCharSize;
            
#endif
        protected:
            SVLockPtr m_lock;
            SVArray<SVBMFontPtr> m_bmFonts;
        };
        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_FONTMODULE_H
