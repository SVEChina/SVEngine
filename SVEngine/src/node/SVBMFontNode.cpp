//
// SVBMFontNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBMFontNode.h"
#include "SVScene.h"
#include "SVCameraNode.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVStaticData.h"
#include "../basesys/SVConfig.h"
#include "../basesys/SVFontProcess.h"
#include "../core/SVBMFont.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../file/SVBMFontLoader.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtl2D.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#define SV_BMFONT_MAX_NUM  20
#define DEFSPACE 40
//
SVBMFontNode::SVBMFontNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVBMFontNode";
    m_font = nullptr;
    m_textDirty = true;
    m_rsType = RST_SOLID_3D;
    m_atchType = ATCH_MC;
    m_spacing = 0.0f;
    m_alpha = 1.0f;
    m_fontSize = 1.0f;
    m_pRenderVertex = MakeSharedPtr<SVDataSwap>();
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMesh = MakeSharedPtr<SVRenderMesh>(mApp);
    m_pMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
    m_spacing = 0.0f;
    _genMesh();
}

SVBMFontNode::~SVBMFontNode() {
    m_font = nullptr;
    m_pRenderObj = nullptr;
    m_pMesh = nullptr;
    m_pRenderVertex = nullptr;
    m_texture = nullptr;
}

void SVBMFontNode::update(f32 dt) {
    SVNode::update(dt);
    if (!m_font) {
        return;
    }
    if (m_textDirty) {
        m_textDirty = false;
        _refresh();
    }
    if (m_pRenderObj && m_pMesh ) {
        //材质独立性
        SVMtl2DPtr t_mtl = MakeSharedPtr<SVMtl2D>(mApp, "normal2d_c");
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        if (m_texture) {
            t_mtl->setTexture(0,m_texture);
            t_mtl->setTexSizeIndex(0, 1.0/m_texture->getwidth(), 1.0/m_texture->getheight());
        }
        t_mtl->setModelMatrix(m_absolutMat.get());
        t_mtl->setTexcoordFlip(1.0, 1.0f);
        t_mtl->setAlpha(m_alpha);
        t_mtl->update(dt);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(t_mtl);
    }
}

