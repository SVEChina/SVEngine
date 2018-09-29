//
// SVBitFontNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBitFontNode.h"
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

#define SV_BITFONT_MAX_NUM  10
//
SVBitFontNode::SVBitFontNode(SVInst *_app)
:SVNode(_app) {
    m_texture = nullptr;
    m_fontDirty = false;
    m_FontWidth = 64.0f;    //物理尺寸宽高
    m_FontHeight = 64.0f;
    m_innerOffXScale = 0.12f;
    m_innerOffYScale = 0.0f;
    m_alpha = 1;
    m_atchType = ATCH_MC;
    m_pRenderVertex = MakeSharedPtr<SVDataSwap>();
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMesh = MakeSharedPtr<SVRenderMesh>(mApp);
    m_pMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
    //构建基础mesh和纹理坐标
    _genTexcoords();
    _genMesh();
}

SVBitFontNode::~SVBitFontNode() {
    m_pRenderVertex = nullptr;
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
    m_texture = nullptr;
}

void SVBitFontNode::update(f32 dt) {
    SVNode::update(dt);
    if(m_fontDirty){
        m_fontDirty = false;
        _refresh();
    }
    if (m_pRenderObj && m_pMesh ) {
        //材质独立性
        SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d_c");
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        if (m_texture) {
            t_mtl->setTexture(0,m_texture);
        }
        t_mtl->setModelMatrix(m_absolutMat.get());
        t_mtl->setTexcoordFlip(1.0, -1.0f);
        t_mtl->update(dt);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(t_mtl);
    }
}


void SVBitFontNode::setTexture(SVTexturePtr _tex){
    if (_tex) {
        m_texture = _tex;
    }
}

void SVBitFontNode::setTexture(cptr8 _path){
    if (m_texture) {
        if (strcmp(m_texture->getname(), _path) != 0) {
            m_texture = nullptr;
        }
    }
    if (m_texture == nullptr) {
        m_texture = mApp->getTexMgr()->getTextureSync(_path,true);
    }
}

cptr8 SVBitFontNode::getTexturePath(){
    if (m_texture) {
        return m_texture->getname();
    }
    return "";
}

void SVBitFontNode::setAlpha(s32 _alpha){
    if (_alpha < 0 || _alpha > 1) {
        return;
    }
    if (m_alpha != _alpha) {
        m_fontDirty = true;
        m_alpha = _alpha;
    }
}

void SVBitFontNode::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->m_pGlobalParam->m_curScene->getRenderRS();
        if (m_pRenderObj && m_pMesh) {
            m_pRenderObj->pushCmd(t_rs, RST_FREETYPE, "SVBitFontNode");
        }
    }
    SVNode::render();
}

void SVBitFontNode::setFontSize(f32 _w,f32 _h){
    m_FontWidth = _w;    //物理尺寸宽高
    m_FontHeight = _h;
    m_fontDirty = true;
}

f32 SVBitFontNode::getFontW(){
    return m_FontWidth;
}

f32 SVBitFontNode::getFontH(){
    return m_FontHeight;
}

void SVBitFontNode::setInnerOff(f32 _offXScale,f32 _offYScale){
    m_innerOffXScale = _offXScale;
    m_innerOffYScale = _offYScale;
    m_fontDirty = true;
}

f32 SVBitFontNode::getInnerOffXScale(){
    return m_innerOffXScale;
}

f32 SVBitFontNode::getInnerOffYScale(){
    return m_innerOffYScale;
}

void SVBitFontNode::setText(s32 _number) {
    setText(SVString::format("%d",_number).c_str());
}

void SVBitFontNode::setAtcPt(BITFONT_ATCH_PT _type){
    m_atchType = _type;
    m_fontDirty = true;
};

BITFONT_ATCH_PT SVBitFontNode::getAtcPt(){
    return m_atchType;
}

//重新构建mesh数据
void SVBitFontNode::setText(cptr8 _text) {
    if(m_Text!=_text){
        m_Text = _text;
        m_fontDirty = true;
    }
}

