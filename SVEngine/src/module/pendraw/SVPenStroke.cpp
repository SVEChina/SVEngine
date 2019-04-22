//
// SVPenStroke.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPenStroke.h"
#include "SVPenCurve.h"
#include "../SVGameReady.h"
#include "../SVGameRun.h"
#include "../SVGameEnd.h"
#include "../../core/SVVertDef.h"
#include "../../base/SVVec3.h"
#include "../../base/SVDataSwap.h"
#include "../../rendercore/SVRenderObject.h"
#include "../../rendercore/SVRenderMesh.h"
#include "../../app/SVInst.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../../mtl/SVMtlCore.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../basesys/SVCameraMgr.h"
#include "../../node/SVCameraNode.h"
#include "../../basesys/SVBasicSys.h"
#include "../../basesys/SVPickProcess.h"
#include "../../basesys/SVSensorProcess.h"
#include "../../mtl/SVMtlStrokeBase.h"
#include "../../mtl/SVMtlNocolor.h"

SVPenStroke::SVPenStroke(SVInst *_app)
:SVGameBase(_app) {
//    m_penCurve = MakeSharedPtr<SVPenCurve>(_app);
    m_ptPool.clear();
    m_localMat.setIdentity();
    m_lock = MakeSharedPtr<SVLock>();
    m_pVertData = MakeSharedPtr<SVDataSwap>();
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMesh = _app->getRenderMgr()->createMeshRObj();
    m_pMesh->createMesh();
    m_pMesh->setVertexType(E_VF_V3_C_T0);
    m_pMesh->setDrawMethod(E_DM_POINTS);
    m_pMesh->setDrawMethod(E_DM_LINES);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    m_pTex = mApp->getTexMgr()->getTexture("svres/textures/a_line.png",true);
    m_density = 0.05;
    m_vertexNum = 0;
    m_drawBox = false;
    m_isFirstTouch = true;
    setDrawBox(true);
    m_point_dis_dert = 0.002f;
    m_pen_width = 0.006f;
    m_plane_dis = 0.2f;
}

SVPenStroke::~SVPenStroke() {
    m_penCurve = nullptr;
    m_pVertData->reback();
    m_pVertData = nullptr;
    m_pTex = nullptr;
    m_lock = nullptr;
    m_ptPool.clear();
    m_aabbBox.clear();
}

void SVPenStroke::setStrokeWidth(f32 _width){
    m_pen_width = _width;
}

void SVPenStroke::setDrawBox(bool _drawBox){
    m_drawBox = _drawBox;
}

//绘制一笔
void SVPenStroke::update(f32 _dt) {
    m_lock->unlock();
    //转化成世界坐标系下的坐标点
    _genPolygon();
    //插值生成面片
    _genMesh();
    //绘制dataswap
    _drawMesh();
    m_lock->unlock();
}

void SVPenStroke::begin(f32 _px,f32 _py,f32 _pz) {
    m_lock->lock();
    m_isFirstTouch = true;
    //二维点到三维点的转换
    FVec2 t_pt = FVec2(_px, _py);
    SVStrokePoint t_worldPt;
    _screenPointToWorld(t_pt, t_worldPt);
    m_ptPool.append(t_worldPt);
    m_lock->unlock();
}

void SVPenStroke::end(f32 _px,f32 _py,f32 _pz) {
    m_lock->lock();
    //二维点到三维点的转换
    FVec2 t_pt = FVec2(_px, _py);
    SVStrokePoint t_worldPt;
    _screenPointToWorld(t_pt, t_worldPt);
    //
    s32 t_pt_num = m_ptPool.size();
    SVStrokePoint t_lastpt = m_ptPool[t_pt_num-1];
    if(length(t_lastpt.point - t_worldPt.point) > m_point_dis_dert ) {
        m_ptPool.append(t_worldPt);
    }
    //
    m_lock->unlock();
}

