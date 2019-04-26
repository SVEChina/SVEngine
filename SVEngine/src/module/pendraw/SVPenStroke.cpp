//
// SVPenStroke.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPenStroke.h"
#include "../SVGameReady.h"
#include "../SVGameRun.h"
#include "../SVGameEnd.h"
#include "../../core/SVVertDef.h"
#include "../../core/SVGeoGen.h"
#include "../../core/SVPass.h"
#include "../../base/SVVec3.h"
#include "../../base/SVDataSwap.h"
#include "../../rendercore/SVRenderObject.h"
#include "../../rendercore/SVRenderMesh.h"
#include "../../app/SVInst.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../../mtl/SVMtlCore.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../mtl/SVMtlPenStrokeGlow.h"
#include "../../basesys/SVCameraMgr.h"
#include "../../node/SVCameraNode.h"
#include "../../basesys/SVBasicSys.h"
#include "../../basesys/SVPickProcess.h"
#include "../../basesys/SVSensorProcess.h"
#include "../../mtl/SVMtlStrokeBase.h"
#include "../../mtl/SVMtlNocolor.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../rendercore/SVRenderTexture.h"
#include "../../node/SVBillboardNode.h"
#include "../../node/SVSpriteNode.h"
#include "../../basesys/SVSceneMgr.h"
#include "../../node/SVScene.h"
SVPenStroke::SVPenStroke(SVInst *_app)
:SVGameBase(_app) {
    m_penCurve = MakeSharedPtr<SVPenCurve>(_app);
    m_ptPool.clear();
    m_localMat.setIdentity();
    m_lock = MakeSharedPtr<SVLock>();
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pInstanceOffsetData = MakeSharedPtr<SVDataSwap>();
    m_pTex = mApp->getTexMgr()->getTexture("svres/textures/a_point50.png",true);
    //
    m_pGlowInstanceOffsetData = MakeSharedPtr<SVDataSwap>();
    m_pGlowTex = mApp->getTexMgr()->getTexture("svres/textures/a_point50.png",true);
//    m_pGlowTex = mApp->getTexMgr()->getTexture("svres/HollowKnight.png",true);
    //
    m_lerpMethod = SV_LERP_BALANCE;
    m_drawBox = false;
    m_instanceCount = 0;
    m_lastInstanceIndex = 0;
    m_lastGlowInstanceIndex = 0;
    m_plane_dis = 0.2f;
    m_glowDensity = 0.2;
    m_glowStrokeWidth = 0.08f;
    m_density = 0.2;
    m_pen_width = 0.016f;
    m_glowColor.set(0, 255, 0, 200);
    m_strokeColor.set(255, 255, 255, 255);
    _createStrokeMesh();
    _createGlowMesh();
//    setDrawBox(true);
}

SVPenStroke::~SVPenStroke() {
    m_penCurve = nullptr;
    m_pInstanceOffsetData->reback();
    m_pInstanceOffsetData = nullptr;
    m_pGlowInstanceOffsetData->reback();
    m_pGlowInstanceOffsetData = nullptr;
    m_pRenderObj = nullptr;
    m_pBoxMesh = nullptr;
    m_pMtl = nullptr;
    m_pTex = nullptr;
    m_pGlowTex = nullptr;
    m_lock = nullptr;
    m_pGlowMesh = nullptr;
    m_pGlowMtl = nullptr;
    m_ptPool.clear();
    m_ptGlowPool.clear();
    m_aabbBox.clear();
    m_glowStrokes.destroy();
}

void SVPenStroke::setStrokeWidth(f32 _width){
    m_pen_width = _width;
}

void SVPenStroke::setStrokeColor(FVec4 &_color){
    m_strokeColor = _color;
}

void SVPenStroke::setDrawBox(bool _drawBox){
    m_drawBox = _drawBox;
}

//绘制一笔
void SVPenStroke::update(f32 _dt) {
    m_lock->unlock();
    //创建实例
    _genInstances();
    //绘制
    _drawMesh();
    m_lock->unlock();
}

