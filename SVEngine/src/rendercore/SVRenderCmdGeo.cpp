//
// SVRenderCmdGeo.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderCmdGeo.h"
#include "SVGL/SVRResGL.h"
#include "../base/SVDataSwap.h"
#include "../core/SVVertDef.h"
#include "../mtl/SVMtlCore.h"

SVRenderCmdGeo::SVRenderCmdGeo()
:m_pMtl(nullptr){
}

SVRenderCmdGeo::~SVRenderCmdGeo() {
    m_pMtl = nullptr;
}

void SVRenderCmdGeo::render(){
    if (m_pMtl ) {
        if (m_pMtl->submitMtl()) {
           _render();
        }
    }
}

void SVRenderCmdGeo::_render(){
}

void SVRenderCmdGeo::setMtl(SVMtlCorePtr _mtl){
    m_pMtl = _mtl;
}

//
void SVRenderCmdLine::setLine(FVec3& _start, FVec3& _end){
    m_start = _start;
    m_end = _end;
}

void SVRenderCmdLine::_render(){
    V3 t_verts[2];
    t_verts[0].x = m_start.x;
    t_verts[0].y = m_start.y;
    t_verts[0].z = m_start.z;
    t_verts[1].x = m_end.x;
    t_verts[1].y = m_end.y;
    t_verts[1].z = m_end.z;
    //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(CHANNEL_POSITION);
    glVertexAttribPointer(CHANNEL_POSITION, 3, GL_FLOAT, false, 0, t_verts);
    glDrawArrays(GL_LINES,0,2);
    glDisableVertexAttribArray(CHANNEL_POSITION);
}

//
void SVRenderCmdRect::setRect(SVRect& _rect){
    m_rect = _rect;
}

void SVRenderCmdRect::_render(){
    V3 t_verts[4];
    t_verts[0].x = m_rect.m_lb_pt.x;
    t_verts[0].y = m_rect.m_lb_pt.y;
    t_verts[0].z = 0.0f;
    t_verts[1].x = m_rect.m_rt_pt.x;
    t_verts[1].y = m_rect.m_lb_pt.y;
    t_verts[1].z = 0.0f;
    t_verts[2].x = m_rect.m_rt_pt.x;
    t_verts[2].y = m_rect.m_rt_pt.y;
    t_verts[2].z = 0.0f;
    t_verts[3].x = m_rect.m_lb_pt.x;
    t_verts[3].y = m_rect.m_rt_pt.y;
    t_verts[3].z = 0.0f;
    //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(CHANNEL_POSITION);
    glVertexAttribPointer(CHANNEL_POSITION, 3, GL_FLOAT, false, 0, t_verts);
    glDrawArrays(GL_LINE_LOOP,0,4);
}


void SVRenderCmdCurve::_render(){
//    V3 t_verts[4];
//    t_verts[0].x = m_rect.m_lb_pt.X;
//    t_verts[0].y = m_rect.m_lb_pt.Y;
//    t_verts[0].z = 0.0f;
//    t_verts[1].x = m_rect.m_rt_pt.X;
//    t_verts[1].y = m_rect.m_lb_pt.Y;
//    t_verts[1].z = 0.0f;
//    t_verts[2].x = m_rect.m_rt_pt.X;
//    t_verts[2].y = m_rect.m_rt_pt.Y;
//    t_verts[2].z = 0.0f;
//    t_verts[3].x = m_rect.m_lb_pt.X;
//    t_verts[3].y = m_rect.m_rt_pt.Y;
//    t_verts[3].z = 0.0f;
//    //
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glEnableVertexAttribArray(CHANNEL_POSITION);
//    glVertexAttribPointer(CHANNEL_POSITION, 3, GL_FLOAT, false, 0, t_verts);
//    glDrawArrays(GL_LINE_LOOP,0,4);
}

void SVRenderCmdCircle::_render(){
    //    V3 t_verts[4];
    //    t_verts[0].x = m_rect.m_lb_pt.X;
    //    t_verts[0].y = m_rect.m_lb_pt.Y;
    //    t_verts[0].z = 0.0f;
    //    t_verts[1].x = m_rect.m_rt_pt.X;
    //    t_verts[1].y = m_rect.m_lb_pt.Y;
    //    t_verts[1].z = 0.0f;
    //    t_verts[2].x = m_rect.m_rt_pt.X;
    //    t_verts[2].y = m_rect.m_rt_pt.Y;
    //    t_verts[2].z = 0.0f;
    //    t_verts[3].x = m_rect.m_lb_pt.X;
    //    t_verts[3].y = m_rect.m_rt_pt.Y;
    //    t_verts[3].z = 0.0f;
    //    //
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    glEnableVertexAttribArray(CHANNEL_POSITION);
    //    glVertexAttribPointer(CHANNEL_POSITION, 3, GL_FLOAT, false, 0, t_verts);
    //    glDrawArrays(GL_LINE_LOOP,0,4);
}

void SVRenderCmdCircleSolid::_render(){
    //    V3 t_verts[4];
    //    t_verts[0].x = m_rect.m_lb_pt.X;
    //    t_verts[0].y = m_rect.m_lb_pt.Y;
    //    t_verts[0].z = 0.0f;
    //    t_verts[1].x = m_rect.m_rt_pt.X;
    //    t_verts[1].y = m_rect.m_lb_pt.Y;
    //    t_verts[1].z = 0.0f;
    //    t_verts[2].x = m_rect.m_rt_pt.X;
    //    t_verts[2].y = m_rect.m_rt_pt.Y;
    //    t_verts[2].z = 0.0f;
    //    t_verts[3].x = m_rect.m_lb_pt.X;
    //    t_verts[3].y = m_rect.m_rt_pt.Y;
    //    t_verts[3].z = 0.0f;
    //    //
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //    glEnableVertexAttribArray(CHANNEL_POSITION);
    //    glVertexAttribPointer(CHANNEL_POSITION, 3, GL_FLOAT, false, 0, t_verts);
    //    glDrawArrays(GL_LINE_LOOP,0,4);
}