void SVBitFontNode::_refresh(){
    if (m_Text.size() > SV_BITFONT_MAX_NUM) {
        return;
    }
    if( m_texcoordsTbl.size()!=m_charTbl.size() ){
        return;
    }
    s32 t_Len = m_Text.size();
    f32 t_total_w = m_FontWidth*t_Len;
    f32 t_total_h = m_FontHeight;
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
    V2_C_T0 tVerts[SV_BITFONT_MAX_NUM * 6];
    //更新每个字符的的纹理坐标
    for (u32 i = 0; i < t_Len; ++i) {
        c8 t_char = m_Text[i];
        //查找字符以及纹理坐标
        for(u32 j = 0;j<m_charTbl.size();j++){
            if( m_charTbl[j] == t_char ){
                tVerts[i * 6 + 0].x = m_FontWidth*i - t_offx;
                tVerts[i * 6 + 0].y = -t_offy;
                tVerts[i * 6 + 0].t0x = m_texcoordsTbl[j].lb_x;
                tVerts[i * 6 + 0].t0y = m_texcoordsTbl[j].lb_y;
                tVerts[i * 6 + 0].r = 255;
                tVerts[i * 6 + 0].g = 255;
                tVerts[i * 6 + 0].b = 255;
                tVerts[i * 6 + 0].a = 255*m_alpha;
                //
                tVerts[i * 6 + 1].x = m_FontWidth*(i+1) - t_offx;
                tVerts[i * 6 + 1].y = -t_offy;
                tVerts[i * 6 + 1].t0x = m_texcoordsTbl[j].rb_x;
                tVerts[i * 6 + 1].t0y = m_texcoordsTbl[j].rb_y;
                tVerts[i * 6 + 1].r = 255;
                tVerts[i * 6 + 1].g = 255;
                tVerts[i * 6 + 1].b = 255;
                tVerts[i * 6 + 1].a = 255*m_alpha;
                //
                tVerts[i * 6 + 2].x = m_FontWidth*i - t_offx;
                tVerts[i * 6 + 2].y = m_FontHeight-t_offy;
                tVerts[i * 6 + 2].t0x = m_texcoordsTbl[j].lt_x;
                tVerts[i * 6 + 2].t0y = m_texcoordsTbl[j].lt_y;
                tVerts[i * 6 + 2].r = 255;
                tVerts[i * 6 + 2].g = 255;
                tVerts[i * 6 + 2].b = 255;
                tVerts[i * 6 + 2].a = 255*m_alpha;
                //
                tVerts[i * 6 + 3].x = m_FontWidth*i - t_offx;
                tVerts[i * 6 + 3].y = m_FontHeight-t_offy;
                tVerts[i * 6 + 3].t0x = m_texcoordsTbl[j].lt_x;
                tVerts[i * 6 + 3].t0y = m_texcoordsTbl[j].lt_y;
                tVerts[i * 6 + 3].r = 255;
                tVerts[i * 6 + 3].g = 255;
                tVerts[i * 6 + 3].b = 255;
                tVerts[i * 6 + 3].a = 255*m_alpha;
                //
                tVerts[i * 6 + 4].x = m_FontWidth*(i+1) - t_offx;
                tVerts[i * 6 + 4].y = -t_offy;
                tVerts[i * 6 + 4].t0x = m_texcoordsTbl[j].rb_x;
                tVerts[i * 6 + 4].t0y = m_texcoordsTbl[j].rb_y;
                tVerts[i * 6 + 4].r = 255;
                tVerts[i * 6 + 4].g = 255;
                tVerts[i * 6 + 4].b = 255;
                tVerts[i * 6 + 4].a = 255*m_alpha;
                //
                tVerts[i * 6 + 5].x = m_FontWidth*(i+1) - t_offx;
                tVerts[i * 6 + 5].y = m_FontHeight-t_offy;
                tVerts[i * 6 + 5].t0x = m_texcoordsTbl[j].rt_x;
                tVerts[i * 6 + 5].t0y = m_texcoordsTbl[j].rt_y;
                tVerts[i * 6 + 5].r = 255;
                tVerts[i * 6 + 5].g = 255;
                tVerts[i * 6 + 5].b = 255;
                tVerts[i * 6 + 5].a = 255*m_alpha;
                break;
            }
        }
    }
    //
    if (t_Len < SV_BITFONT_MAX_NUM) {
        for (s32 i = t_Len; i < SV_BITFONT_MAX_NUM; ++i) {
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
    m_pRenderVertex->writeData(&tVerts[0], sizeof(V2_C_T0) * SV_BITFONT_MAX_NUM * 6);
    m_pMesh->setVertexDataNum(SV_BITFONT_MAX_NUM * 6);
    m_pMesh->setVertexData(m_pRenderVertex);
    m_pMesh->setVertexType(E_VF_V2_C_T0);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    m_pMesh->createMesh();
}

void SVBitFontNode::_genMesh(){
    //构建Mehs
    //构建顶点坐标
    //字符大小
    f32 t_total_w = m_FontWidth*SV_BITFONT_MAX_NUM;
    f32 t_total_h = m_FontHeight;
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
    
    //
    V2_C_T0 m_Verts[SV_BITFONT_MAX_NUM * 6];
    for (u32 i = 0; i < SV_BITFONT_MAX_NUM; i++) {
        m_Verts[i * 6 + 0].x = m_FontWidth*i-t_offx;
        m_Verts[i * 6 + 0].y = -t_offy;
        m_Verts[i * 6 + 0].t0x = 0.0f;
        m_Verts[i * 6 + 0].t0y = 0.0f;
        m_Verts[i * 6 + 0].r = 255;
        m_Verts[i * 6 + 0].g = 255;
        m_Verts[i * 6 + 0].b = 255;
        m_Verts[i * 6 + 0].a = 255;
        
        m_Verts[i * 6 + 1].x = m_FontWidth*(i+1)-t_offx;
        m_Verts[i * 6 + 1].y = -t_offy;
        m_Verts[i * 6 + 1].t0x = 1.0f;
        m_Verts[i * 6 + 1].t0y = 0.0f;
        m_Verts[i * 6 + 1].r = 255;
        m_Verts[i * 6 + 1].g = 255;
        m_Verts[i * 6 + 1].b = 255;
        m_Verts[i * 6 + 1].a = 255;
        
        m_Verts[i * 6 + 2].x = m_FontWidth*i-t_offx;
        m_Verts[i * 6 + 2].y = m_FontHeight-t_offy;
        m_Verts[i * 6 + 2].t0x = 0.0f;
        m_Verts[i * 6 + 2].t0y = 1.0f;
        m_Verts[i * 6 + 2].r = 255;
        m_Verts[i * 6 + 2].g = 255;
        m_Verts[i * 6 + 2].b = 255;
        m_Verts[i * 6 + 2].a = 255;
        
        m_Verts[i * 6 + 3].x = m_FontWidth*i-t_offx;
        m_Verts[i * 6 + 3].y = m_FontHeight-t_offy;
        m_Verts[i * 6 + 3].t0x = 0.0f;
        m_Verts[i * 6 + 3].t0y = 1.0f;
        m_Verts[i * 6 + 3].r = 255;
        m_Verts[i * 6 + 3].g = 255;
        m_Verts[i * 6 + 3].b = 255;
        m_Verts[i * 6 + 3].a = 255;
        
        m_Verts[i * 6 + 4].x = m_FontWidth*(i+1)-t_offx;
        m_Verts[i * 6 + 4].y = -t_offy;
        m_Verts[i * 6 + 4].t0x = 1.0f;
        m_Verts[i * 6 + 4].t0y = 0.0f;
        m_Verts[i * 6 + 4].r = 255;
        m_Verts[i * 6 + 4].g = 255;
        m_Verts[i * 6 + 4].b = 255;
        m_Verts[i * 6 + 4].a = 255;
        
        m_Verts[i * 6 + 5].x = m_FontWidth*(i+1)-t_offx;
        m_Verts[i * 6 + 5].y = m_FontHeight-t_offy;
        m_Verts[i * 6 + 5].t0x = 1.0f;
        m_Verts[i * 6 + 5].t0y = 1.0f;
        m_Verts[i * 6 + 5].r = 255;
        m_Verts[i * 6 + 5].g = 255;
        m_Verts[i * 6 + 5].b = 255;
        m_Verts[i * 6 + 5].a = 255;
    }
    //
    m_pRenderVertex->writeData(&m_Verts[0], sizeof(V2_C_T0) * SV_BITFONT_MAX_NUM * 6);
    m_pMesh->setVertexDataNum(SV_BITFONT_MAX_NUM * 6);
    m_pMesh->setVertexData(m_pRenderVertex);
    m_pMesh->setVertexType(E_VF_V2_C_T0);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    //
    m_pMesh->createMesh();
}

void SVBitFontNode::_genTexcoords(){
    //构建字符表
    m_charTbl.clear();
    m_texcoordsTbl.clear();
    //
    m_charTbl.append('0');
    m_charTbl.append('1');
    m_charTbl.append('2');
    m_charTbl.append('3');
    m_charTbl.append('4');
    m_charTbl.append('5');
    m_charTbl.append('6');
    m_charTbl.append('7');
    m_charTbl.append('8');
    m_charTbl.append('9');
    m_charTbl.append(':');
    m_charTbl.append('.');
    m_charTbl.append('+');
    m_charTbl.append('-');
    m_charTbl.append('x');
    m_charTbl.append('%');
    //构建字符表 对应的纹理坐标
    s32 tCharNum = m_charTbl.size();
    //从图片计算的字符大小
    f32 tHalfTexCharW = 0.5f/tCharNum;
    f32 tHalfTexCharH = 0.5f;
    for(u32 i=0;i<tCharNum;i++){
        FontTexcoords t_texcoord;
        f32 t_standard_x = tHalfTexCharW + 2.0f*tHalfTexCharW*i;
        f32 t_standard_y = tHalfTexCharH;
        //
        t_texcoord.lt_x = t_standard_x - tHalfTexCharW + tHalfTexCharW*m_innerOffXScale;
        t_texcoord.lt_y = t_standard_y + tHalfTexCharH - tHalfTexCharH*m_innerOffYScale;
        //
        t_texcoord.rt_x = t_standard_x + tHalfTexCharW - tHalfTexCharW*m_innerOffXScale;
        t_texcoord.rt_y = t_standard_y + tHalfTexCharH - tHalfTexCharH*m_innerOffYScale;
        //
        t_texcoord.lb_x = t_standard_x - tHalfTexCharW + tHalfTexCharW*m_innerOffXScale;
        t_texcoord.lb_y = t_standard_y - tHalfTexCharH + tHalfTexCharH*m_innerOffYScale;
        //
        t_texcoord.rb_x = t_standard_x + tHalfTexCharW - tHalfTexCharW*m_innerOffXScale;
        t_texcoord.rb_y = t_standard_y - tHalfTexCharH + tHalfTexCharH*m_innerOffYScale;
        //
        m_texcoordsTbl.append(t_texcoord);
    }
}

cptr8 SVBitFontNode::getText(){
    return m_Text.c_str();
}
