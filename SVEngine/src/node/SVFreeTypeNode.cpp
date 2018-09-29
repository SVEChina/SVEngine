//
// SVFreeTypeNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFreeTypeNode.h"

#ifdef CONFIG_IS_LOAD_FREETYPE

#include "ft2build.h"
#include "freetype.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../core/SVGeoGen.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVFontProcess.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVFontProcess.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"

SVFreeTypeNode::SVFreeTypeNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVFreeTypeNode";
    m_fontPixelSize=0;    //字体标准像素尺寸
    m_DefTextPixelSize=0; //
    m_textChange = false;
    m_Text = "";
    m_texture = nullptr;
    m_pMesh = nullptr;
    m_renderObject = MakeSharedPtr<SVRenderObject>();
    m_pMtl= MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
}

SVFreeTypeNode::~SVFreeTypeNode() {
    m_pMesh = nullptr;
    m_renderObject = nullptr;
}

void SVFreeTypeNode::setText(cptr8 _text) {
    if (strcmp(m_Text.c_str(), _text) != 0) {
        m_Text = _text;
        m_textChange = true;
    }
}

cptr8 SVFreeTypeNode::getText() {
    return m_Text.c_str();
}

void SVFreeTypeNode::update(f32 dt) {
    SVNode::update(dt);
    if (m_textChange) {
        _updateTextTexture();
        m_textChange = false;
    }
    if (m_renderObject && m_pMesh && m_pMtl) {
        m_pMtl->update(dt);
        m_pMtl->setModelMatrix(m_absolutMat.get());
        m_pMtl->setTexcoordFlip(1.0, -1.0f);
        m_pMtl->setTexture(0,m_texture);
        m_pMtl->setBlendEnable(true);
        m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        m_renderObject->setMesh(m_pMesh);
        m_renderObject->setMtl(m_pMtl);
    }
}

void SVFreeTypeNode::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->m_pGlobalParam->m_curScene->getRenderRS();
        if (m_renderObject && m_pMesh && m_pMtl) {
            m_renderObject->pushCmd(t_rs, RST_FREETYPE, "SVFreeTypeNode");
        }
    }
    SVNode::render();
}

void SVFreeTypeNode::_updateTextTexture() {
    FT_Face t_fface = mApp->getBasicSys()->getFontModule()->getFontFace();
    if(!t_fface)
        return ;
    //trans 转化字符
    SVArray<s32> TransChar;
    for(s32 i=0;i<m_Text.size();i++){
        u32 t_result_char = 0;
        if ( (m_Text[i] & 0x80) && (u8)m_Text[i] >= 0) {
            //汉字 转换一个utf8 - unicode
            SVString t_substr = SVString::substr(m_Text.c_str(),i,3);
            SVString::utf8ToUnicode(t_substr.c_str(), t_result_char);
            i += 2;
        } else {
            t_result_char = m_Text[i];
        }
        TransChar.append(t_result_char);
    }
    //输出的像素数目
    s32 m_fontPixelSize = mApp->getBasicSys()->getFontModule()->getFontPixelSize();
    //纹理尺寸的大小
    s32 t_texw = TransChar.size()*m_fontPixelSize;  //这块是跟advance走的
    s32 t_texh = s32( (t_fface->size->metrics.ascender - t_fface->size->metrics.descender)/64 );
    //
    m_pMesh = SVGeoGen::genRect(mApp,t_texw, t_texh,0,0,t_texw,t_texh,m_aabbBox); //矩形
    m_texture = mApp->getTexMgr()->createUnctrlTexture(t_texw, t_texh,GL_RGBA,GL_RGBA);
    SVDataSwapPtr t_texture_data = MakeSharedPtr<SVDataSwap>();
    t_texture_data->resize(t_texw*t_texh*4);
    m_texture->setData(t_texture_data);
    u8* pData = (u8*)t_texture_data->getData();
    //加载位图信息
    FT_Matrix t_matrix;
    f32 angle = 0;//3.1415956f/4.0f;
    t_matrix.xx = (FT_Fixed) (cos(angle) * 0x10000L);
    t_matrix.xy = (FT_Fixed) (-sin(angle) * 0x10000L);
    t_matrix.yx = (FT_Fixed) (sin(angle) * 0x10000L);
    t_matrix.yy = (FT_Fixed) (cos(angle) * 0x10000L);
    //获取笔点(从基准点开始)
    FT_Vector t_pen;
    t_pen.x = 0;
    t_pen.y = 0;
    //
    FT_GlyphSlot t_slot = t_fface->glyph;
    for(s32 k=0;k<TransChar.size();k++){
        FT_Set_Transform(t_fface, &t_matrix, &t_pen);
        //产生点阵图
        FT_Error error = FT_Load_Char(t_fface, (FT_ULong) TransChar[k], FT_LOAD_RENDER);//FT_LOAD_MONOCHROME
        if (FT_Err_Ok != error)
            continue;
        FT_Bitmap *bitmap = &t_slot->bitmap;
        //s64 t_x = t_slot->bitmap_left;
        s64 t_y = t_texh - (t_slot->bitmap_top - t_fface->size->metrics.descender/64);  //重新修正top
        for (s32 p = 0; p < bitmap->width; p++ ) {
            for (s32 q = 0; q < bitmap->rows; q++ ) {
                u8 t_char = bitmap->buffer[q * bitmap->width + p];
                s32 t_img_x = p + t_slot->bitmap_left;
                s32 t_img_y = q + (s32)t_y;
                s32 t_img_pindex = t_img_x + t_img_y*t_texw;
                pData[4*t_img_pindex] = t_char;                //r
                pData[4*t_img_pindex+1] = t_char;              //g
                pData[4*t_img_pindex+2] = t_char;              //b
                pData[4*t_img_pindex+3] = t_char;              //a
            }
        }
        //移动笔点
        t_pen.x += t_slot->advance.x;
        t_pen.y += t_slot->advance.y;
    }
}

#endif
