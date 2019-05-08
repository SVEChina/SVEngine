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
SVPenStroke::SVPenStroke(SVInst* _app, f32 _strokeWidth, FVec4 &_strokeColor, f32 _glowWidth, FVec4 &_glowColor, SVPENMODE _mode)
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
    m_dirty = false;
    m_enableTranslation = false;
    m_instanceCount = 0;
    m_glowInstanceCount = 0;
    m_lastInstanceIndex = 0;
    m_lastGlowInstanceIndex = 0;
    m_plane_dis = 0.3f;
    m_position.set(0.0f, 0.0f, 0.0f);
    m_rotation.set(0.0f, 0.0f, 0.0f);
    m_scale = 1.0f;
    m_originalPosition.set(0.0f, 0.0f, 0.0f);
    m_originalRotation.set(0.0f, 0.0f, 0.0f);
    m_originalScale = 1.0f;
    m_penMode = _mode;
    if (m_penMode == SV_ARMODE) {
        m_glowDensity = 0.05;
        m_glowStrokeWidth = _glowWidth;
        m_density = 0.06;
        m_pen_width = _strokeWidth;
        m_glowColor = _glowColor;
        m_strokeColor = _strokeColor;
    }else if (m_penMode == SV_FACEMODE){
        m_glowDensity = 0.25;
        m_glowStrokeWidth = _glowWidth*5;
        m_density = 0.1;
        m_pen_width = _strokeWidth*8;
        m_glowColor = _glowColor;
        m_strokeColor = _strokeColor;
    }else if (m_penMode == SV_NORMAL){
        m_glowDensity = 0.25;
        m_glowStrokeWidth = _glowWidth*5;
        m_density = 0.1;
        m_pen_width = _strokeWidth*8;
        m_glowColor = _glowColor;
        m_strokeColor = _strokeColor;
    }
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
    m_ptPool.destroy();
    m_ptGlowPool.destroy();
    m_aabbBox.clear();
}

void SVPenStroke::setDrawBox(bool _drawBox){
    m_drawBox = _drawBox;
}

void SVPenStroke::setPosition(FVec3 &_position){
    SVStrokePoint t_worldPt;
    FVec2 t_pt = FVec2(_position.x, _position.y);
    _screenPointToWorld(t_pt, t_worldPt);
    FVec3 t_new_position = t_worldPt.point;
    if (m_position != t_new_position) {
        m_dirty = true;
        m_position = t_new_position;
    }
}

void SVPenStroke::setScale(f32 _scale){
    if (m_scale != _scale) {
        m_dirty = true;
        m_scale = _scale;
    }
}

void SVPenStroke::setRotation(FVec3 &_rotation){
    if (m_rotation != _rotation) {
        m_dirty = true;
        m_rotation = _rotation;
    }
}

void SVPenStroke::setOriginalPosition(FVec3 &_position){
    SVStrokePoint t_worldPt;
    FVec2 t_pt = FVec2(_position.x, _position.y);
    _screenPointToWorld(t_pt, t_worldPt);
    FVec3 t_new_position = t_worldPt.point;
    m_originalPosition = t_new_position;
}

void SVPenStroke::setOriginalScale(f32 _scale){
    m_originalScale = _scale;
}

void SVPenStroke::setOriginalRotation(FVec3 &_rotation){
    m_originalRotation = _rotation;
}

void SVPenStroke::setEnableTranslation(bool _enable){
    m_enableTranslation = _enable;
}

