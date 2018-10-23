//
// SVBMFontNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBMFontNode.h"
#include "SVScene.h"
#include "SVCameraNode.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../basesys/SVStaticData.h"
#include "../rendercore/SVRenderMesh.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../core/SVBMFont.h"

#define SV_BMFONT_MAX_NUM  20
//
SVBMFontNode::SVBMFontNode(SVInst *_app)
:SVNode(_app) {
    m_font = nullptr;
    m_textDirty = true;
    m_textSize = 0;
    m_atchType = ATCH_MC;
    m_spacing = 0.0f;
    m_fontW = 60;
    m_fontH = 60;
    m_alpha = 1.0f;
    m_pRenderVertex = MakeSharedPtr<SVDataSwap>();
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMesh = MakeSharedPtr<SVRenderMesh>(mApp);
    m_pMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
    _genMesh();
}

SVBMFontNode::~SVBMFontNode() {
    m_font = nullptr;
    m_pRenderObj = nullptr;
    m_pMesh = nullptr;
    m_pRenderVertex = nullptr;
    m_texture = nullptr;
    m_texcoordsTbl.clear();
}

void SVBMFontNode::update(f32 dt) {
    SVNode::update(dt);
    if (!m_font) {
        return;
    }
    if (m_textSize == 0) {
        return;
    }
    if (m_textDirty) {
        m_textDirty = false;
        _refresh();
    }
    if (m_pRenderObj && m_pMesh ) {
        //材质独立性
        SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d_c");
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        if (m_texture) {
            t_mtl->setTexture(0,m_texture);
            t_mtl->setTexSizeIndex(0, 1.0/m_texture->getwidth(), 1.0/m_texture->getheight());
        }
        t_mtl->setModelMatrix(m_absolutMat.get());
        t_mtl->setTexcoordFlip(1.0, 1.0f);
        t_mtl->update(dt);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(t_mtl);
    }
}

void SVBMFontNode::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj && m_pMesh) {
            m_pRenderObj->pushCmd(t_rs, RST_FREETYPE, "SVBMFontNode");
        }
    }
    SVNode::render();
}

void SVBMFontNode::setFont(SVBMFontPtr _font){
    if (m_font != _font) {
        m_font = _font;
        m_textDirty = true;
    }
}

void SVBMFontNode::setText(cptr8 _text){
    m_text = _text;
    m_textSize = m_text.size();
    m_textDirty = true;
}

cptr8 SVBMFontNode::getText(){
    return m_text;
}

void SVBMFontNode::setFontSize(f32 _w,f32 _h){
    m_fontW = _w;
    m_fontH = _w;
    m_textDirty = true;
}

f32 SVBMFontNode::getFontW(){
    return m_fontW;
}

f32 SVBMFontNode::getFontH(){
    return m_fontH;
}

void SVBMFontNode::setSpacing(f32 _spacing){
    m_spacing = _spacing;
    m_textDirty = true;
}

f32 SVBMFontNode::getSpacing(){
    return m_spacing;
}

void SVBMFontNode::setAtcPt(BITFONT_ATCH_PT _type){
    m_atchType = _type;
    m_textDirty = true;
}

BITFONT_ATCH_PT SVBMFontNode::getAtcPt(){
    return m_atchType;
}

void SVBMFontNode::setAlpha(f32 _alpha){
    if (_alpha < 0 || _alpha > 1) {
        return;
    }
    if (m_alpha != _alpha) {
        m_textDirty = true;
        m_alpha = _alpha;
    }
}