void SVPenStroke::draw(f32 _px,f32 _py,f32 _pz) {
    m_lock->lock();
    //二维点到三维点的转换
    FVec2 t_pt = FVec2(_px, _py);
    SVStrokePoint t_worldPt;
    _screenPointToWorld(t_pt, t_worldPt);
    //
    s32 t_pt_num = m_ptPool.size();
    SVStrokePoint t_lastpt = m_ptPool[t_pt_num-1];
    if(length(t_lastpt.point - t_worldPt.point) > m_point_dis_dert ) {
        m_ptPool.append(t_worldPt);
    }
    //
    m_lock->unlock();
}

//生成mesh
void SVPenStroke::_genPolygon(){
    return ;
    s32 t_pt_num = m_ptPool.size();
    if(t_pt_num<2)
        return ;
    for (s32 i = 0; i<t_pt_num; i++) {
        if(i == 0) {
            //第一个点
            SVStrokePoint pt0 = m_ptPool[i];
            SVStrokePoint pt1 = m_ptPool[i+1];
            FVec3 edage = pt1.point - pt0.point;
            edage.normalize();
            //求出轴
            FVec3 t_axis;
            cross(t_axis, pt0.normal,edage);
            t_axis.normalize();
            m_ptPool[i].ext0 = pt0.point + t_axis*m_pen_width;
            m_ptPool[i].ext1 = pt0.point - t_axis*m_pen_width;
        }else if(i == t_pt_num-1) {
            //最后一个点
            SVStrokePoint pt0 = m_ptPool[i-1];
            SVStrokePoint pt1 = m_ptPool[i];
            FVec3 edage = pt1.point - pt0.point;
            edage.normalize();
            //求出轴
            FVec3 t_axis;
            cross(t_axis, pt1.normal,edage);
            t_axis.normalize();
            m_ptPool[i].ext0 = pt0.point + t_axis*m_pen_width;
            m_ptPool[i].ext1 = pt0.point - t_axis*m_pen_width;
        }else{
            //中间点
            SVStrokePoint pt0 = m_ptPool[i-1];
            SVStrokePoint pt1 = m_ptPool[i];
            SVStrokePoint pt2 = m_ptPool[i+1];
            //
            FVec3 edage = pt2.point - pt0.point;
            edage.normalize();
            //求出轴
            FVec3 t_axis;
            cross(t_axis, pt1.normal,edage);
            t_axis.normalize();
            m_ptPool[i].ext0 = pt0.point + t_axis*m_pen_width;
            m_ptPool[i].ext1 = pt0.point - t_axis*m_pen_width;
        }
    }
}

//生成数据
void SVPenStroke::_genMesh() {
    s32 t_pt_num = m_ptPool.size();
    m_pVertData->resize(t_pt_num*36*sizeof(V3_C_T0));
    for (s32 i = 0; i<t_pt_num; i++) {
        SVStrokePoint t_pt = m_ptPool[i];
        _genBox(t_pt.point);
    }
    m_vertexNum = t_pt_num*36;//6个面 一个面6个点
}

