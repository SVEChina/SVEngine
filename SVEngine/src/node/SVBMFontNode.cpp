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
    m_spacing = 0.0f;
    m_fontW = 60;
    m_fontH = 60;
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
    
}

void SVBMFontNode::render() {
//    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
//        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
//        if (m_pRenderObj && m_pMesh) {
//            m_pRenderObj->pushCmd(t_rs, RST_FREETYPE, "SVBMFontNode");
//        }
//    }
    SVNode::render();
}

void SVBMFontNode::setFont(SVBMFontPtr _font){
    if (m_font != _font) {
        m_font = _font;
    }
}

void SVBMFontNode::setText(cptr8 _text){
    m_text = _text;
    m_textSize = m_text.size();
    m_textDirty = true;
}

void SVBMFontNode::setFontSize(f32 _w,f32 _h){
    m_fontW = _w;
    m_fontH = _w;
}

void SVBMFontNode::setSpacing(f32 _spacing){
    m_spacing = _spacing;
}

void SVBMFontNode::_refresh(){
    if (!m_font) {
        return;
    }
    if (m_textDirty) {
        m_textDirty = false;
        _refreshTexcoords();
    }
}

void SVBMFontNode::_genMesh(){
    V2_C_T0 t_Verts[SV_BMFONT_MAX_NUM * 6];
    for (u32 i = 0; i < SV_BMFONT_MAX_NUM; i++) {
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
    if (!m_font) {
        return;
    }
    m_texcoordsTbl.clear();
    s32 page = -1;
    for( int n = 0; n < m_textSize; )
    {
        s32 charId = m_font->getTextChar(m_text, n, &n);
        SVBMFont::SVBMFONTCHARINFO ch = m_font->getChar(charId);
        // Map the center of the texel to the corners
        // in order to get pixel perfect mapping
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
//            render->End();
            page = ch.page;
//            render->GetGraphics()->SetTexture(pages[page]);
//            render->Begin(RENDER_QUAD_LIST);
        }
    }

}