void SVBMFontNode::_refresh(){
    _refreshTexcoords();
    s32 t_Len = m_texcoordsTbl.size();
    f32 t_total_w = m_fontW*t_Len;
    f32 t_total_h = m_fontH;
    f32 t_offx = 0.0f;
    f32 t_offy = 0.0f;
    if(m_atchType == ATCH_LB){
        t_offx = 0.0f;
        t_offy = 0.0f;
    }else if(m_atchType == ATCH_LC){
        t_offx = 0.0f;
        t_offy = t_total_h*0.5f;
    }else if(m_atchType == ATCH_LT){
        t_offx = 0.0f;
        t_offy = t_total_h;
    }else if(m_atchType == ATCH_MB){
        t_offx = 0.5f*t_total_w;
        t_offy = 0.0f;
    }else if(m_atchType == ATCH_MC){
        t_offx = 0.5f*t_total_w;
        t_offy = t_total_h*0.5f;
    }else if(m_atchType == ATCH_MT){
        t_offx = 0.5f*t_total_w;
        t_offy = t_total_h;
    }else if(m_atchType == ATCH_RB){
        t_offx = t_total_w;
        t_offy = 0.0f;
    }else if(m_atchType == ATCH_RC){
        t_offx = t_total_w;
        t_offy = t_total_h*0.5f;
    }else if(m_atchType == ATCH_RT){
        t_offx = t_total_w;
        t_offy = t_total_h;
    }
    //顶点数据
    V2_C_T0 tVerts[SV_BMFONT_MAX_NUM * 6];
    //更新每个字符的的纹理坐标
    for (u32 i = 0; i < t_Len; ++i) {
        tVerts[i * 6 + 0].x = (m_fontW + m_spacing)*i - t_offx;
        tVerts[i * 6 + 0].y = -t_offy;
        tVerts[i * 6 + 0].t0x = m_texcoordsTbl[i].lb_x;
        tVerts[i * 6 + 0].t0y = m_texcoordsTbl[i].lb_y;
        tVerts[i * 6 + 0].r = 255;
        tVerts[i * 6 + 0].g = 255;
        tVerts[i * 6 + 0].b = 255;
        tVerts[i * 6 + 0].a = 255*m_alpha;
        //
        tVerts[i * 6 + 1].x = m_fontW*(i+1) + m_spacing*i - t_offx;
        tVerts[i * 6 + 1].y = -t_offy;
        tVerts[i * 6 + 1].t0x = m_texcoordsTbl[i].rb_x;
        tVerts[i * 6 + 1].t0y = m_texcoordsTbl[i].rb_y;
        tVerts[i * 6 + 1].r = 255;
        tVerts[i * 6 + 1].g = 255;
        tVerts[i * 6 + 1].b = 255;
        tVerts[i * 6 + 1].a = 255*m_alpha;
        //
        tVerts[i * 6 + 2].x = (m_fontW + m_spacing)*i - t_offx;
        tVerts[i * 6 + 2].y = m_fontH - t_offy;
        tVerts[i * 6 + 2].t0x = m_texcoordsTbl[i].lt_x;
        tVerts[i * 6 + 2].t0y = m_texcoordsTbl[i].lt_y;
        tVerts[i * 6 + 2].r = 255;
        tVerts[i * 6 + 2].g = 255;
        tVerts[i * 6 + 2].b = 255;
        tVerts[i * 6 + 2].a = 255*m_alpha;
        //
        tVerts[i * 6 + 3].x = (m_fontW + m_spacing)*i - t_offx;
        tVerts[i * 6 + 3].y = m_fontH - t_offy;
        tVerts[i * 6 + 3].t0x = m_texcoordsTbl[i].lt_x;
        tVerts[i * 6 + 3].t0y = m_texcoordsTbl[i].lt_y;
        tVerts[i * 6 + 3].r = 255;
        tVerts[i * 6 + 3].g = 255;
        tVerts[i * 6 + 3].b = 255;
        tVerts[i * 6 + 3].a = 255*m_alpha;
        //
        tVerts[i * 6 + 4].x = m_fontW*(i+1) + m_spacing*i - t_offx;
        tVerts[i * 6 + 4].y = - t_offy;
        tVerts[i * 6 + 4].t0x = m_texcoordsTbl[i].rb_x;
        tVerts[i * 6 + 4].t0y = m_texcoordsTbl[i].rb_y;
        tVerts[i * 6 + 4].r = 255;
        tVerts[i * 6 + 4].g = 255;
        tVerts[i * 6 + 4].b = 255;
        tVerts[i * 6 + 4].a = 255*m_alpha;
        //
        tVerts[i * 6 + 5].x = m_fontW*(i+1) + m_spacing*i - t_offx;
        tVerts[i * 6 + 5].y = m_fontH - t_offy;
        tVerts[i * 6 + 5].t0x = m_texcoordsTbl[i].rt_x;
        tVerts[i * 6 + 5].t0y = m_texcoordsTbl[i].rt_y;
        tVerts[i * 6 + 5].r = 255;
        tVerts[i * 6 + 5].g = 255;
        tVerts[i * 6 + 5].b = 255;
        tVerts[i * 6 + 5].a = 255*m_alpha;
    }
    //
    if (t_Len < SV_BMFONT_MAX_NUM) {
        for (s32 i = t_Len; i < SV_BMFONT_MAX_NUM; ++i) {
            tVerts[i * 6 + 0].x = 0;
            tVerts[i * 6 + 0].y = 0;
            tVerts[i * 6 + 1].x = 0;
            tVerts[i * 6 + 1].y = 0;
            tVerts[i * 6 + 2].x = 0;
            tVerts[i * 6 + 2].y = 0;
            tVerts[i * 6 + 3].x = 0;
            tVerts[i * 6 + 3].y = 0;
            tVerts[i * 6 + 4].x = 0;
            tVerts[i * 6 + 4].y = 0;
            tVerts[i * 6 + 5].x = 0;
            tVerts[i * 6 + 5].y = 0;
        }
    }
    //
    m_pRenderVertex->writeData(&tVerts[0], sizeof(V2_C_T0) * t_Len * 6);
    m_pMesh->setVertexDataNum(t_Len * 6);
    m_pMesh->setVertexData(m_pRenderVertex);
    m_pMesh->setVertexType(E_VF_V2_C_T0);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    m_pMesh->createMesh();
}