void SVPenStroke::_genBox(FVec3& _pt) {
    V3_C_T0 t_ver[36];
//    m_pVertData->appendData(t_ver, 24*sizeof(V3_C_T0));
//    return ;
    f32 t_half_w = m_pen_width*0.5f;
    FVec3 t_pt0 = _pt + FVec3(-1.0f,-1.0f,-1.0f)*t_half_w;
    FVec3 t_pt1 = _pt + FVec3(1.0f,-1.0f,-1.0f)*t_half_w;
    FVec3 t_pt2 = _pt + FVec3(1.0f,-1.0f,1.0f)*t_half_w;
    FVec3 t_pt3 = _pt + FVec3(-1.0f,-1.0f,1.0f)*t_half_w;
    FVec3 t_pt4 = _pt + FVec3(-1.0f,1.0f,-1.0f)*t_half_w;
    FVec3 t_pt5 = _pt + FVec3(1.0f,1.0f,-1.0f)*t_half_w;
    FVec3 t_pt6 = _pt + FVec3(1.0f,1.0f,1.0f)*t_half_w;
    FVec3 t_pt7 = _pt + FVec3(-1.0f,1.0f,1.0f)*t_half_w;
    FVec2 t_t0 = FVec2(0.0f,0.0f);
    FVec2 t_t1 = FVec2(1.0f,0.0f);
    FVec2 t_t2 = FVec2(0.0f,1.0f);
    FVec2 t_t3 = FVec2(1.0f,1.0f);
    u8 t_r = 255;
    u8 t_g = 0;
    u8 t_b = 255;
    u8 t_a = 255;
    //0,1,4,5
    t_ver[0].x = t_pt0.x;
    t_ver[0].y = t_pt0.y;
    t_ver[0].z = t_pt0.z;
    t_ver[0].t0x = t_t0.x;
    t_ver[0].t0y = t_t0.y;
    t_ver[0].r = t_r;
    t_ver[0].g = t_g;
    t_ver[0].b = t_b;
    t_ver[0].a = t_a;
    t_ver[1].x = t_pt1.x;
    t_ver[1].y = t_pt1.y;
    t_ver[1].z = t_pt1.z;
    t_ver[1].t0x = t_t1.x;
    t_ver[1].t0y = t_t1.y;
    t_ver[1].r = t_r;
    t_ver[1].g = t_g;
    t_ver[1].b = t_b;
    t_ver[1].a = t_a;
    t_ver[2].x = t_pt4.x;
    t_ver[2].y = t_pt4.y;
    t_ver[2].z = t_pt4.z;
    t_ver[2].t0x = t_t2.x;
    t_ver[2].t0y = t_t2.y;
    t_ver[2].r = t_r;
    t_ver[2].g = t_g;
    t_ver[2].b = t_b;
    t_ver[2].a = t_a;
    t_ver[3].x = t_pt1.x;
    t_ver[3].y = t_pt1.y;
    t_ver[3].z = t_pt1.z;
    t_ver[3].t0x = t_t1.x;
    t_ver[3].t0y = t_t1.y;
    t_ver[3].r = t_r;
    t_ver[3].g = t_g;
    t_ver[3].b = t_b;
    t_ver[3].a = t_a;
    t_ver[4].x = t_pt4.x;
    t_ver[4].y = t_pt4.y;
    t_ver[4].z = t_pt4.z;
    t_ver[4].t0x = t_t2.x;
    t_ver[4].t0y = t_t2.y;
    t_ver[4].r = t_r;
    t_ver[4].g = t_g;
    t_ver[4].b = t_b;
    t_ver[4].a = t_a;
    t_ver[5].x = t_pt5.x;
    t_ver[5].y = t_pt5.y;
    t_ver[5].z = t_pt5.z;
    t_ver[5].t0x = t_t3.x;
    t_ver[5].t0y = t_t3.y;
    t_ver[5].r = t_r;
    t_ver[5].g = t_g;
    t_ver[5].b = t_b;
    t_ver[5].a = t_a;
    //1,2,5,6
    s32 i=1;
    t_ver[0+6*i].x = t_pt1.x;
    t_ver[0+6*i].y = t_pt1.y;
    t_ver[0+6*i].z = t_pt1.z;
    t_ver[0+6*i].t0x = t_t0.x;
    t_ver[0+6*i].t0y = t_t0.y;
    t_ver[0+6*i].r = t_r;
    t_ver[0+6*i].g = t_g;
    t_ver[0+6*i].b = t_b;
    t_ver[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt2.x;
    t_ver[1+6*i].y = t_pt2.y;
    t_ver[1+6*i].z = t_pt2.z;
    t_ver[1+6*i].t0x = t_t1.x;
    t_ver[1+6*i].t0y = t_t1.y;
    t_ver[1+6*i].r = t_r;
    t_ver[1+6*i].g = t_g;
    t_ver[1+6*i].b = t_b;
    t_ver[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt5.x;
    t_ver[2+6*i].y = t_pt5.y;
    t_ver[2+6*i].z = t_pt5.z;
    t_ver[2+6*i].t0x = t_t2.x;
    t_ver[2+6*i].t0y = t_t2.y;
    t_ver[2+6*i].r = t_r;
    t_ver[2+6*i].g = t_g;
    t_ver[2+6*i].b = t_b;
    t_ver[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt2.x;
    t_ver[3+6*i].y = t_pt2.y;
    t_ver[3+6*i].z = t_pt2.z;
    t_ver[3+6*i].t0x = t_t1.x;
    t_ver[3+6*i].t0y = t_t1.y;
    t_ver[3+6*i].r = t_r;
    t_ver[3+6*i].g = t_g;
    t_ver[3+6*i].b = t_b;
    t_ver[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt5.x;
    t_ver[4+6*i].y = t_pt5.y;
    t_ver[4+6*i].z = t_pt5.z;
    t_ver[4+6*i].t0x = t_t2.x;
    t_ver[4+6*i].t0y = t_t2.y;
    t_ver[4+6*i].r = t_r;
    t_ver[4+6*i].g = t_g;
    t_ver[4+6*i].b = t_b;
    t_ver[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt6.x;
    t_ver[5+6*i].y = t_pt6.y;
    t_ver[5+6*i].z = t_pt6.z;
    t_ver[5+6*i].t0x = t_t3.x;
    t_ver[5+6*i].t0y = t_t3.y;
    t_ver[5+6*i].r = t_r;
    t_ver[5+6*i].g = t_g;
    t_ver[5+6*i].b = t_b;
    t_ver[5+6*i].a = t_a;
    //2,3,6,7
    i=2;
    t_ver[0+6*i].x = t_pt2.x;
    t_ver[0+6*i].y = t_pt2.y;
    t_ver[0+6*i].z = t_pt2.z;
    t_ver[0+6*i].t0x = t_t0.x;
    t_ver[0+6*i].t0y = t_t0.y;
    t_ver[0+6*i].r = t_r;
    t_ver[0+6*i].g = t_g;
    t_ver[0+6*i].b = t_b;
    t_ver[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt3.x;
    t_ver[1+6*i].y = t_pt3.y;
    t_ver[1+6*i].z = t_pt3.z;
    t_ver[1+6*i].t0x = t_t1.x;
    t_ver[1+6*i].t0y = t_t1.y;
    t_ver[1+6*i].r = t_r;
    t_ver[1+6*i].g = t_g;
    t_ver[1+6*i].b = t_b;
    t_ver[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt6.x;
    t_ver[2+6*i].y = t_pt6.y;
    t_ver[2+6*i].z = t_pt6.z;
    t_ver[2+6*i].t0x = t_t2.x;
    t_ver[2+6*i].t0y = t_t2.y;
    t_ver[2+6*i].r = t_r;
    t_ver[2+6*i].g = t_g;
    t_ver[2+6*i].b = t_b;
    t_ver[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt3.x;
    t_ver[3+6*i].y = t_pt3.y;
    t_ver[3+6*i].z = t_pt3.z;
    t_ver[3+6*i].t0x = t_t1.x;
    t_ver[3+6*i].t0y = t_t1.y;
    t_ver[3+6*i].r = t_r;
    t_ver[3+6*i].g = t_g;
    t_ver[3+6*i].b = t_b;
    t_ver[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt6.x;
    t_ver[4+6*i].y = t_pt6.y;
    t_ver[4+6*i].z = t_pt6.z;
    t_ver[4+6*i].t0x = t_t2.x;
    t_ver[4+6*i].t0y = t_t2.y;
    t_ver[4+6*i].r = t_r;
    t_ver[4+6*i].g = t_g;
    t_ver[4+6*i].b = t_b;
    t_ver[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt7.x;
    t_ver[5+6*i].y = t_pt7.y;
    t_ver[5+6*i].z = t_pt7.z;
    t_ver[5+6*i].t0x = t_t3.x;
    t_ver[5+6*i].t0y = t_t3.y;
    t_ver[5+6*i].r = t_r;
    t_ver[5+6*i].g = t_g;
    t_ver[5+6*i].b = t_b;
    t_ver[5+6*i].a = t_a;
    //3,0,7,4
    i=3;
    t_ver[0+6*i].x = t_pt3.x;
    t_ver[0+6*i].y = t_pt3.y;
    t_ver[0+6*i].z = t_pt3.z;
    t_ver[0+6*i].t0x = t_t0.x;
    t_ver[0+6*i].t0y = t_t0.y;
    t_ver[0+6*i].r = t_r;
    t_ver[0+6*i].g = t_g;
    t_ver[0+6*i].b = t_b;
    t_ver[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt0.x;
    t_ver[1+6*i].y = t_pt0.y;
    t_ver[1+6*i].z = t_pt0.z;
    t_ver[1+6*i].t0x = t_t1.x;
    t_ver[1+6*i].t0y = t_t1.y;
    t_ver[1+6*i].r = t_r;
    t_ver[1+6*i].g = t_g;
    t_ver[1+6*i].b = t_b;
    t_ver[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt7.x;
    t_ver[2+6*i].y = t_pt7.y;
    t_ver[2+6*i].z = t_pt7.z;
    t_ver[2+6*i].t0x = t_t2.x;
    t_ver[2+6*i].t0y = t_t2.y;
    t_ver[2+6*i].r = t_r;
    t_ver[2+6*i].g = t_g;
    t_ver[2+6*i].b = t_b;
    t_ver[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt0.x;
    t_ver[3+6*i].y = t_pt0.y;
    t_ver[3+6*i].z = t_pt0.z;
    t_ver[3+6*i].t0x = t_t1.x;
    t_ver[3+6*i].t0y = t_t1.y;
    t_ver[3+6*i].r = t_r;
    t_ver[3+6*i].g = t_g;
    t_ver[3+6*i].b = t_b;
    t_ver[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt7.x;
    t_ver[4+6*i].y = t_pt7.y;
    t_ver[4+6*i].z = t_pt7.z;
    t_ver[4+6*i].t0x = t_t2.x;
    t_ver[4+6*i].t0y = t_t2.y;
    t_ver[4+6*i].r = t_r;
    t_ver[4+6*i].g = t_g;
    t_ver[4+6*i].b = t_b;
    t_ver[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt4.x;
    t_ver[5+6*i].y = t_pt4.y;
    t_ver[5+6*i].z = t_pt4.z;
    t_ver[5+6*i].t0x = t_t3.x;
    t_ver[5+6*i].t0y = t_t3.y;
    t_ver[5+6*i].r = t_r;
    t_ver[5+6*i].g = t_g;
    t_ver[5+6*i].b = t_b;
    t_ver[5+6*i].a = t_a;
    //3,2,0,1
    i=4;
    t_ver[0+6*i].x = t_pt3.x;
    t_ver[0+6*i].y = t_pt3.y;
    t_ver[0+6*i].z = t_pt3.z;
    t_ver[0+6*i].t0x = t_t0.x;
    t_ver[0+6*i].t0y = t_t0.y;
    t_ver[0+6*i].r = t_r;
    t_ver[0+6*i].g = t_g;
    t_ver[0+6*i].b = t_b;
    t_ver[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt2.x;
    t_ver[1+6*i].y = t_pt2.y;
    t_ver[1+6*i].z = t_pt2.z;
    t_ver[1+6*i].t0x = t_t1.x;
    t_ver[1+6*i].t0y = t_t1.y;
    t_ver[1+6*i].r = t_r;
    t_ver[1+6*i].g = t_g;
    t_ver[1+6*i].b = t_b;
    t_ver[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt0.x;
    t_ver[2+6*i].y = t_pt0.y;
    t_ver[2+6*i].z = t_pt0.z;
    t_ver[2+6*i].t0x = t_t2.x;
    t_ver[2+6*i].t0y = t_t2.y;
    t_ver[2+6*i].r = t_r;
    t_ver[2+6*i].g = t_g;
    t_ver[2+6*i].b = t_b;
    t_ver[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt2.x;
    t_ver[3+6*i].y = t_pt2.y;
    t_ver[3+6*i].z = t_pt2.z;
    t_ver[3+6*i].t0x = t_t1.x;
    t_ver[3+6*i].t0y = t_t1.y;
    t_ver[3+6*i].r = t_r;
    t_ver[3+6*i].g = t_g;
    t_ver[3+6*i].b = t_b;
    t_ver[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt0.x;
    t_ver[4+6*i].y = t_pt0.y;
    t_ver[4+6*i].z = t_pt0.z;
    t_ver[4+6*i].t0x = t_t2.x;
    t_ver[4+6*i].t0y = t_t2.y;
    t_ver[4+6*i].r = t_r;
    t_ver[4+6*i].g = t_g;
    t_ver[4+6*i].b = t_b;
    t_ver[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt1.x;
    t_ver[5+6*i].y = t_pt1.y;
    t_ver[5+6*i].z = t_pt1.z;
    t_ver[5+6*i].t0x = t_t3.x;
    t_ver[5+6*i].t0y = t_t3.y;
    t_ver[5+6*i].r = t_r;
    t_ver[5+6*i].g = t_g;
    t_ver[5+6*i].b = t_b;
    t_ver[5+6*i].a = t_a;
    //4,5,7,6
    i=5;
    t_ver[0+6*i].x = t_pt4.x;
    t_ver[0+6*i].y = t_pt4.y;
    t_ver[0+6*i].z = t_pt4.z;
    t_ver[0+6*i].t0x = t_t0.x;
    t_ver[0+6*i].t0y = t_t0.y;
    t_ver[0+6*i].r = t_r;
    t_ver[0+6*i].g = t_g;
    t_ver[0+6*i].b = t_b;
    t_ver[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt5.x;
    t_ver[1+6*i].y = t_pt5.y;
    t_ver[1+6*i].z = t_pt5.z;
    t_ver[1+6*i].t0x = t_t1.x;
    t_ver[1+6*i].t0y = t_t1.y;
    t_ver[1+6*i].r = t_r;
    t_ver[1+6*i].g = t_g;
    t_ver[1+6*i].b = t_b;
    t_ver[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt7.x;
    t_ver[2+6*i].y = t_pt7.y;
    t_ver[2+6*i].z = t_pt7.z;
    t_ver[2+6*i].t0x = t_t2.x;
    t_ver[2+6*i].t0y = t_t2.y;
    t_ver[2+6*i].r = t_r;
    t_ver[2+6*i].g = t_g;
    t_ver[2+6*i].b = t_b;
    t_ver[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt5.x;
    t_ver[3+6*i].y = t_pt5.y;
    t_ver[3+6*i].z = t_pt5.z;
    t_ver[3+6*i].t0x = t_t1.x;
    t_ver[3+6*i].t0y = t_t1.y;
    t_ver[3+6*i].r = t_r;
    t_ver[3+6*i].g = t_g;
    t_ver[3+6*i].b = t_b;
    t_ver[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt7.x;
    t_ver[4+6*i].y = t_pt7.y;
    t_ver[4+6*i].z = t_pt7.z;
    t_ver[4+6*i].t0x = t_t2.x;
    t_ver[4+6*i].t0y = t_t2.y;
    t_ver[4+6*i].r = t_r;
    t_ver[4+6*i].g = t_g;
    t_ver[4+6*i].b = t_b;
    t_ver[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt6.x;
    t_ver[5+6*i].y = t_pt6.y;
    t_ver[5+6*i].z = t_pt6.z;
    t_ver[5+6*i].t0x = t_t3.x;
    t_ver[5+6*i].t0y = t_t3.y;
    t_ver[5+6*i].r = t_r;
    t_ver[5+6*i].g = t_g;
    t_ver[5+6*i].b = t_b;
    t_ver[5+6*i].a = t_a;
    //
    m_pVertData->appendData(t_ver, 36*sizeof(V3_C_T0));
}

void SVPenStroke::_drawMesh() {
    if (m_pMesh && m_pRenderObj && m_vertexNum > 0) {
        if (!m_pMtl) {
            m_pMtl = MakeSharedPtr<SVMtlStrokeBase>(mApp);
            m_pMtl->setTexture(0, m_pTex);
            m_pMtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
            m_pMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
             //void setTextureParam(s32 _chanel,TEXTUREPARAM _type,s32 _value);
            m_pMtl->setTexcoordFlip(1.0, -1.0);
            m_pMtl->setLineSize(5.0f);
        }
        m_pMtl->setBlendEnable(true);
        m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        m_pMtl->setCullEnable(false);
        m_pMtl->setModelMatrix(m_localMat);
        //更新顶点数据
        m_pMesh->setVertexDataNum(m_vertexNum);
        m_pMesh->setVertexData(m_pVertData);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(m_pMtl);
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        m_pRenderObj->pushCmd(t_rs, RST_AR, "SVPenStroke");
    }
}

void SVPenStroke::_screenPointToWorld(FVec2 &_point, SVStrokePoint &_worldPoint){
    SVSensorProcessPtr t_sensor = mApp->getBasicSys()->getSensorModule();
    SVCameraNodePtr t_arCam = t_sensor->getARCamera();
    if(!t_arCam)
        return ;
    FMat4 t_cameraMatrix = t_arCam->getViewMatObj();
    FMat4 t_camRotInver = t_cameraMatrix;
    t_camRotInver[12] = 0;
    t_camRotInver[13] = 0;
    t_camRotInver[14] = 0;
    t_camRotInver =transpose(t_camRotInver);
    FMat4 tmpMat = t_camRotInver*t_cameraMatrix;
    //获取相机世界位置
    FVec3 t_cameraEye = FVec3(-tmpMat[12], -tmpMat[13], -tmpMat[14]);
    //构建虚拟平面
    FVec3 t_cameraDir = FVec3(-t_cameraMatrix[2],
                              -t_cameraMatrix[6],
                              -t_cameraMatrix[10]);
    FVec3 t_targetPos = t_cameraEye + t_cameraDir*m_plane_dis;
    f32 t_dis = dot(t_targetPos,t_cameraDir);
    FVec4 t_plane = FVec4(-t_cameraDir,t_dis);
    SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
    //求交点
    FVec3 t_pos;
    f32 t_pt_x = _point.x;
    f32 t_pt_y = _point.y;
    f32 t_pt_z = 0.0f;
    if(t_pickModule && t_pickModule->getCrossPointWithPlane(t_arCam,t_pt_x, t_pt_y,t_pos, t_plane) ){
        t_pt_x = t_pos.x;
        t_pt_y = t_pos.y;
        t_pt_z = t_pos.z;
    }
    //保存交点的位置和法线方向
    _worldPoint.point = FVec3(t_pt_x,t_pt_y,t_pt_z);
    _worldPoint.normal = -t_cameraDir;
    //_worldPoint.normal.normalize();
    _worldPoint.ext0 = FVec3(0.0f,0.0f,0.0f);
    _worldPoint.ext1 = FVec3(0.0f,0.0f,0.0f);
}