//
void SVRenderCmdAABB::setAABB(SVBoundBox& _aabb){
    m_aabb = _aabb;
}

/*
 Edges are stored in this way:
 Hey, am I an ascii artist, or what? :) niko.
   /3--------/ 7
  / |       /  |
 /  |      /   |
 1---------5   |
 |  /2- - -|- -6
 | /       |  /
 |/        | /
 0---------4/
 */

void SVRenderCmdAABB::_render(){
    V3 t_verts[24];
    // 0-4
    t_verts[0].x = m_aabb.getMin().x;
    t_verts[0].y = m_aabb.getMin().y;
    t_verts[0].z = m_aabb.getMin().z;
    t_verts[1].x = m_aabb.getMax().x;
    t_verts[1].y = m_aabb.getMin().y;
    t_verts[1].z = m_aabb.getMin().z;
    //4-5
    t_verts[2].x = m_aabb.getMax().x;
    t_verts[2].y = m_aabb.getMin().y;
    t_verts[2].z = m_aabb.getMin().z;
    t_verts[3].x = m_aabb.getMax().x;
    t_verts[3].y = m_aabb.getMax().y;
    t_verts[3].z = m_aabb.getMin().z;
    //5-1
    t_verts[4].x = m_aabb.getMax().x;
    t_verts[4].y = m_aabb.getMax().y;
    t_verts[4].z = m_aabb.getMin().z;
    t_verts[5].x = m_aabb.getMin().x;
    t_verts[5].y = m_aabb.getMax().y;
    t_verts[5].z = m_aabb.getMin().z;
    //1-0
    t_verts[6].x = m_aabb.getMin().x;
    t_verts[6].y = m_aabb.getMax().y;
    t_verts[6].z = m_aabb.getMin().z;
    t_verts[7].x = m_aabb.getMin().x;
    t_verts[7].y = m_aabb.getMin().y;
    t_verts[7].z = m_aabb.getMin().z;
    //4-6
    t_verts[8].x = m_aabb.getMax().x;
    t_verts[8].y = m_aabb.getMin().y;
    t_verts[8].z = m_aabb.getMin().z;
    t_verts[9].x = m_aabb.getMax().x;
    t_verts[9].y = m_aabb.getMin().y;
    t_verts[9].z = m_aabb.getMax().z;
    //6-7
    t_verts[10].x = m_aabb.getMax().x;
    t_verts[10].y = m_aabb.getMin().y;
    t_verts[10].z = m_aabb.getMax().z;
    t_verts[11].x = m_aabb.getMax().x;
    t_verts[11].y = m_aabb.getMax().y;
    t_verts[11].z = m_aabb.getMax().z;
    //7-5
    t_verts[12].x = m_aabb.getMax().x;
    t_verts[12].y = m_aabb.getMax().y;
    t_verts[12].z = m_aabb.getMax().z;
    t_verts[13].x = m_aabb.getMax().x;
    t_verts[13].y = m_aabb.getMax().y;
    t_verts[13].z = m_aabb.getMin().z;
    //7-3z
    t_verts[14].x = m_aabb.getMax().x;
    t_verts[14].y = m_aabb.getMax().y;
    t_verts[14].z = m_aabb.getMax().z;
    t_verts[15].x = m_aabb.getMin().x;
    t_verts[15].y = m_aabb.getMax().y;
    t_verts[15].z = m_aabb.getMax().z;
    //3-2
    t_verts[16].x = m_aabb.getMin().x;
    t_verts[16].y = m_aabb.getMax().y;
    t_verts[16].z = m_aabb.getMax().z;
    t_verts[17].x = m_aabb.getMin().x;
    t_verts[17].y = m_aabb.getMin().y;
    t_verts[17].z = m_aabb.getMax().z;
    //2-6
    t_verts[18].x = m_aabb.getMin().x;
    t_verts[18].y = m_aabb.getMin().y;
    t_verts[18].z = m_aabb.getMax().z;
    t_verts[19].x = m_aabb.getMax().x;
    t_verts[19].y = m_aabb.getMin().y;
    t_verts[19].z = m_aabb.getMax().z;
    //3-1
    t_verts[20].x = m_aabb.getMin().x;
    t_verts[20].y = m_aabb.getMax().y;
    t_verts[20].z = m_aabb.getMax().z;
    t_verts[21].x = m_aabb.getMin().x;
    t_verts[21].y = m_aabb.getMax().y;
    t_verts[21].z = m_aabb.getMin().z;
    //0-2
    t_verts[22].x = m_aabb.getMin().x;
    t_verts[22].y = m_aabb.getMin().y;
    t_verts[22].z = m_aabb.getMin().z;
    t_verts[23].x = m_aabb.getMin().x;
    t_verts[23].y = m_aabb.getMin().y;
    t_verts[23].z = m_aabb.getMax().z;
    //
    glDisableVertexAttribArray(CHANNEL_COLOR0);
    glDisableVertexAttribArray(CHANNEL_TEXCOORD0);
    //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(CHANNEL_POSITION);
    glVertexAttribPointer(CHANNEL_POSITION, 3, GL_FLOAT, false, 0, &t_verts[0]);
    glDrawArrays(GL_LINES,0,24);
    glDisableVertexAttribArray(CHANNEL_POSITION);
}