void SVPenStroke::begin(f32 _px,f32 _py,f32 _pz) {
    m_lock->lock();
    //二维点到三维点的转换
    FVec2 t_pt = FVec2(_px, _py);
    SVStrokePoint t_worldPt;
    _screenPointToWorld(t_pt, t_worldPt);
    if (m_penCurve) {
        m_penCurve->reset();
        //画笔补点
        SVArray<FVec3> t_ptArray;
        m_penCurve->addPoint(t_worldPt.point, m_pen_width, m_density, SV_ADD_DRAWBEGIN, t_ptArray);
        //光晕补点
        SVArray<FVec3> t_ptGlowArray;
        m_penCurve->addPoint(t_worldPt.point, m_glowStrokeWidth, m_glowDensity, SV_ADD_DRAWBEGIN, t_ptGlowArray);
    }
    m_ptGlowPool.append(t_worldPt);
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
    if (m_penCurve) {
        //画笔补点
        SVArray<FVec3> t_ptArray;
        if (m_lerpMethod == SV_LERP_BALANCE) {
            m_penCurve->addPointB(t_worldPt.point, m_pen_width, m_density, SV_ADD_DRAWEND, t_ptArray);
        }else if (m_lerpMethod == SV_LERP_NOTBALANCE){
            m_penCurve->addPoint(t_worldPt.point, m_pen_width, m_density, SV_ADD_DRAWEND, t_ptArray);
        }
        for (s32 i = 0; i<t_ptArray.size(); i++) {
            FVec3 t_pt = t_ptArray[i];
            SVStrokePoint t_n_worldPt;
            t_n_worldPt.point = t_pt;
            t_n_worldPt.normal = t_worldPt.normal;
            t_n_worldPt.ext0 = t_worldPt.ext0;
            t_n_worldPt.ext1 = t_worldPt.ext1;
            m_ptPool.append(t_n_worldPt);
        }
        
        //光晕补点
        SVArray<FVec3> t_ptGlowArray;
        if (m_lerpMethod == SV_LERP_BALANCE) {
            m_penCurve->addPointB(t_worldPt.point, m_glowStrokeWidth, m_glowDensity, SV_ADD_DRAWEND, t_ptGlowArray);
        }else if (m_lerpMethod == SV_LERP_NOTBALANCE){
            m_penCurve->addPoint(t_worldPt.point, m_glowStrokeWidth, m_glowDensity, SV_ADD_DRAWEND, t_ptGlowArray);
        }
        for (s32 i = 0; i<t_ptGlowArray.size(); i++) {
            FVec3 t_pt = t_ptGlowArray[i];
            SVStrokePoint t_n_worldPt;
            t_n_worldPt.point = t_pt;
            t_n_worldPt.normal = t_worldPt.normal;
            t_n_worldPt.ext0 = t_worldPt.ext0;
            t_n_worldPt.ext1 = t_worldPt.ext1;
            m_ptGlowPool.append(t_n_worldPt);
        }
    }else{
        m_ptPool.append(t_worldPt);
        m_ptGlowPool.append(t_worldPt);
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
    if (m_penCurve) {
        SVArray<FVec3> t_ptArray;
        if (m_lerpMethod == SV_LERP_BALANCE) {
            m_penCurve->addPointB(t_worldPt.point, m_pen_width, m_density, SV_ADD_DRAWING, t_ptArray);
        }else if (m_lerpMethod == SV_LERP_NOTBALANCE){
            m_penCurve->addPoint(t_worldPt.point, m_pen_width, m_density, SV_ADD_DRAWING, t_ptArray);
        }
        for (s32 i = 0; i<t_ptArray.size(); i++) {
            FVec3 t_pt = t_ptArray[i];
            SVStrokePoint t_n_worldPt;
            t_n_worldPt.point = t_pt;
            t_n_worldPt.normal = t_worldPt.normal;
            t_n_worldPt.ext0 = t_worldPt.ext0;
            t_n_worldPt.ext1 = t_worldPt.ext1;
            m_ptPool.append(t_n_worldPt);
        }
        
        //光晕补点
        SVArray<FVec3> t_ptGlowArray;
        if (m_lerpMethod == SV_LERP_BALANCE) {
            m_penCurve->addPointB(t_worldPt.point, m_glowStrokeWidth, m_glowDensity, SV_ADD_DRAWEND, t_ptGlowArray);
        }else if (m_lerpMethod == SV_LERP_NOTBALANCE){
            m_penCurve->addPoint(t_worldPt.point, m_glowStrokeWidth, m_glowDensity, SV_ADD_DRAWEND, t_ptGlowArray);
        }
        for (s32 i = 0; i<t_ptGlowArray.size(); i++) {
            FVec3 t_pt = t_ptGlowArray[i];
            SVStrokePoint t_n_worldPt;
            t_n_worldPt.point = t_pt;
            t_n_worldPt.normal = t_worldPt.normal;
            t_n_worldPt.ext0 = t_worldPt.ext0;
            t_n_worldPt.ext1 = t_worldPt.ext1;
            m_ptGlowPool.append(t_n_worldPt);
        }
    }else{
        m_ptPool.append(t_worldPt);
        m_ptGlowPool.append(t_worldPt);
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

void SVPenStroke::_genInstances() {
    //荧光实例
    s32 t_pt_original_num = m_ptGlowPool.size();
    s32 t_glow_deltCount = t_pt_original_num - m_lastGlowInstanceIndex;
    if (t_glow_deltCount > 0) {
        V3 t_glowPoints[t_glow_deltCount];
        s32 t_index = m_lastGlowInstanceIndex;
        s32 j = 0;
        for (s32 i = t_index; i<t_pt_original_num; i++) {
            SVStrokePoint t_pt = m_ptGlowPool[i];
            t_glowPoints[j].x = t_pt.point.x;
            t_glowPoints[j].y = t_pt.point.y;
            t_glowPoints[j].z = t_pt.point.z;
            m_aabbBox.expand(t_pt.point);
            j++;
            m_lastGlowInstanceIndex++;
        }
        m_glowInstanceCount = t_pt_original_num;
        m_pGlowInstanceOffsetData->appendData(t_glowPoints, t_glow_deltCount*sizeof(V3));
    }
    
    //三维盒子实例子
    s32 t_pt_num = m_ptPool.size();
    s32 t_pt_deltCount = t_pt_num - m_lastInstanceIndex;
    if (t_pt_deltCount > 0) {
        V3 t_points[t_pt_deltCount];
        s32 t_index = m_lastInstanceIndex;
        s32 j = 0;
        for (s32 i = t_index; i<t_pt_num; i++) {
            SVStrokePoint t_pt = m_ptPool[i];
            t_points[j].x = t_pt.point.x;
            t_points[j].y = t_pt.point.y;
            t_points[j].z = t_pt.point.z;
            j++;
            m_lastInstanceIndex++;
        }
        m_instanceCount = t_pt_num;
        m_pInstanceOffsetData->appendData(t_points, t_pt_deltCount*sizeof(V3));
    }
}
#define vn 18
void SVPenStroke::_createGlowMesh(){
    m_pGlowMesh = MakeSharedPtr<SVRenderMeshDvid>(mApp);
    m_pGlowMesh->createMesh();
    m_pGlowMesh->setVertexType(E_VF_V3_C_T0_INS);
    m_pGlowMesh->setDrawMethod(E_DM_TRIANGLES);
    V3 t_ver[vn];
    V2 t_texcoord[vn];
    C t_color[vn];
    f32 t_half_w = m_glowStrokeWidth*0.5f;
    FVec2 t_t0 = FVec2(0.0f,0.0f);
    FVec2 t_t1 = FVec2(1.0f,0.0f);
    FVec2 t_t2 = FVec2(0.0f,1.0f);
    FVec2 t_t3 = FVec2(1.0f,1.0f);
    u8 t_r = m_glowColor.x;
    u8 t_g = m_glowColor.y;
    u8 t_b = m_glowColor.z;
    u8 t_a = m_glowColor.w;
    //x o z
    FVec3 t_pt0 = FVec3(-1.0f, 0.0f, 1.0f)*t_half_w;
    FVec3 t_pt1 = FVec3(1.0f, 0.0f, 1.0f)*t_half_w;
    FVec3 t_pt2 = FVec3(-1.0f, 0.0f, -1.0f)*t_half_w;
    FVec3 t_pt3 = FVec3(1.0f, 0.0f, -1.0f)*t_half_w;
    //x o y
    FVec3 t_pt4 = FVec3(-1.0f, -1.0f, 0.0f)*t_half_w;
    FVec3 t_pt5 = FVec3(1.0f, -1.0f, 0.0f)*t_half_w;
    FVec3 t_pt6 = FVec3(-1.0f, 1.0f, 0.0f)*t_half_w;
    FVec3 t_pt7 = FVec3(1.0f, 1.0f, 0.0f)*t_half_w;
    // y o z
    FVec3 t_pt8 = FVec3(0.0f, -1.0f, 1.0f)*t_half_w;
    FVec3 t_pt9 = FVec3(0.0f, -1.0f, -1.0f)*t_half_w;
    FVec3 t_pt10 = FVec3(0.0f, 1.0f, 1.0f)*t_half_w;
    FVec3 t_pt11 = FVec3(0.0f, 1.0f, -1.0f)*t_half_w;
    
    t_ver[0].x = t_pt0.x;
    t_ver[0].y = t_pt0.y;
    t_ver[0].z = t_pt0.z;
    t_texcoord[0].x = t_t0.x;
    t_texcoord[0].y = t_t0.y;
    t_color[0].r = t_r;
    t_color[0].g = t_g;
    t_color[0].b = t_b;
    t_color[0].a = t_a;
    t_ver[1].x = t_pt1.x;
    t_ver[1].y = t_pt1.y;
    t_ver[1].z = t_pt1.z;
    t_texcoord[1].x = t_t1.x;
    t_texcoord[1].y = t_t1.y;
    t_color[1].r = t_r;
    t_color[1].g = t_g;
    t_color[1].b = t_b;
    t_color[1].a = t_a;
    t_ver[2].x = t_pt2.x;
    t_ver[2].y = t_pt2.y;
    t_ver[2].z = t_pt2.z;
    t_texcoord[2].x = t_t2.x;
    t_texcoord[2].y = t_t2.y;
    t_color[2].r = t_r;
    t_color[2].g = t_g;
    t_color[2].b = t_b;
    t_color[2].a = t_a;
    t_ver[3].x = t_pt2.x;
    t_ver[3].y = t_pt2.y;
    t_ver[3].z = t_pt2.z;
    t_texcoord[3].x = t_t2.x;
    t_texcoord[3].y = t_t2.y;
    t_color[3].r = t_r;
    t_color[3].g = t_g;
    t_color[3].b = t_b;
    t_color[3].a = t_a;
    t_ver[4].x = t_pt1.x;
    t_ver[4].y = t_pt1.y;
    t_ver[4].z = t_pt1.z;
    t_texcoord[4].x = t_t1.x;
    t_texcoord[4].y = t_t1.y;
    t_color[4].r = t_r;
    t_color[4].g = t_g;
    t_color[4].b = t_b;
    t_color[4].a = t_a;
    t_ver[5].x = t_pt3.x;
    t_ver[5].y = t_pt3.y;
    t_ver[5].z = t_pt3.z;
    t_texcoord[5].x = t_t3.x;
    t_texcoord[5].y = t_t3.y;
    t_color[5].r = t_r;
    t_color[5].g = t_g;
    t_color[5].b = t_b;
    t_color[5].a = t_a;
    
    s32 i=1;
    t_ver[0+6*i].x = t_pt4.x;
    t_ver[0+6*i].y = t_pt4.y;
    t_ver[0+6*i].z = t_pt4.z;
    t_texcoord[0+6*i].x = t_t0.x;
    t_texcoord[0+6*i].y = t_t0.y;
    t_color[0+6*i].r = t_r;
    t_color[0+6*i].g = t_g;
    t_color[0+6*i].b = t_b;
    t_color[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt5.x;
    t_ver[1+6*i].y = t_pt5.y;
    t_ver[1+6*i].z = t_pt5.z;
    t_texcoord[1+6*i].x = t_t1.x;
    t_texcoord[1+6*i].y = t_t1.y;
    t_color[1+6*i].r = t_r;
    t_color[1+6*i].g = t_g;
    t_color[1+6*i].b = t_b;
    t_color[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt6.x;
    t_ver[2+6*i].y = t_pt6.y;
    t_ver[2+6*i].z = t_pt6.z;
    t_texcoord[2+6*i].x = t_t2.x;
    t_texcoord[2+6*i].y = t_t2.y;
    t_color[2+6*i].r = t_r;
    t_color[2+6*i].g = t_g;
    t_color[2+6*i].b = t_b;
    t_color[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt6.x;
    t_ver[3+6*i].y = t_pt6.y;
    t_ver[3+6*i].z = t_pt6.z;
    t_texcoord[3+6*i].x = t_t2.x;
    t_texcoord[3+6*i].y = t_t2.y;
    t_color[3+6*i].r = t_r;
    t_color[3+6*i].g = t_g;
    t_color[3+6*i].b = t_b;
    t_color[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt5.x;
    t_ver[4+6*i].y = t_pt5.y;
    t_ver[4+6*i].z = t_pt5.z;
    t_texcoord[4+6*i].x = t_t1.x;
    t_texcoord[4+6*i].y = t_t1.y;
    t_color[4+6*i].r = t_r;
    t_color[4+6*i].g = t_g;
    t_color[4+6*i].b = t_b;
    t_color[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt7.x;
    t_ver[5+6*i].y = t_pt7.y;
    t_ver[5+6*i].z = t_pt7.z;
    t_texcoord[5+6*i].x = t_t3.x;
    t_texcoord[5+6*i].y = t_t3.y;
    t_color[5+6*i].r = t_r;
    t_color[5+6*i].g = t_g;
    t_color[5+6*i].b = t_b;
    t_color[5+6*i].a = t_a;
    
    i=2;
    t_ver[0+6*i].x = t_pt8.x;
    t_ver[0+6*i].y = t_pt8.y;
    t_ver[0+6*i].z = t_pt8.z;
    t_texcoord[0+6*i].x = t_t0.x;
    t_texcoord[0+6*i].y = t_t0.y;
    t_color[0+6*i].r = t_r;
    t_color[0+6*i].g = t_g;
    t_color[0+6*i].b = t_b;
    t_color[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt9.x;
    t_ver[1+6*i].y = t_pt9.y;
    t_ver[1+6*i].z = t_pt9.z;
    t_texcoord[1+6*i].x = t_t1.x;
    t_texcoord[1+6*i].y = t_t1.y;
    t_color[1+6*i].r = t_r;
    t_color[1+6*i].g = t_g;
    t_color[1+6*i].b = t_b;
    t_color[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt10.x;
    t_ver[2+6*i].y = t_pt10.y;
    t_ver[2+6*i].z = t_pt10.z;
    t_texcoord[2+6*i].x = t_t2.x;
    t_texcoord[2+6*i].y = t_t2.y;
    t_color[2+6*i].r = t_r;
    t_color[2+6*i].g = t_g;
    t_color[2+6*i].b = t_b;
    t_color[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt10.x;
    t_ver[3+6*i].y = t_pt10.y;
    t_ver[3+6*i].z = t_pt10.z;
    t_texcoord[3+6*i].x = t_t2.x;
    t_texcoord[3+6*i].y = t_t2.y;
    t_color[3+6*i].r = t_r;
    t_color[3+6*i].g = t_g;
    t_color[3+6*i].b = t_b;
    t_color[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt9.x;
    t_ver[4+6*i].y = t_pt9.y;
    t_ver[4+6*i].z = t_pt9.z;
    t_texcoord[4+6*i].x = t_t1.x;
    t_texcoord[4+6*i].y = t_t1.y;
    t_color[4+6*i].r = t_r;
    t_color[4+6*i].g = t_g;
    t_color[4+6*i].b = t_b;
    t_color[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt11.x;
    t_ver[5+6*i].y = t_pt11.y;
    t_ver[5+6*i].z = t_pt11.z;
    t_texcoord[5+6*i].x = t_t3.x;
    t_texcoord[5+6*i].y = t_t3.y;
    t_color[5+6*i].r = t_r;
    t_color[5+6*i].g = t_g;
    t_color[5+6*i].b = t_b;
    t_color[5+6*i].a = t_a;
    
    SVDataSwapPtr t_pVertexData = MakeSharedPtr<SVDataSwap>();
    t_pVertexData->resize(vn*sizeof(V3));
    t_pVertexData->writeData(t_ver, vn*sizeof(V3));
    m_pGlowMesh->setVertex3Data(t_pVertexData);
    m_pGlowMesh->setVertexDataNum(vn);
    SVDataSwapPtr t_pColorData = MakeSharedPtr<SVDataSwap>();
    t_pColorData->resize(vn*sizeof(C));
    t_pColorData->writeData(t_color, vn*sizeof(C));
    m_pGlowMesh->setColor0Data(t_pColorData);
    SVDataSwapPtr t_pTexcoordData = MakeSharedPtr<SVDataSwap>();
    t_pTexcoordData->resize(vn*sizeof(V2));
    t_pTexcoordData->writeData(t_texcoord, vn*sizeof(V2));
    m_pGlowMesh->setTexcoord0Data(t_pTexcoordData);
}

void SVPenStroke::_drawGlow(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (t_renderer && t_rs && m_pRenderObj && m_pGlowMesh && m_glowInstanceCount > 0) {
        if (!m_pGlowMtl) {
            m_pGlowMtl = MakeSharedPtr<SVMtlStrokeBase>(mApp, "penstroke_texture");
            m_pGlowMtl->setTexture(0, m_pGlowTex);
            m_pGlowMtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
            m_pGlowMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
            //void setTextureParam(s32 _chanel,TEXTUREPARAM _type,s32 _value);
            m_pGlowMtl->setTexcoordFlip(1.0, -1.0);
            m_pGlowMtl->setLineSize(5.0f);
        }
        m_pGlowMtl->setDepthEnable(false);
        m_pGlowMtl->setBlendEnable(true);
        m_pGlowMtl->setBlendState(GL_SRC_ALPHA, GL_ONE);
        m_pGlowMtl->setCullEnable(false);
        m_pGlowMtl->setModelMatrix(m_localMat);
        //更新顶点数据
        m_pGlowMesh->setInstanceEnable(true);
        m_pGlowMesh->setInstanceOffsetData(m_pGlowInstanceOffsetData, m_glowInstanceCount);
        m_pRenderObj->setMesh(m_pGlowMesh);
        m_pRenderObj->setMtl(m_pGlowMtl);
        m_pRenderObj->pushCmd(t_rs, RST_AR, "SVPenStrokeRenderGlow");
    }
}

void SVPenStroke::_createStrokeMesh() {
    m_pBoxMesh = MakeSharedPtr<SVRenderMeshDvid>(mApp);
    m_pBoxMesh->createMesh();
    m_pBoxMesh->setVertexType(E_VF_V3_C_T0_INS);
    m_pBoxMesh->setDrawMethod(E_DM_TRIANGLES);
    V3 t_ver[vn];
    V2 t_texcoord[vn];
    C t_color[vn];
    f32 t_half_w = m_pen_width*0.5f;
    FVec2 t_t0 = FVec2(0.0f,0.0f);
    FVec2 t_t1 = FVec2(1.0f,0.0f);
    FVec2 t_t2 = FVec2(0.0f,1.0f);
    FVec2 t_t3 = FVec2(1.0f,1.0f);
    u8 t_r = m_strokeColor.x;
    u8 t_g = m_strokeColor.y;
    u8 t_b = m_strokeColor.z;
    u8 t_a = m_strokeColor.w;
    //x o z
    FVec3 t_pt0 = FVec3(-1.0f, 0.0f, 1.0f)*t_half_w;
    FVec3 t_pt1 = FVec3(1.0f, 0.0f, 1.0f)*t_half_w;
    FVec3 t_pt2 = FVec3(-1.0f, 0.0f, -1.0f)*t_half_w;
    FVec3 t_pt3 = FVec3(1.0f, 0.0f, -1.0f)*t_half_w;
    //x o y
    FVec3 t_pt4 = FVec3(-1.0f, -1.0f, 0.0f)*t_half_w;
    FVec3 t_pt5 = FVec3(1.0f, -1.0f, 0.0f)*t_half_w;
    FVec3 t_pt6 = FVec3(-1.0f, 1.0f, 0.0f)*t_half_w;
    FVec3 t_pt7 = FVec3(1.0f, 1.0f, 0.0f)*t_half_w;
    // y o z
    FVec3 t_pt8 = FVec3(0.0f, -1.0f, 1.0f)*t_half_w;
    FVec3 t_pt9 = FVec3(0.0f, -1.0f, -1.0f)*t_half_w;
    FVec3 t_pt10 = FVec3(0.0f, 1.0f, 1.0f)*t_half_w;
    FVec3 t_pt11 = FVec3(0.0f, 1.0f, -1.0f)*t_half_w;
    
    t_ver[0].x = t_pt0.x;
    t_ver[0].y = t_pt0.y;
    t_ver[0].z = t_pt0.z;
    t_texcoord[0].x = t_t0.x;
    t_texcoord[0].y = t_t0.y;
    t_color[0].r = t_r;
    t_color[0].g = t_g;
    t_color[0].b = t_b;
    t_color[0].a = t_a;
    t_ver[1].x = t_pt1.x;
    t_ver[1].y = t_pt1.y;
    t_ver[1].z = t_pt1.z;
    t_texcoord[1].x = t_t1.x;
    t_texcoord[1].y = t_t1.y;
    t_color[1].r = t_r;
    t_color[1].g = t_g;
    t_color[1].b = t_b;
    t_color[1].a = t_a;
    t_ver[2].x = t_pt2.x;
    t_ver[2].y = t_pt2.y;
    t_ver[2].z = t_pt2.z;
    t_texcoord[2].x = t_t2.x;
    t_texcoord[2].y = t_t2.y;
    t_color[2].r = t_r;
    t_color[2].g = t_g;
    t_color[2].b = t_b;
    t_color[2].a = t_a;
    t_ver[3].x = t_pt2.x;
    t_ver[3].y = t_pt2.y;
    t_ver[3].z = t_pt2.z;
    t_texcoord[3].x = t_t2.x;
    t_texcoord[3].y = t_t2.y;
    t_color[3].r = t_r;
    t_color[3].g = t_g;
    t_color[3].b = t_b;
    t_color[3].a = t_a;
    t_ver[4].x = t_pt1.x;
    t_ver[4].y = t_pt1.y;
    t_ver[4].z = t_pt1.z;
    t_texcoord[4].x = t_t1.x;
    t_texcoord[4].y = t_t1.y;
    t_color[4].r = t_r;
    t_color[4].g = t_g;
    t_color[4].b = t_b;
    t_color[4].a = t_a;
    t_ver[5].x = t_pt3.x;
    t_ver[5].y = t_pt3.y;
    t_ver[5].z = t_pt3.z;
    t_texcoord[5].x = t_t3.x;
    t_texcoord[5].y = t_t3.y;
    t_color[5].r = t_r;
    t_color[5].g = t_g;
    t_color[5].b = t_b;
    t_color[5].a = t_a;
    
    s32 i=1;
    t_ver[0+6*i].x = t_pt4.x;
    t_ver[0+6*i].y = t_pt4.y;
    t_ver[0+6*i].z = t_pt4.z;
    t_texcoord[0+6*i].x = t_t0.x;
    t_texcoord[0+6*i].y = t_t0.y;
    t_color[0+6*i].r = t_r;
    t_color[0+6*i].g = t_g;
    t_color[0+6*i].b = t_b;
    t_color[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt5.x;
    t_ver[1+6*i].y = t_pt5.y;
    t_ver[1+6*i].z = t_pt5.z;
    t_texcoord[1+6*i].x = t_t1.x;
    t_texcoord[1+6*i].y = t_t1.y;
    t_color[1+6*i].r = t_r;
    t_color[1+6*i].g = t_g;
    t_color[1+6*i].b = t_b;
    t_color[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt6.x;
    t_ver[2+6*i].y = t_pt6.y;
    t_ver[2+6*i].z = t_pt6.z;
    t_texcoord[2+6*i].x = t_t2.x;
    t_texcoord[2+6*i].y = t_t2.y;
    t_color[2+6*i].r = t_r;
    t_color[2+6*i].g = t_g;
    t_color[2+6*i].b = t_b;
    t_color[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt6.x;
    t_ver[3+6*i].y = t_pt6.y;
    t_ver[3+6*i].z = t_pt6.z;
    t_texcoord[3+6*i].x = t_t2.x;
    t_texcoord[3+6*i].y = t_t2.y;
    t_color[3+6*i].r = t_r;
    t_color[3+6*i].g = t_g;
    t_color[3+6*i].b = t_b;
    t_color[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt5.x;
    t_ver[4+6*i].y = t_pt5.y;
    t_ver[4+6*i].z = t_pt5.z;
    t_texcoord[4+6*i].x = t_t1.x;
    t_texcoord[4+6*i].y = t_t1.y;
    t_color[4+6*i].r = t_r;
    t_color[4+6*i].g = t_g;
    t_color[4+6*i].b = t_b;
    t_color[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt7.x;
    t_ver[5+6*i].y = t_pt7.y;
    t_ver[5+6*i].z = t_pt7.z;
    t_texcoord[5+6*i].x = t_t3.x;
    t_texcoord[5+6*i].y = t_t3.y;
    t_color[5+6*i].r = t_r;
    t_color[5+6*i].g = t_g;
    t_color[5+6*i].b = t_b;
    t_color[5+6*i].a = t_a;
//    
    i=2;
    t_ver[0+6*i].x = t_pt8.x;
    t_ver[0+6*i].y = t_pt8.y;
    t_ver[0+6*i].z = t_pt8.z;
    t_texcoord[0+6*i].x = t_t0.x;
    t_texcoord[0+6*i].y = t_t0.y;
    t_color[0+6*i].r = t_r;
    t_color[0+6*i].g = t_g;
    t_color[0+6*i].b = t_b;
    t_color[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt9.x;
    t_ver[1+6*i].y = t_pt9.y;
    t_ver[1+6*i].z = t_pt9.z;
    t_texcoord[1+6*i].x = t_t1.x;
    t_texcoord[1+6*i].y = t_t1.y;
    t_color[1+6*i].r = t_r;
    t_color[1+6*i].g = t_g;
    t_color[1+6*i].b = t_b;
    t_color[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt10.x;
    t_ver[2+6*i].y = t_pt10.y;
    t_ver[2+6*i].z = t_pt10.z;
    t_texcoord[2+6*i].x = t_t2.x;
    t_texcoord[2+6*i].y = t_t2.y;
    t_color[2+6*i].r = t_r;
    t_color[2+6*i].g = t_g;
    t_color[2+6*i].b = t_b;
    t_color[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt10.x;
    t_ver[3+6*i].y = t_pt10.y;
    t_ver[3+6*i].z = t_pt10.z;
    t_texcoord[3+6*i].x = t_t2.x;
    t_texcoord[3+6*i].y = t_t2.y;
    t_color[3+6*i].r = t_r;
    t_color[3+6*i].g = t_g;
    t_color[3+6*i].b = t_b;
    t_color[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt9.x;
    t_ver[4+6*i].y = t_pt9.y;
    t_ver[4+6*i].z = t_pt9.z;
    t_texcoord[4+6*i].x = t_t1.x;
    t_texcoord[4+6*i].y = t_t1.y;
    t_color[4+6*i].r = t_r;
    t_color[4+6*i].g = t_g;
    t_color[4+6*i].b = t_b;
    t_color[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt11.x;
    t_ver[5+6*i].y = t_pt11.y;
    t_ver[5+6*i].z = t_pt11.z;
    t_texcoord[5+6*i].x = t_t3.x;
    t_texcoord[5+6*i].y = t_t3.y;
    t_color[5+6*i].r = t_r;
    t_color[5+6*i].g = t_g;
    t_color[5+6*i].b = t_b;
    t_color[5+6*i].a = t_a;
    
    SVDataSwapPtr t_pVertexData = MakeSharedPtr<SVDataSwap>();
    t_pVertexData->resize(vn*sizeof(V3));
    t_pVertexData->writeData(t_ver, vn*sizeof(V3));
    m_pBoxMesh->setVertex3Data(t_pVertexData);
    m_pBoxMesh->setVertexDataNum(vn);
    SVDataSwapPtr t_pColorData = MakeSharedPtr<SVDataSwap>();
    t_pColorData->resize(vn*sizeof(C));
    t_pColorData->writeData(t_color, vn*sizeof(C));
    m_pBoxMesh->setColor0Data(t_pColorData);
    SVDataSwapPtr t_pTexcoordData = MakeSharedPtr<SVDataSwap>();
    t_pTexcoordData->resize(vn*sizeof(V2));
    t_pTexcoordData->writeData(t_texcoord, vn*sizeof(V2));
    m_pBoxMesh->setTexcoord0Data(t_pTexcoordData);
}

void SVPenStroke::_drawMesh() {
    _drawGlow();
    _drawStroke();
    if (m_drawBox) {
        _drawBoundBox();
    }
}

void SVPenStroke::_drawStroke(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (t_renderer && t_rs && m_pRenderObj && m_pBoxMesh && m_instanceCount > 0) {
        if (!m_pMtl) {
            m_pMtl = MakeSharedPtr<SVMtlStrokeBase>(mApp, "penstroke_texture");
            m_pMtl->setTexture(0, m_pTex);
            m_pMtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
            m_pMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
            //void setTextureParam(s32 _chanel,TEXTUREPARAM _type,s32 _value);
            m_pMtl->setTexcoordFlip(1.0, -1.0);
            m_pMtl->setLineSize(5.0f);
        }
        m_pMtl->setDepthEnable(false);
        m_pMtl->setBlendEnable(true);
        m_pMtl->setBlendState(GL_SRC_ALPHA, GL_ONE);
        m_pMtl->setCullEnable(false);
        m_pMtl->setModelMatrix(m_localMat);
        //更新顶点数据
        m_pBoxMesh->setInstanceEnable(true);
        m_pBoxMesh->setInstanceOffsetData(m_pInstanceOffsetData, m_instanceCount);
        m_pRenderObj->setMesh(m_pBoxMesh);
        m_pRenderObj->setMtl(m_pMtl);
        m_pRenderObj->pushCmd(t_rs, RST_AR, "SVPenStrokeRender");
    }
}

void SVPenStroke::_drawBoundBox(){
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    SVMtlGeo3dPtr t_mtl_geo3d = MakeSharedPtr<SVMtlGeo3d>(mApp);
    t_mtl_geo3d->setColor(1.0f, 0.0f, 0.0f, 1.0f);
    FMat4 m_mat_unit = FMat4_identity;
    t_mtl_geo3d->setModelMatrix( m_mat_unit.get() ); SVRenderObjInst::pushAABBCmd(t_rs,RST_AR_END,m_aabbBox,t_mtl_geo3d,"SV3DBOX_aabb");
}

void SVPenStroke::_screenPointToWorld(FVec2 &_point, SVStrokePoint &_worldPoint){
    SVSensorProcessPtr t_sensor = mApp->getBasicSys()->getSensorModule();
    SVCameraNodePtr t_arCam = t_sensor->getARCamera();
    if(!t_arCam)
        return ;
    FMat4 t_cameraMatrix = t_arCam->getViewMatObj();
    FVec3 t_cameraEye = t_arCam->getPosition();
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