void SVBMFontNode::_genMesh(){
    V2_C_T0 t_Verts[SV_BMFONT_MAX_NUM * 6];
    for (u32 i = 0; i < SV_BMFONT_MAX_NUM ; i++) {
        t_Verts[i * 6 + 0].x = m_fontW*i;
        t_Verts[i * 6 + 0].y = 0.0f;
        t_Verts[i * 6 + 0].t0x = 0.0f;
        t_Verts[i * 6 + 0].t0y = 0.0f;
        t_Verts[i * 6 + 0].r = 255;
        t_Verts[i * 6 + 0].g = 255;
        t_Verts[i * 6 + 0].b = 255;
        t_Verts[i * 6 + 0].a = 255;
        
        t_Verts[i * 6 + 1].x = m_fontW*(i+1);
        t_Verts[i * 6 + 1].y = 0.0f;
        t_Verts[i * 6 + 1].t0x = 1.0f;
        t_Verts[i * 6 + 1].t0y = 0.0f;
        t_Verts[i * 6 + 1].r = 255;
        t_Verts[i * 6 + 1].g = 255;
        t_Verts[i * 6 + 1].b = 255;
        t_Verts[i * 6 + 1].a = 255;
        
        t_Verts[i * 6 + 2].x = m_fontW*i;
        t_Verts[i * 6 + 2].y = m_fontH;
        t_Verts[i * 6 + 2].t0x = 0.0f;
        t_Verts[i * 6 + 2].t0y = 1.0f;
        t_Verts[i * 6 + 2].r = 255;
        t_Verts[i * 6 + 2].g = 255;
        t_Verts[i * 6 + 2].b = 255;
        t_Verts[i * 6 + 2].a = 255;
        
        t_Verts[i * 6 + 3].x = m_fontW*i;
        t_Verts[i * 6 + 3].y = m_fontH;
        t_Verts[i * 6 + 3].t0x = 0.0f;
        t_Verts[i * 6 + 3].t0y = 1.0f;
        t_Verts[i * 6 + 3].r = 255;
        t_Verts[i * 6 + 3].g = 255;
        t_Verts[i * 6 + 3].b = 255;
        t_Verts[i * 6 + 3].a = 255;
        
        t_Verts[i * 6 + 4].x = m_fontW*(i+1);
        t_Verts[i * 6 + 4].y = 0.0f;
        t_Verts[i * 6 + 4].t0x = 1.0f;
        t_Verts[i * 6 + 4].t0y = 0.0f;
        t_Verts[i * 6 + 4].r = 255;
        t_Verts[i * 6 + 4].g = 255;
        t_Verts[i * 6 + 4].b = 255;
        t_Verts[i * 6 + 4].a = 255;
        
        t_Verts[i * 6 + 5].x = m_fontW*(i+1);
        t_Verts[i * 6 + 5].y = m_fontH;
        t_Verts[i * 6 + 5].t0x = 1.0f;
        t_Verts[i * 6 + 5].t0y = 1.0f;
        t_Verts[i * 6 + 5].r = 255;
        t_Verts[i * 6 + 5].g = 255;
        t_Verts[i * 6 + 5].b = 255;
        t_Verts[i * 6 + 5].a = 255;
    }
    //
    m_pRenderVertex->writeData(&t_Verts[0], sizeof(V2_C_T0) * SV_BMFONT_MAX_NUM * 6);
    m_pMesh->setVertexDataNum(SV_BMFONT_MAX_NUM * 6);
    m_pMesh->setVertexData(m_pRenderVertex);
    m_pMesh->setVertexType(E_VF_V2_C_T0);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    //
    m_pMesh->createMesh();
}

void SVBMFontNode::_refreshTexcoords(){
    m_texcoordsTbl.clear();
    s32 page = -1;
    for( int n = 0; n < m_textSize; )
    {
        s32 charId = m_font->getTextChar(m_text, n, &n);
        SVBMFont::SVBMFONTCHARINFO ch = m_font->getChar(charId);
        f32 u = (f32(ch.x)+0.5f) / m_font->m_scaleW;
        f32 v = (f32(ch.y)+0.5f) / m_font->m_scaleH;
        f32 u2 = u + f32(ch.width) / m_font->m_scaleW;
        f32 v2 = v + f32(ch.height) / m_font->m_scaleH;
        
        f32 a = m_font->m_scale * f32(ch.xAdvance);
        f32 w = m_font->m_scale * f32(ch.width);
        f32 h = m_font->m_scale * f32(ch.height);
        f32 ox = m_font->m_scale * f32(ch.xOffset);
        f32 oy = m_font->m_scale * f32(ch.yOffset);
        
        if( ch.page != page )
        {
            page = ch.page;
            m_texture = m_font->m_textures[page];
        }
        FontTexcoords t_texcoord;
        t_texcoord.lt_x = u;
        t_texcoord.lt_y = v;
        //
        t_texcoord.rt_x = u2;
        t_texcoord.rt_y = v;
        //
        t_texcoord.lb_x = u;
        t_texcoord.lb_y = v2;
        //
        t_texcoord.rb_x = u2;
        t_texcoord.rb_y = v2;
        //
        m_texcoordsTbl.append(t_texcoord);
    }

}