void SVBMFontNode::render() {
    if (m_visible){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj && m_pMesh) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVBMFontNode");
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

void SVBMFontNode::setFont(cptr8 _name){
    SVFontProcessPtr fontProcess = mApp->getBasicSys()->getFontModule();
    SVBMFontPtr t_font = fontProcess->getBMFont(_name);
    if (t_font && t_font != m_font) {
        m_font = t_font;
        m_textDirty = true;
    }
}

void SVBMFontNode::setText(cptr8 _text){
    m_text = _text;
    m_textDirty = true;
}

cptr8 SVBMFontNode::getText(){
    return m_text;
}

void SVBMFontNode::setSpacing(f32 _spacing){
    m_spacing = _spacing;
    m_textDirty = true;
}

void SVBMFontNode::setFontSize(f32 _size){
    if (_size <= 0.0f) {
        return;
    }
    m_fontSize = _size;
}

f32 SVBMFontNode::getWidth(){
    if (!m_font) {
        return 0;
    }
    f32 t_scaleX = 1.0f;
    SVNodePtr t_curNode = THIS_TO_SHAREPTR(SVBMFontNode);
    while (t_curNode) {
        t_scaleX = t_scaleX * t_curNode->getScale().x;
        if (t_curNode->getParent()) {
            t_curNode = t_curNode->getParent();
        } else {
            break;
        }
    }
    f32 t_total_w = m_font->getTextWidth(m_text.c_str(), m_fontSize);
    t_total_w = t_total_w + (m_text.size() - 1)*m_spacing;
    return t_total_w*t_scaleX;
}

f32 SVBMFontNode::getHeight(){
    if (!m_font) {
        return 0;
    }
    f32 t_scaleY = 1.0f;
    SVNodePtr t_curNode = THIS_TO_SHAREPTR(SVBMFontNode);
    while (t_curNode) {
        t_scaleY = t_scaleY * t_curNode->getScale().y;
        if (t_curNode->getParent()) {
            t_curNode = t_curNode->getParent();
        } else {
            break;
        }
    }
    return m_font->getTextHeight(m_text.c_str(), m_fontSize)*t_scaleY;
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
    //顶点数据
    V2_C_T0 tVerts[SV_BMFONT_MAX_NUM * 6];
    s32 t_texLen = m_font->getTextLength(m_text.c_str());
    f32 t_total_w = m_font->getTextWidth(m_text.c_str(), m_fontSize);
    if (m_text.size() > 1) {
        t_total_w = t_total_w + (m_text.size() - 1)*m_spacing;
    }
    f32 x = 0.0f;
    if(m_atchType == ATCH_LC){
        x = t_total_w;
    }else if(m_atchType == ATCH_MC){
        x = -t_total_w*0.5;
    }else if(m_atchType == ATCH_RC){
        x = -t_total_w;
    }
    m_aabbBox.clear();
    s32 page = -1;
    s32 i = 0;
    for( s32 n = 0; n < t_texLen; )
    {
        s32 charId = m_font->getTextChar(m_text, n, &n);
        SVBMFont::SVBMFONTCHARINFO ch = m_font->getChar(charId);
        f32 u = (f32(ch.x)+0.5f) / m_font->m_fontScaleW;
        f32 v = (f32(ch.y)+0.5f) / m_font->m_fontScaleH;
        f32 u2 = u + f32(ch.width) / m_font->m_fontScaleW;
        f32 v2 = v + f32(ch.height) / m_font->m_fontScaleH;
        
        f32 a = m_fontSize * f32(ch.xAdvance);
        f32 w = m_fontSize * f32(ch.width);
        f32 h = m_fontSize * f32(ch.height);
        f32 ox = m_fontSize * f32(ch.xOffset);
        f32 oy = m_fontSize * f32(ch.yOffset);
        
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
        t_texcoord.ox = ox;
        t_texcoord.oy = oy;
        //
        t_texcoord.w = w;
        t_texcoord.h = h;
        //
        t_texcoord.a = a;
        //
        t_texcoord.charID = charId;
        //数据
        tVerts[i * 6 + 0].x = x+t_texcoord.ox;
        tVerts[i * 6 + 0].y = -(t_texcoord.h+t_texcoord.oy)*0.5;
        tVerts[i * 6 + 0].t0x = t_texcoord.lb_x;
        tVerts[i * 6 + 0].t0y = t_texcoord.lb_y;
        tVerts[i * 6 + 0].r = 255;
        tVerts[i * 6 + 0].g = 255;
        tVerts[i * 6 + 0].b = 255;
        tVerts[i * 6 + 0].a = 255*m_alpha;
        m_aabbBox.expand(FVec3(tVerts[i * 6 + 0].x,tVerts[i * 6 + 0].y, 0.0));
        //
        tVerts[i * 6 + 1].x = x+t_texcoord.w+t_texcoord.ox;
        tVerts[i * 6 + 1].y = -(t_texcoord.h+t_texcoord.oy)*0.5;
        tVerts[i * 6 + 1].t0x = t_texcoord.rb_x;
        tVerts[i * 6 + 1].t0y = t_texcoord.rb_y;
        tVerts[i * 6 + 1].r = 255;
        tVerts[i * 6 + 1].g = 255;
        tVerts[i * 6 + 1].b = 255;
        tVerts[i * 6 + 1].a = 255*m_alpha;
        m_aabbBox.expand(FVec3(tVerts[i * 6 + 1].x,tVerts[i * 6 + 1].y, 0.0));
        //
        tVerts[i * 6 + 2].x = x+t_texcoord.ox;
        tVerts[i * 6 + 2].y = -(t_texcoord.oy*0.5-t_texcoord.h*0.5);
        tVerts[i * 6 + 2].t0x = t_texcoord.lt_x;
        tVerts[i * 6 + 2].t0y = t_texcoord.lt_y;
        tVerts[i * 6 + 2].r = 255;
        tVerts[i * 6 + 2].g = 255;
        tVerts[i * 6 + 2].b = 255;
        tVerts[i * 6 + 2].a = 255*m_alpha;
        m_aabbBox.expand(FVec3(tVerts[i * 6 + 2].x,tVerts[i * 6 + 2].y, 0.0));
        //
        tVerts[i * 6 + 3].x = x+t_texcoord.ox;
        tVerts[i * 6 + 3].y = -(t_texcoord.oy*0.5-t_texcoord.h*0.5);
        tVerts[i * 6 + 3].t0x = t_texcoord.lt_x;
        tVerts[i * 6 + 3].t0y = t_texcoord.lt_y;
        tVerts[i * 6 + 3].r = 255;
        tVerts[i * 6 + 3].g = 255;
        tVerts[i * 6 + 3].b = 255;
        tVerts[i * 6 + 3].a = 255*m_alpha;
        m_aabbBox.expand(FVec3(tVerts[i * 6 + 3].x,tVerts[i * 6 + 3].y, 0.0));
        //
        tVerts[i * 6 + 4].x = x+t_texcoord.w+t_texcoord.ox;
        tVerts[i * 6 + 4].y = -(t_texcoord.h+t_texcoord.oy)*0.5;
        tVerts[i * 6 + 4].t0x = t_texcoord.rb_x;
        tVerts[i * 6 + 4].t0y = t_texcoord.rb_y;
        tVerts[i * 6 + 4].r = 255;
        tVerts[i * 6 + 4].g = 255;
        tVerts[i * 6 + 4].b = 255;
        tVerts[i * 6 + 4].a = 255*m_alpha;
        m_aabbBox.expand(FVec3(tVerts[i * 6 + 4].x,tVerts[i * 6 + 4].y, 0.0));
        //
        tVerts[i * 6 + 5].x = x+t_texcoord.w+t_texcoord.ox;
        tVerts[i * 6 + 5].y = -(t_texcoord.oy*0.5-t_texcoord.h*0.5);
        tVerts[i * 6 + 5].t0x = t_texcoord.rt_x;
        tVerts[i * 6 + 5].t0y = t_texcoord.rt_y;
        tVerts[i * 6 + 5].r = 255;
        tVerts[i * 6 + 5].g = 255;
        tVerts[i * 6 + 5].b = 255;
        tVerts[i * 6 + 5].a = 255*m_alpha;
        m_aabbBox.expand(FVec3(tVerts[i * 6 + 5].x,tVerts[i * 6 + 5].y, 0.0));
        x += t_texcoord.a;
        x += m_spacing;
        if (t_texcoord.charID == ' ') {
            x += DEFSPACE;
        }
        i++;
    }
    s32 t_len = i;
    m_pRenderVertex->writeData(&tVerts[0], sizeof(V2_C_T0) * t_len * 6);
    m_pMesh->setVertexDataNum(t_len * 6);
    m_pMesh->setVertexData(m_pRenderVertex);
}

void SVBMFontNode::_genMesh(){
    V2_C_T0 t_Verts[SV_BMFONT_MAX_NUM * 6];
    for (u32 i = 0; i < SV_BMFONT_MAX_NUM ; i++) {
        t_Verts[i * 6 + 0].x = 0.0f;
        t_Verts[i * 6 + 0].y = 0.0f;
        t_Verts[i * 6 + 0].t0x = 0.0f;
        t_Verts[i * 6 + 0].t0y = 0.0f;
        t_Verts[i * 6 + 0].r = 255;
        t_Verts[i * 6 + 0].g = 255;
        t_Verts[i * 6 + 0].b = 255;
        t_Verts[i * 6 + 0].a = 255;
        
        t_Verts[i * 6 + 1].x = 0.0f;
        t_Verts[i * 6 + 1].y = 0.0f;
        t_Verts[i * 6 + 1].t0x = 1.0f;
        t_Verts[i * 6 + 1].t0y = 0.0f;
        t_Verts[i * 6 + 1].r = 255;
        t_Verts[i * 6 + 1].g = 255;
        t_Verts[i * 6 + 1].b = 255;
        t_Verts[i * 6 + 1].a = 255;
        
        t_Verts[i * 6 + 2].x = 0.0f;
        t_Verts[i * 6 + 2].y = 0.0f;
        t_Verts[i * 6 + 2].t0x = 0.0f;
        t_Verts[i * 6 + 2].t0y = 1.0f;
        t_Verts[i * 6 + 2].r = 255;
        t_Verts[i * 6 + 2].g = 255;
        t_Verts[i * 6 + 2].b = 255;
        t_Verts[i * 6 + 2].a = 255;
        
        t_Verts[i * 6 + 3].x = 0.0f;
        t_Verts[i * 6 + 3].y = 0.0f;
        t_Verts[i * 6 + 3].t0x = 0.0f;
        t_Verts[i * 6 + 3].t0y = 1.0f;
        t_Verts[i * 6 + 3].r = 255;
        t_Verts[i * 6 + 3].g = 255;
        t_Verts[i * 6 + 3].b = 255;
        t_Verts[i * 6 + 3].a = 255;
        
        t_Verts[i * 6 + 4].x = 0.0f;
        t_Verts[i * 6 + 4].y = 0.0f;
        t_Verts[i * 6 + 4].t0x = 1.0f;
        t_Verts[i * 6 + 4].t0y = 0.0f;
        t_Verts[i * 6 + 4].r = 255;
        t_Verts[i * 6 + 4].g = 255;
        t_Verts[i * 6 + 4].b = 255;
        t_Verts[i * 6 + 4].a = 255;
        
        t_Verts[i * 6 + 5].x = 0.0f;
        t_Verts[i * 6 + 5].y = 0.0f;
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
    m_pMesh->createMesh();
}

//序列化接口
void SVBMFontNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    _toJsonData(_allocator, _objValue);
    _objValue.AddMember("fntname", RAPIDJSON_NAMESPACE::StringRef(m_font->m_fntFileName.c_str()), _allocator);
    _objValue.AddMember("content", RAPIDJSON_NAMESPACE::StringRef(m_text.c_str()), _allocator);
    _objValue.AddMember("encode", s32(m_font->getTextEncoding()), _allocator);
    _objValue.AddMember("space", m_spacing, _allocator);
    _objValue.AddMember("alignment", s32(m_atchType), _allocator);
    _objValue.AddMember("encode", s32(m_font->getTextEncoding()), _allocator);
}

void SVBMFontNode::fromJSON(RAPIDJSON_NAMESPACE::Value &_item){
    _fromJsonData(_item);
    if (_item.HasMember("fntname") && _item["fntname"].IsString()) {
        SVString t_fntName = _item["fntname"].GetString();
        SVString t_resPath = m_rootPath +  t_fntName;
        SVFontProcessPtr fontProcess = mApp->getBasicSys()->getFontModule();
        fontProcess->loadBMFont(t_resPath);
        setFont(t_fntName.c_str());
    }
    if (_item.HasMember("content") && _item["content"].IsString()) {
        m_text = _item["content"].GetString();
    }
    if (_item.HasMember("encode") && _item["encode"].IsInt()) {
        if (m_font) {
            m_font->setTextEncoding(SVFont::SVFONTTEXTENCODING(_item["encode"].GetInt()));
        }
    }
    if (_item.HasMember("space") && _item["space"].IsFloat()) {
        m_spacing = _item["space"].GetFloat();
    }
    if (_item.HasMember("alignment") && _item["alignment"].IsInt()) {
        m_atchType = BITFONT_ATCH_PT(_item["alignment"].GetInt());
    }
    m_textDirty = true;
}
