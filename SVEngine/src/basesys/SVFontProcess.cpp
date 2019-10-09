//
// SVFontProcess.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFontProcess.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalParam.h"
#include "../base/SVLock.h"
#include "../core/SVBMFont.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../file/SVFileMgr.h"
#include "../file/SVBMFontLoader.h"
#include "../node/SVFreeTypeNode.h"

#ifdef CONFIG_IS_LOAD_FREETYPE
#include "ft2build.h"
#include "freetype.h"
#endif

SVFontProcess::SVFontProcess(SVInst *_app)
:SVProcess(_app) {
    m_lock = MakeSharedPtr<SVLock>();
    loadBMFont("svres/bmfont/chineses.fnt");
    loadBMFont("svres/bmfont/sveengine.fnt");
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
    m_bmFonts.destroy();
}

bool SVFontProcess::procEvent(SVEventPtr _event){
    return true;
}

void SVFontProcess::loadBMFont(cptr8 _path){
    m_lock->lock();
    SVString t_path = _path;
    s32 pos = t_path.rfind('/');
    SVString t_name = SVString::substr(t_path.c_str(), pos+1, t_path.size() - pos - 1);
    bool t_result = false;
    for (s32 i = 0; i<m_bmFonts.size(); i++) {
        SVBMFontPtr t_font = m_bmFonts[i];
        SVString t_t_name = t_font->m_fntFileName;
        if (strcmp(t_name.c_str(), t_t_name.c_str()) == 0) {
            t_result = true;
        }
    }
    if (!t_result) {
            SVBMFontPtr font = MakeSharedPtr<SVBMFont>(mApp);
        SVBMFontLoader t_loder(mApp);
        t_loder.loadData(_path, font);
        m_bmFonts.append(font);
    }
    m_lock->unlock();
}

SVBMFontPtr SVFontProcess::getBMFont(cptr8 _name){
    for (s32 i = 0; i<m_bmFonts.size(); i++) {
        SVBMFontPtr t_font = m_bmFonts[i];
        SVString t_t_name = t_font->m_fntFileName;
        if (strcmp(_name, t_t_name.c_str()) == 0) {
            return t_font;
        }
    }
    return nullptr;
}

void SVFontProcess::removeBMFont(cptr8 _name){
    m_lock->lock();
    for (s32 i = 0; i<m_bmFonts.size(); i++) {
        SVBMFontPtr t_font = m_bmFonts[i];
        SVString t_t_name = t_font->m_fntFileName;
        if (strcmp(_name, t_t_name.c_str()) == 0) {
            m_bmFonts.removeForce(i);
            break;
        }
    }
    m_lock->unlock();
}

void SVFontProcess::getAllBMFontNames(SVArray<SVString> &_array){
    m_lock->lock();
    for (s32 i = 0; i<m_bmFonts.size(); i++) {
        SVBMFontPtr t_font = m_bmFonts[i];
        SVString t_t_name = t_font->m_fntFileName;
        _array.append(t_t_name);
    }
    m_lock->unlock();
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
