//
// SVFontProcess.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFontProcess.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalParam.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../file/SVFileMgr.h"
#include "../node/SVFreeTypeNode.h"

#ifdef CONFIG_IS_LOAD_FREETYPE
#include "ft2build.h"
#include "freetype.h"
#endif

SVFontProcess::SVFontProcess(SVInst *_app)
:SVProcess(_app) {
    
#ifdef CONFIG_IS_LOAD_FREETYPE
    m_fontFace = nullptr;
    m_fontLib = nullptr;
    m_fontPixelSize = 64;
    m_fontCharSize = 16;
    //
    do {
        SVString t_fullname = mApp->getFileMgr()->getFileFullName(mApp->m_pGlobalParam->m_fontFileName.c_str());
        // 1. 初始化freetype2库
        FT_Error t_error = FT_Init_FreeType(&m_fontLib);
        if (FT_Err_Ok != t_error) {
            break;
        }
        // 2. 创建一个face
        t_error = FT_New_Face(m_fontLib, t_fullname.c_str(), 0, &m_fontFace);
        if (FT_Err_Ok != t_error) {
            FT_Done_Face( m_fontFace );
            FT_Done_FreeType( m_fontLib );
            m_fontFace = nullptr;
            m_fontLib = nullptr;
            break;
        }
        // 3. 设置字体尺寸 (大小 x 64 ) 设备水平分辨率 垂直分辨率
        t_error = FT_Set_Char_Size(m_fontFace, m_fontCharSize*64, m_fontCharSize*64, 0, 0);
        if (FT_Err_Ok != t_error) {
            FT_Done_Face( m_fontFace );
            FT_Done_FreeType( m_fontLib );
            m_fontFace = nullptr;
            m_fontLib = nullptr;
        }
        // 4. 设置实际像素大小
        t_error = FT_Set_Pixel_Sizes(m_fontFace, m_fontPixelSize, m_fontPixelSize);
        if (FT_Err_Ok != t_error) {
            FT_Done_Face( m_fontFace );
            FT_Done_FreeType( m_fontLib );
            m_fontFace = nullptr;
            m_fontLib = nullptr;
        }
    }while(0);
#endif
}

SVFontProcess::~SVFontProcess() {
#ifdef CONFIG_IS_LOAD_FREETYPE
    FT_Done_Face( m_fontFace );
    FT_Done_FreeType( m_fontLib );
    m_fontFace = nullptr;
    m_fontLib = nullptr;
#endif
}

bool SVFontProcess::procEvent(SVEventPtr _event){
    return true;
}

#ifdef CONFIG_IS_LOAD_FREETYPE
FT_Library SVFontProcess::getFontLib(){
    return m_fontLib;
}

FT_Face SVFontProcess::getFontFace(){
    return m_fontFace;
}

s32 SVFontProcess::getFontPixelSize(){
    return m_fontPixelSize;
}

#endif