void SVPenStroke::update(f32 _dt) {
    if (m_dirty && m_enableTranslation) {
        m_localMat.setIdentity();
        FMat4 t_mat_scale = FMat4_identity;
        FMat4 t_mat_rotX = FMat4_identity;
        FMat4 t_mat_rotY = FMat4_identity;
        FMat4 t_mat_rotZ = FMat4_identity;
        FMat4 t_mat_trans = FMat4_identity;
        t_mat_scale.setScale(FVec3(m_scale/m_originalScale, m_scale/m_originalScale, m_scale/m_originalScale));
        t_mat_rotX.setRotateX(m_rotation.x - m_originalRotation.x);
        t_mat_rotY.setRotateY(m_rotation.y - m_originalRotation.y);
        t_mat_rotZ.setRotateZ(m_rotation.z - m_originalRotation.z);
        t_mat_trans.setTranslate(m_position - m_originalPosition);
        m_localMat = t_mat_trans*t_mat_rotZ*t_mat_rotY*t_mat_rotX*t_mat_scale;
        m_dirty = false;
    }
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

#define vn_glow 18
void SVPenStroke::_createGlowMesh(){
    m_pGlowMesh = MakeSharedPtr<SVRenderMeshDvid>(mApp);
    m_pGlowMesh->createMesh();
    m_pGlowMesh->setVertexType(E_VF_V3_C_T0);
    m_pGlowMesh->setDrawMethod(E_DM_TRIANGLES);
    V3 t_ver[vn_glow];
    V2 t_texcoord[vn_glow];
    C t_color[vn_glow];
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
    t_pVertexData->resize(vn_glow*sizeof(V3));
    t_pVertexData->writeData(t_ver, vn_glow*sizeof(V3));
    m_pGlowMesh->setVertex3Data(t_pVertexData);
    m_pGlowMesh->setVertexDataNum(vn_glow);
    SVDataSwapPtr t_pColorData = MakeSharedPtr<SVDataSwap>();
    t_pColorData->resize(vn_glow*sizeof(C));
    t_pColorData->writeData(t_color, vn_glow*sizeof(C));
    m_pGlowMesh->setColor0Data(t_pColorData);
    SVDataSwapPtr t_pTexcoordData = MakeSharedPtr<SVDataSwap>();
    t_pTexcoordData->resize(vn_glow*sizeof(V2));
    t_pTexcoordData->writeData(t_texcoord, vn_glow*sizeof(V2));
    m_pGlowMesh->setTexcoord0Data(t_pTexcoordData);
}

#define vn_stroke 36
void SVPenStroke::_createStrokeMesh() {
    m_pBoxMesh = MakeSharedPtr<SVRenderMeshDvid>(mApp);
    m_pBoxMesh->createMesh();
    m_pBoxMesh->setVertexType(E_VF_V3_C);
    m_pBoxMesh->setDrawMethod(E_DM_TRIANGLES);
    V3 t_ver[vn_stroke];
    V2 t_texcoord[vn_stroke];
    C t_color[vn_stroke];
    f32 t_half_w = m_pen_width*0.5f;
    FVec3 t_pt0 = FVec3(-1.0f,-1.0f,-1.0f)*t_half_w;
    FVec3 t_pt1 = FVec3(1.0f,-1.0f,-1.0f)*t_half_w;
    FVec3 t_pt2 = FVec3(1.0f,-1.0f,1.0f)*t_half_w;
    FVec3 t_pt3 = FVec3(-1.0f,-1.0f,1.0f)*t_half_w;
    FVec3 t_pt4 = FVec3(-1.0f,1.0f,-1.0f)*t_half_w;
    FVec3 t_pt5 = FVec3(1.0f,1.0f,-1.0f)*t_half_w;
    FVec3 t_pt6 = FVec3(1.0f,1.0f,1.0f)*t_half_w;
    FVec3 t_pt7 = FVec3(-1.0f,1.0f,1.0f)*t_half_w;
    FVec2 t_t0 = FVec2(0.0f,0.0f);
    FVec2 t_t1 = FVec2(1.0f,0.0f);
    FVec2 t_t2 = FVec2(0.0f,1.0f);
    FVec2 t_t3 = FVec2(1.0f,1.0f);
    u8 t_r = m_strokeColor.x;
    u8 t_g = m_strokeColor.y;
    u8 t_b = m_strokeColor.z;
    u8 t_a = m_strokeColor.w;
    //0,1,4,5
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
    t_ver[2].x = t_pt4.x;
    t_ver[2].y = t_pt4.y;
    t_ver[2].z = t_pt4.z;
    t_texcoord[2].x = t_t2.x;
    t_texcoord[2].y = t_t2.y;
    t_color[2].r = t_r;
    t_color[2].g = t_g;
    t_color[2].b = t_b;
    t_color[2].a = t_a;
    t_ver[3].x = t_pt1.x;
    t_ver[3].y = t_pt1.y;
    t_ver[3].z = t_pt1.z;
    t_texcoord[3].x = t_t1.x;
    t_texcoord[3].y = t_t1.y;
    t_color[3].r = t_r;
    t_color[3].g = t_g;
    t_color[3].b = t_b;
    t_color[3].a = t_a;
    t_ver[4].x = t_pt4.x;
    t_ver[4].y = t_pt4.y;
    t_ver[4].z = t_pt4.z;
    t_texcoord[4].x = t_t2.x;
    t_texcoord[4].y = t_t2.y;
    t_color[4].r = t_r;
    t_color[4].g = t_g;
    t_color[4].b = t_b;
    t_color[4].a = t_a;
    t_ver[5].x = t_pt5.x;
    t_ver[5].y = t_pt5.y;
    t_ver[5].z = t_pt5.z;
    t_texcoord[5].x = t_t3.x;
    t_texcoord[5].y = t_t3.y;
    t_color[5].r = t_r;
    t_color[5].g = t_g;
    t_color[5].b = t_b;
    t_color[5].a = t_a;
    //1,2,5,6
    s32 i=1;
    t_ver[0+6*i].x = t_pt1.x;
    t_ver[0+6*i].y = t_pt1.y;
    t_ver[0+6*i].z = t_pt1.z;
    t_texcoord[0+6*i].x = t_t0.x;
    t_texcoord[0+6*i].y = t_t0.y;
    t_color[0+6*i].r = t_r;
    t_color[0+6*i].g = t_g;
    t_color[0+6*i].b = t_b;
    t_color[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt2.x;
    t_ver[1+6*i].y = t_pt2.y;
    t_ver[1+6*i].z = t_pt2.z;
    t_texcoord[1+6*i].x = t_t1.x;
    t_texcoord[1+6*i].y = t_t1.y;
    t_color[1+6*i].r = t_r;
    t_color[1+6*i].g = t_g;
    t_color[1+6*i].b = t_b;
    t_color[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt5.x;
    t_ver[2+6*i].y = t_pt5.y;
    t_ver[2+6*i].z = t_pt5.z;
    t_texcoord[2+6*i].x = t_t2.x;
    t_texcoord[2+6*i].y = t_t2.y;
    t_color[2+6*i].r = t_r;
    t_color[2+6*i].g = t_g;
    t_color[2+6*i].b = t_b;
    t_color[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt2.x;
    t_ver[3+6*i].y = t_pt2.y;
    t_ver[3+6*i].z = t_pt2.z;
    t_texcoord[3+6*i].x = t_t1.x;
    t_texcoord[3+6*i].y = t_t1.y;
    t_color[3+6*i].r = t_r;
    t_color[3+6*i].g = t_g;
    t_color[3+6*i].b = t_b;
    t_color[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt5.x;
    t_ver[4+6*i].y = t_pt5.y;
    t_ver[4+6*i].z = t_pt5.z;
    t_texcoord[4+6*i].x = t_t2.x;
    t_texcoord[4+6*i].y = t_t2.y;
    t_color[4+6*i].r = t_r;
    t_color[4+6*i].g = t_g;
    t_color[4+6*i].b = t_b;
    t_color[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt6.x;
    t_ver[5+6*i].y = t_pt6.y;
    t_ver[5+6*i].z = t_pt6.z;
    t_texcoord[5+6*i].x = t_t3.x;
    t_texcoord[5+6*i].y = t_t3.y;
    t_color[5+6*i].r = t_r;
    t_color[5+6*i].g = t_g;
    t_color[5+6*i].b = t_b;
    t_color[5+6*i].a = t_a;
    //2,3,6,7
    i=2;
    t_ver[0+6*i].x = t_pt2.x;
    t_ver[0+6*i].y = t_pt2.y;
    t_ver[0+6*i].z = t_pt2.z;
    t_texcoord[0+6*i].x = t_t0.x;
    t_texcoord[0+6*i].y = t_t0.y;
    t_color[0+6*i].r = t_r;
    t_color[0+6*i].g = t_g;
    t_color[0+6*i].b = t_b;
    t_color[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt3.x;
    t_ver[1+6*i].y = t_pt3.y;
    t_ver[1+6*i].z = t_pt3.z;
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
    t_ver[3+6*i].x = t_pt3.x;
    t_ver[3+6*i].y = t_pt3.y;
    t_ver[3+6*i].z = t_pt3.z;
    t_texcoord[3+6*i].x = t_t1.x;
    t_texcoord[3+6*i].y = t_t1.y;
    t_color[3+6*i].r = t_r;
    t_color[3+6*i].g = t_g;
    t_color[3+6*i].b = t_b;
    t_color[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt6.x;
    t_ver[4+6*i].y = t_pt6.y;
    t_ver[4+6*i].z = t_pt6.z;
    t_texcoord[4+6*i].x = t_t2.x;
    t_texcoord[4+6*i].y = t_t2.y;
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
    //3,0,7,4
    i=3;
    t_ver[0+6*i].x = t_pt3.x;
    t_ver[0+6*i].y = t_pt3.y;
    t_ver[0+6*i].z = t_pt3.z;
    t_texcoord[0+6*i].x = t_t0.x;
    t_texcoord[0+6*i].y = t_t0.y;
    t_color[0+6*i].r = t_r;
    t_color[0+6*i].g = t_g;
    t_color[0+6*i].b = t_b;
    t_color[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt0.x;
    t_ver[1+6*i].y = t_pt0.y;
    t_ver[1+6*i].z = t_pt0.z;
    t_texcoord[1+6*i].x = t_t1.x;
    t_texcoord[1+6*i].y = t_t1.y;
    t_color[1+6*i].r = t_r;
    t_color[1+6*i].g = t_g;
    t_color[1+6*i].b = t_b;
    t_color[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt7.x;
    t_ver[2+6*i].y = t_pt7.y;
    t_ver[2+6*i].z = t_pt7.z;
    t_texcoord[2+6*i].x = t_t2.x;
    t_texcoord[2+6*i].y = t_t2.y;
    t_color[2+6*i].r = t_r;
    t_color[2+6*i].g = t_g;
    t_color[2+6*i].b = t_b;
    t_color[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt0.x;
    t_ver[3+6*i].y = t_pt0.y;
    t_ver[3+6*i].z = t_pt0.z;
    t_texcoord[3+6*i].x = t_t1.x;
    t_texcoord[3+6*i].y = t_t1.y;
    t_color[3+6*i].r = t_r;
    t_color[3+6*i].g = t_g;
    t_color[3+6*i].b = t_b;
    t_color[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt7.x;
    t_ver[4+6*i].y = t_pt7.y;
    t_ver[4+6*i].z = t_pt7.z;
    t_texcoord[4+6*i].x = t_t2.x;
    t_texcoord[4+6*i].y = t_t2.y;
    t_color[4+6*i].r = t_r;
    t_color[4+6*i].g = t_g;
    t_color[4+6*i].b = t_b;
    t_color[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt4.x;
    t_ver[5+6*i].y = t_pt4.y;
    t_ver[5+6*i].z = t_pt4.z;
    t_texcoord[5+6*i].x = t_t3.x;
    t_texcoord[5+6*i].y = t_t3.y;
    t_color[5+6*i].r = t_r;
    t_color[5+6*i].g = t_g;
    t_color[5+6*i].b = t_b;
    t_color[5+6*i].a = t_a;
    //3,2,0,1
    i=4;
    t_ver[0+6*i].x = t_pt3.x;
    t_ver[0+6*i].y = t_pt3.y;
    t_ver[0+6*i].z = t_pt3.z;
    t_texcoord[0+6*i].x = t_t0.x;
    t_texcoord[0+6*i].y = t_t0.y;
    t_color[0+6*i].r = t_r;
    t_color[0+6*i].g = t_g;
    t_color[0+6*i].b = t_b;
    t_color[0+6*i].a = t_a;
    t_ver[1+6*i].x = t_pt2.x;
    t_ver[1+6*i].y = t_pt2.y;
    t_ver[1+6*i].z = t_pt2.z;
    t_texcoord[1+6*i].x = t_t1.x;
    t_texcoord[1+6*i].y = t_t1.y;
    t_color[1+6*i].r = t_r;
    t_color[1+6*i].g = t_g;
    t_color[1+6*i].b = t_b;
    t_color[1+6*i].a = t_a;
    t_ver[2+6*i].x = t_pt0.x;
    t_ver[2+6*i].y = t_pt0.y;
    t_ver[2+6*i].z = t_pt0.z;
    t_texcoord[2+6*i].x = t_t2.x;
    t_texcoord[2+6*i].y = t_t2.y;
    t_color[2+6*i].r = t_r;
    t_color[2+6*i].g = t_g;
    t_color[2+6*i].b = t_b;
    t_color[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt2.x;
    t_ver[3+6*i].y = t_pt2.y;
    t_ver[3+6*i].z = t_pt2.z;
    t_texcoord[3+6*i].x = t_t1.x;
    t_texcoord[3+6*i].y = t_t1.y;
    t_color[3+6*i].r = t_r;
    t_color[3+6*i].g = t_g;
    t_color[3+6*i].b = t_b;
    t_color[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt0.x;
    t_ver[4+6*i].y = t_pt0.y;
    t_ver[4+6*i].z = t_pt0.z;
    t_texcoord[4+6*i].x = t_t2.x;
    t_texcoord[4+6*i].y = t_t2.y;
    t_color[4+6*i].r = t_r;
    t_color[4+6*i].g = t_g;
    t_color[4+6*i].b = t_b;
    t_color[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt1.x;
    t_ver[5+6*i].y = t_pt1.y;
    t_ver[5+6*i].z = t_pt1.z;
    t_texcoord[5+6*i].x = t_t3.x;
    t_texcoord[5+6*i].y = t_t3.y;
    t_color[5+6*i].r = t_r;
    t_color[5+6*i].g = t_g;
    t_color[5+6*i].b = t_b;
    t_color[5+6*i].a = t_a;
    //4,5,7,6
    i=5;
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
    t_ver[2+6*i].x = t_pt7.x;
    t_ver[2+6*i].y = t_pt7.y;
    t_ver[2+6*i].z = t_pt7.z;
    t_texcoord[2+6*i].x = t_t2.x;
    t_texcoord[2+6*i].y = t_t2.y;
    t_color[2+6*i].r = t_r;
    t_color[2+6*i].g = t_g;
    t_color[2+6*i].b = t_b;
    t_color[2+6*i].a = t_a;
    t_ver[3+6*i].x = t_pt5.x;
    t_ver[3+6*i].y = t_pt5.y;
    t_ver[3+6*i].z = t_pt5.z;
    t_texcoord[3+6*i].x = t_t1.x;
    t_texcoord[3+6*i].y = t_t1.y;
    t_color[3+6*i].r = t_r;
    t_color[3+6*i].g = t_g;
    t_color[3+6*i].b = t_b;
    t_color[3+6*i].a = t_a;
    t_ver[4+6*i].x = t_pt7.x;
    t_ver[4+6*i].y = t_pt7.y;
    t_ver[4+6*i].z = t_pt7.z;
    t_texcoord[4+6*i].x = t_t2.x;
    t_texcoord[4+6*i].y = t_t2.y;
    t_color[4+6*i].r = t_r;
    t_color[4+6*i].g = t_g;
    t_color[4+6*i].b = t_b;
    t_color[4+6*i].a = t_a;
    t_ver[5+6*i].x = t_pt6.x;
    t_ver[5+6*i].y = t_pt6.y;
    t_ver[5+6*i].z = t_pt6.z;
    t_texcoord[5+6*i].x = t_t3.x;
    t_texcoord[5+6*i].y = t_t3.y;
    t_color[5+6*i].r = t_r;
    t_color[5+6*i].g = t_g;
    t_color[5+6*i].b = t_b;
    t_color[5+6*i].a = t_a;
    SVDataSwapPtr t_pVertexData = MakeSharedPtr<SVDataSwap>();
    t_pVertexData->resize(vn_stroke*sizeof(V3));
    t_pVertexData->writeData(t_ver, vn_stroke*sizeof(V3));
    m_pBoxMesh->setVertex3Data(t_pVertexData);
    m_pBoxMesh->setVertexDataNum(vn_stroke);
    SVDataSwapPtr t_pColorData = MakeSharedPtr<SVDataSwap>();
    t_pColorData->resize(vn_stroke*sizeof(C));
    t_pColorData->writeData(t_color, vn_stroke*sizeof(C));
    m_pBoxMesh->setColor0Data(t_pColorData);
}

void SVPenStroke::updateStroke(float _dt){
    m_lock->unlock();
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
    m_lock->unlock();
}



void SVPenStroke::renderStroke(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    SVSensorProcessPtr t_sensor = mApp->getBasicSys()->getSensorModule();
    SVCameraNodePtr t_arCam = t_sensor->getARCamera();
    if (t_renderer && t_rs && m_pRenderObj && m_pBoxMesh && m_instanceCount > 0) {
        if (!m_pMtl) {
            m_pMtl = MakeSharedPtr<SVMtlStrokeBase>(mApp, "penstroke_base");
            m_pMtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
            m_pMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
            m_pMtl->setTexcoordFlip(1.0, -1.0);
            m_pMtl->setLineSize(5.0f);
        }
        m_pMtl->setDepthEnable(true);
        m_pMtl->setBlendEnable(true);
        m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        m_pMtl->setCullEnable(false);
        m_pMtl->setModelMatrix(m_localMat);
        //更新顶点数据
        m_pBoxMesh->setInstanceOffsetData(m_pInstanceOffsetData, m_instanceCount);
        m_pRenderObj->setMesh(m_pBoxMesh);
        m_pRenderObj->setMtl(m_pMtl);
        m_pRenderObj->pushCmd(t_rs, RST_AR, "SVPenStrokeRender");
    }
}

void SVPenStroke::updateGlow(float _dt){
    m_lock->unlock();
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
    m_lock->unlock();
}

void SVPenStroke::renderGlow(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    SVSensorProcessPtr t_sensor = mApp->getBasicSys()->getSensorModule();
    SVCameraNodePtr t_arCam = t_sensor->getARCamera();
    if (t_renderer && t_rs && m_pRenderObj && m_pGlowMesh && m_glowInstanceCount > 0) {
        if (!m_pGlowMtl) {
            m_pGlowMtl = MakeSharedPtr<SVMtlStrokeBase>(mApp, "penstroke_texture");
            m_pGlowMtl->setTexture(0, m_pGlowTex);
            m_pGlowMtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
            m_pGlowMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
            m_pGlowMtl->setTexcoordFlip(1.0, -1.0);
            m_pGlowMtl->setLineSize(5.0f);
        }
        m_pGlowMtl->setDepthEnable(false);
        m_pGlowMtl->setBlendEnable(true);
        m_pGlowMtl->setBlendState(GL_SRC_ALPHA, GL_ONE);
        m_pGlowMtl->setCullEnable(false);
        m_pGlowMtl->setModelMatrix(m_localMat);
        //更新顶点数据
        m_pGlowMesh->setInstanceOffsetData(m_pGlowInstanceOffsetData, m_glowInstanceCount);
        m_pRenderObj->setMesh(m_pGlowMesh);
        m_pRenderObj->setMtl(m_pGlowMtl);
        m_pRenderObj->pushCmd(t_rs, RST_AR, "SVPenStrokeRenderGlow");
    }
}

void SVPenStroke::renderBoundingBox(){
    if (m_drawBox) {
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        SVMtlGeo3dPtr t_mtl_geo3d = MakeSharedPtr<SVMtlGeo3d>(mApp);
        t_mtl_geo3d->setColor(1.0f, 0.0f, 0.0f, 1.0f);
        FMat4 m_mat_unit = FMat4_identity;
        t_mtl_geo3d->setModelMatrix( m_mat_unit.get() ); SVRenderObjInst::pushAABBCmd(t_rs,RST_AR_END,m_aabbBox,t_mtl_geo3d,"SV3DBOX_aabb");
    }
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
