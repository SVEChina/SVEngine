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
#include "../../app/SVInst.h"
#include "../../core/SVVertDef.h"
#include "../../core/SVGeoGen.h"
#include "../../core/SVPass.h"
#include "../../base/SVVec3.h"
#include "../../base/SVDataSwap.h"
#include "../../rendercore/SVRenderObject.h"
#include "../../rendercore/SVRenderMesh.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../../rendercore/SVRenderer.h"
#include "../../rendercore/SVRenderTexture.h"
#include "../../mtl/SVMtlCore.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../mtl/SVMtlPenStrokeGlow.h"
#include "../../mtl/SVMtlStrokeBase.h"
#include "../../mtl/SVMtlNocolor.h"
#include "../../basesys/SVCameraMgr.h"
#include "../../basesys/SVBasicSys.h"
#include "../../basesys/SVPickProcess.h"
#include "../../basesys/SVSensorProcess.h"
#include "../../basesys/SVSceneMgr.h"
#include "../../node/SVCameraNode.h"
#include "../../node/SVBillboardNode.h"
#include "../../node/SVSpriteNode.h"
#include "../../node/SVScene.h"
#include "SVPenPackData.h"
SVPenStroke::SVPenStroke(SVInst* _app, SVPENMODE _mode, f32 _strokeWidth, FVec4 &_strokeColor, f32 _glowWidth, FVec4 &_glowColor)
:SVGameBase(_app) {
    m_penStrokeCurve = MakeSharedPtr<SVPenCurve>(_app);
    m_penGlowCurve = MakeSharedPtr<SVPenCurve>(_app);
    m_ptStrokePool.clear();
    m_ptGlowPool.clear();
    m_ptCachePool.clear();
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
    m_drawBox = false;
    m_haveGenFaceCoord = false;
    m_lerpMethod = SV_LERP_BALANCE;
    m_penMode = _mode;
    m_pt_count = 0;
    m_instanceCount = 0;
    m_glowInstanceCount = 0;
    m_lastInstanceIndex = 0;
    m_lastGlowInstanceIndex = 0;
    m_faceTransform.setIdentity();
    m_raw_faceParam.faceCenter.set(0.0f, 0.0f, 0.0f);
    m_raw_faceParam.faceRot.set(0.0f, 0.0f, 0.0f);
    m_raw_faceParam.eyeDis = 1.0;
    m_faceParam.faceCenter.set(0.0f, 0.0f, 0.0f);
    m_faceParam.faceRot.set(0.0f, 0.0f, 0.0f);
    m_faceParam.eyeDis = 1.0;
    //pen param
    if (m_penMode == SV_ARMODE) {
        m_plane_dis = 0.3f;
    }else if (m_penMode == SV_FACEMODE){
        m_plane_dis = 600.0f;
    }
    _createStrokeMesh(_strokeWidth, _strokeColor);
    _createGlowMesh(_glowWidth, _glowColor);
}

SVPenStroke::~SVPenStroke() {
    m_penStrokeCurve = nullptr;
    m_penGlowCurve = nullptr;
    m_pInstanceOffsetData->reback();
    m_pInstanceOffsetData = nullptr;
    m_pGlowInstanceOffsetData->reback();
    m_pGlowInstanceOffsetData = nullptr;
    m_pRenderObj = nullptr;
    m_pStrokeMesh = nullptr;
    m_pMtl = nullptr;
    m_pTex = nullptr;
    m_pGlowTex = nullptr;
    m_lock = nullptr;
    m_pGlowMesh = nullptr;
    m_pGlowMtl = nullptr;
    m_ptStrokePool.destroy();
    m_ptGlowPool.destroy();
    m_ptCachePool.destroy();
    m_aabbBox.clear();
}

void SVPenStroke::setDrawBox(bool _drawBox){
    m_drawBox = _drawBox;
}

void SVPenStroke::update(f32 _dt) {
    _renderBoundingBox();
}


void SVPenStroke::begin(f32 _px,f32 _py,f32 _pz) {
    _addPoint(_px, _py, SV_ADD_DRAWBEGIN);
}

void SVPenStroke::end(f32 _px,f32 _py,f32 _pz) {
    _addPoint(_px, _py, SV_ADD_DRAWEND);
}

void SVPenStroke::draw(f32 _px,f32 _py,f32 _pz) {
    _addPoint(_px, _py, SV_ADD_DRAWING);
}

void SVPenStroke::_addPoint(f32 _px, f32 _py, ADDPOINTACTION _action){
    m_lock->lock();
    //二维点到三维点的转换
    FVec2 t_pt = FVec2(_px, _py);
    m_ptCachePool.append(t_pt);
    SVStrokePoint t_worldPt;
    _screenPointToWorld(t_pt, t_worldPt);
    //
    if (m_penGlowCurve && m_penGlowCurve) {
        if (_action == SV_ADD_DRAWBEGIN) {
            m_penStrokeCurve->reset();
            m_penGlowCurve->reset();
            //画笔补点
            SVArray<FVec3> t_ptArray;
            m_penStrokeCurve->addPoint(t_worldPt.point, m_stroke_width, m_stroke_curve, _action, t_ptArray);
            //光晕补点
            SVArray<FVec3> t_ptGlowArray;
            m_penGlowCurve->addPoint(t_worldPt.point, m_glow_width, m_glow_curve, _action, t_ptGlowArray);
        }else{
            SVArray<FVec3> t_ptArray;
            if (m_lerpMethod == SV_LERP_BALANCE) {
                m_penStrokeCurve->addPointB(t_worldPt.point, m_stroke_width, m_stroke_curve, _action, t_ptArray);
            }else if (m_lerpMethod == SV_LERP_NOTBALANCE){
                m_penStrokeCurve->addPoint(t_worldPt.point, m_stroke_width, m_stroke_curve, _action, t_ptArray);
            }
            for (s32 i = 0; i<t_ptArray.size(); i++) {
                FVec3 t_pt = t_ptArray[i];
                SVStrokePoint t_n_worldPt;
                t_n_worldPt.point = t_pt;
                t_n_worldPt.normal = t_worldPt.normal;
                t_n_worldPt.ext0 = t_worldPt.ext0;
                t_n_worldPt.ext1 = t_worldPt.ext1;
                m_ptStrokePool.append(t_n_worldPt);
            }
            //光晕补点
            SVArray<FVec3> t_ptGlowArray;
            if (m_lerpMethod == SV_LERP_BALANCE) {
                m_penGlowCurve->addPointB(t_worldPt.point, m_glow_width, m_glow_curve, _action, t_ptGlowArray);
            }else if (m_lerpMethod == SV_LERP_NOTBALANCE){
                m_penGlowCurve->addPoint(t_worldPt.point, m_glow_width, m_glow_curve, _action, t_ptGlowArray);
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
        }
    }else{
        m_ptStrokePool.append(t_worldPt);
        m_ptGlowPool.append(t_worldPt);
    }
    //
    m_lock->unlock();
}

#define vn_stroke 36
void SVPenStroke::_createStrokeMesh(f32 _strokeWidth, FVec4 &_strokeColor){
    m_stroke_raw_width = _strokeWidth;
    m_stroke_color = _strokeColor;
    if (m_penMode == SV_ARMODE) {
        m_stroke_width = m_stroke_raw_width;
        m_stroke_curve = 0.05;
    }else if (m_penMode == SV_FACEMODE){
        m_stroke_width = m_stroke_raw_width*800;
        m_stroke_curve = 0.1;
    }
    m_pStrokeMesh = MakeSharedPtr<SVRenderMeshDvid>(mApp);
    m_pStrokeMesh->createMesh();
    m_pStrokeMesh->setVertexType(E_VF_V3_C);
    m_pStrokeMesh->setDrawMethod(E_DM_TRIANGLES);
    V3 t_ver[vn_stroke];
    V2 t_texcoord[vn_stroke];
    C t_color[vn_stroke];
    f32 t_half_w = m_stroke_width*0.5f;
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
    u8 t_r = m_stroke_color.x;
    u8 t_g = m_stroke_color.y;
    u8 t_b = m_stroke_color.z;
    u8 t_a = m_stroke_color.w;
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
    m_pStrokeMesh->setVertex3Data(t_pVertexData);
    m_pStrokeMesh->setVertexDataNum(vn_stroke);
    SVDataSwapPtr t_pColorData = MakeSharedPtr<SVDataSwap>();
    t_pColorData->resize(vn_stroke*sizeof(C));
    t_pColorData->writeData(t_color, vn_stroke*sizeof(C));
    m_pStrokeMesh->setColor0Data(t_pColorData);
}

#define vn_glow 18
void SVPenStroke::_createGlowMesh(f32 _glowWidth, FVec4 &_glowColor){
    m_glow_raw_width = _glowWidth;
    m_glow_color = _glowColor;
    if (m_penMode == SV_ARMODE) {
        m_glow_width = m_glow_raw_width;
        m_glow_curve = 0.06;
    }else if (m_penMode == SV_FACEMODE){
        m_glow_width = m_glow_raw_width*1800;
        m_glow_curve = 0.2;
    }
    m_pGlowMesh = MakeSharedPtr<SVRenderMeshDvid>(mApp);
    m_pGlowMesh->createMesh();
    m_pGlowMesh->setVertexType(E_VF_V3_C_T0);
    m_pGlowMesh->setDrawMethod(E_DM_TRIANGLES);
    V3 t_ver[vn_glow];
    V2 t_texcoord[vn_glow];
    C t_color[vn_glow];
    f32 t_half_w = m_glow_width*0.5f;
    FVec2 t_t0 = FVec2(0.0f,0.0f);
    FVec2 t_t1 = FVec2(1.0f,0.0f);
    FVec2 t_t2 = FVec2(0.0f,1.0f);
    FVec2 t_t3 = FVec2(1.0f,1.0f);
    u8 t_r = m_glow_color.x;
    u8 t_g = m_glow_color.y;
    u8 t_b = m_glow_color.z;
    u8 t_a = m_glow_color.w;
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

void SVPenStroke::updateStroke(float _dt){
    if (m_penMode == SV_ARMODE) {
        _updateARStroke(_dt);
    }else if (m_penMode == SV_FACEMODE){
        _updateARFaceStroke(_dt);
    }
}

void SVPenStroke::_updateARStroke(float _dt){
    m_lock->unlock();
    //三维盒子实例子
    s32 t_pt_num = m_ptStrokePool.size();
    s32 t_pt_deltCount = t_pt_num - m_lastInstanceIndex;
    if (t_pt_deltCount > 0) {
        V3 t_points[t_pt_deltCount];
        s32 t_index = m_lastInstanceIndex;
        s32 j = 0;
        for (s32 i = t_index; i<t_pt_num; i++) {
            SVStrokePoint t_pt = m_ptStrokePool[i];
            t_points[j].x = t_pt.point.x;
            t_points[j].y = t_pt.point.y;
            t_points[j].z = t_pt.point.z;
            j++;
            m_lastInstanceIndex++;
        }
        m_pInstanceOffsetData->appendData(t_points, t_pt_deltCount*sizeof(V3));
    }
    m_instanceCount = t_pt_num;
}

void SVPenStroke::_updateARFaceStroke(float _dt){
    m_lock->unlock();
    s32 t_pt_num = m_ptStrokePool.size();
    if (t_pt_num > 0) {
        V3 t_points[t_pt_num];
        for (s32 i = 0; i<t_pt_num; i++) {
            SVStrokePoint t_pt = m_ptStrokePool[i];
            //转换到脸部中心为原点的坐标系
            t_pt.point = t_pt.point - m_faceParam.faceCenter;
            //变换
            FVec4 t_n_point = FVec4(t_pt.point, 1.0);
            t_n_point = m_faceTransform*t_n_point;
            t_pt.point.set(t_n_point.x, t_n_point.y, t_n_point.z);
            //转回到以屏幕中心为原点的坐标系
            t_pt.point = t_pt.point + m_faceParam.faceCenter;
            t_points[i].x = t_pt.point.x;
            t_points[i].y = t_pt.point.y;
            t_points[i].z = t_pt.point.z;
        }
        m_pInstanceOffsetData->writeData(t_points, t_pt_num*sizeof(V3));
    }
    m_instanceCount = t_pt_num;
    m_lock->unlock();
}



void SVPenStroke::renderStroke(){
    SVRendererPtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    SVSensorProcessPtr t_sensor = mApp->getBasicSys()->getSensorModule();
    SVCameraNodePtr t_arCam = t_sensor->getARCamera();
    if (t_renderer && t_rs && m_pRenderObj && m_pStrokeMesh && m_instanceCount > 0) {
        if (!m_pMtl) {
            m_pMtl = MakeSharedPtr<SVMtlStrokeBase>(mApp, "penstroke_base");
            m_pMtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
            m_pMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
            m_pMtl->setTexcoordFlip(1.0, -1.0);
            m_pMtl->setLineSize(5.0f);
        }
        m_pMtl->setDepthEnable(true);
        m_pMtl->setBlendEnable(true);
        m_pMtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
        m_pMtl->setCullEnable(false);
        m_pMtl->setModelMatrix(m_localMat);
        //更新顶点数据
        m_pStrokeMesh->setInstanceOffsetData(m_pInstanceOffsetData, m_instanceCount);
        m_pRenderObj->setMesh(m_pStrokeMesh);
        m_pRenderObj->setMtl(m_pMtl);
        m_pRenderObj->pushCmd(t_rs, RST_AR, "SVPenStrokeRender");
    }
}

void SVPenStroke::updateGlow(float _dt){
    if (m_penMode == SV_ARMODE) {
        _updateARGlow(_dt);
    }else if (m_penMode == SV_FACEMODE){
        _updateARFaceGlow(_dt);
    }
}

void SVPenStroke::_updateARGlow(float _dt){
    m_lock->unlock();
    //荧光实例
    s32 t_pt_num = m_ptGlowPool.size();
    s32 t_glow_deltCount = t_pt_num - m_lastGlowInstanceIndex;
    if (t_glow_deltCount > 0) {
        V3 t_glowPoints[t_glow_deltCount];
        s32 t_index = m_lastGlowInstanceIndex;
        s32 j = 0;
        for (s32 i = t_index; i<t_pt_num; i++) {
            SVStrokePoint t_pt = m_ptGlowPool[i];
            t_glowPoints[j].x = t_pt.point.x;
            t_glowPoints[j].y = t_pt.point.y;
            t_glowPoints[j].z = t_pt.point.z;
            j++;
            m_lastGlowInstanceIndex++;
        }
        m_pGlowInstanceOffsetData->appendData(t_glowPoints, t_glow_deltCount*sizeof(V3));
    }
    m_glowInstanceCount = t_pt_num;
    m_lock->unlock();
}

void SVPenStroke::_updateARFaceGlow(float _dt){
    m_lock->unlock();
    //荧光实例
    s32 t_pt_num = m_ptGlowPool.size();
    if (t_pt_num > 0) {
        V3 t_glowPoints[t_pt_num];
        for (s32 i = 0; i<t_pt_num; i++) {
            //转换到脸部中心为原点的坐标系
            SVStrokePoint t_pt = m_ptGlowPool[i];
            t_pt.point = t_pt.point - m_faceParam.faceCenter;
            //变换
            FVec4 t_n_point = FVec4(t_pt.point, 1.0);
            t_n_point = m_faceTransform*t_n_point;
            t_pt.point.set(t_n_point.x, t_n_point.y, t_n_point.z);
            //转回到以屏幕中心为原点的坐标系
            t_pt.point = t_pt.point + m_faceParam.faceCenter;
            t_glowPoints[i].x = t_pt.point.x;
            t_glowPoints[i].y = t_pt.point.y;
            t_glowPoints[i].z = t_pt.point.z;
            
        }
        m_pGlowInstanceOffsetData->writeData(t_glowPoints, t_pt_num*sizeof(V3));
    }
    m_glowInstanceCount = t_pt_num;
    m_lock->unlock();
}

void SVPenStroke::renderGlow(){
    SVRendererPtr t_renderer = mApp->getRenderer();
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
        m_pGlowMtl->setBlendState(MTL_BLEND_SRC_ALPHA, MTL_BLEND_ONE);
        m_pGlowMtl->setCullEnable(false);
        m_pGlowMtl->setModelMatrix(m_localMat);
        //更新顶点数据
        m_pGlowMesh->setInstanceOffsetData(m_pGlowInstanceOffsetData, m_glowInstanceCount);
        m_pRenderObj->setMesh(m_pGlowMesh);
        m_pRenderObj->setMtl(m_pGlowMtl);
        m_pRenderObj->pushCmd(t_rs, RST_AR, "SVPenStrokeRenderGlow");
    }
}

void SVPenStroke::_renderBoundingBox(){
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
        return;
    FMat4 t_cameraMatrix = t_arCam->getViewMatObj();
    FVec3 t_cameraEye = t_arCam->getPosition();
    //构建虚拟平面
    FVec3 t_cameraDir = FVec3(-t_cameraMatrix[2],
                              -t_cameraMatrix[6],
                              -t_cameraMatrix[10]);
    t_cameraDir.normalize();
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
    _worldPoint.ext0 = FVec3(0.0f,0.0f,0.0f);
    _worldPoint.ext1 = FVec3(0.0f,0.0f,0.0f);
}

void SVPenStroke::genFaceRawParam(FVec3 &_noseCenter, FVec3 &_rotation, f32 _eyeDis){
    if (!m_haveGenFaceCoord) {
        m_cache_faceParam.faceCenter = _noseCenter;
        m_cache_faceParam.faceRot = _rotation;
        m_cache_faceParam.eyeDis = _eyeDis;
        SVStrokePoint t_worldPt;
        FVec2 t_pt = FVec2(_noseCenter.x, _noseCenter.y);
        _screenPointToWorld(t_pt, t_worldPt);
        FVec3 t_faceCenter = t_worldPt.point;
        FVec3 t_faceRot = _rotation;
        f32 t_eyeDis = 1.0;
        if (_eyeDis != 0) {
            t_eyeDis = _eyeDis;
        }
        m_raw_faceParam.faceCenter = t_faceCenter;
        m_raw_faceParam.faceRot = t_faceRot;
        m_raw_faceParam.eyeDis = t_eyeDis;
        m_haveGenFaceCoord = true;
    }
}

void SVPenStroke::setFaceParam(FVec3 &_noseCenter, FVec3 &_rotation, f32 _eyeDis){
    SVStrokePoint t_worldPt;
    FVec2 t_pt = FVec2(_noseCenter.x, _noseCenter.y);
    _screenPointToWorld(t_pt, t_worldPt);
    FVec3 t_faceCenter = t_worldPt.point;
    FVec3 t_faceRot = _rotation;
    f32 t_eyeDis = 1.0;
    if (_eyeDis != 0) {
        t_eyeDis = _eyeDis;
    }
    m_faceParam.faceCenter = t_faceCenter;
    m_faceParam.faceRot = t_faceRot;
    m_faceParam.eyeDis = t_eyeDis;
    m_faceTransform.setIdentity();
    if (m_haveGenFaceCoord) {
        FMat4 t_mat_scale = FMat4_identity;
        FMat4 t_mat_rotX = FMat4_identity;
        FMat4 t_mat_rotY = FMat4_identity;
        FMat4 t_mat_rotZ = FMat4_identity;
        FMat4 t_mat_trans = FMat4_identity;
        //scale
        FVec3 t_scale = FVec3(m_faceParam.eyeDis/m_raw_faceParam.eyeDis, m_faceParam.eyeDis/m_raw_faceParam.eyeDis, m_faceParam.eyeDis/m_raw_faceParam.eyeDis);
        t_mat_scale.setScale(t_scale);
        //rotation
        FVec3 t_rot = m_faceParam.faceRot - m_raw_faceParam.faceRot;
        t_mat_rotX.setRotateX(t_rot.x);
        t_mat_rotY.setRotateY(t_rot.y);
        t_mat_rotZ.setRotateZ(t_rot.z);
        //translation
        FVec3 t_pos = m_faceParam.faceCenter - m_raw_faceParam.faceCenter;
        t_mat_trans.setTranslate(t_pos);
      //这里要先做平移变换，再做旋转变换，跟shader里的运算不一样，shader是列主元--晓帆。
        m_faceTransform = t_mat_rotZ*t_mat_rotY*t_mat_rotX*t_mat_trans*t_mat_scale;
    }
}

void SVPenStroke::_packCachePt(SVDataSwapPtr _dataSwap){
    if (!_dataSwap) {
        return;
    }
    m_lock->unlock();
    s32 t_pt_num = m_ptCachePool.size();
    if (t_pt_num > 0) {
        V2 t_points[t_pt_num];
        for (s32 i = 0; i<t_pt_num; i++) {
            //转换到脸部中心为原点的坐标系
            FVec2 t_pt = m_ptCachePool[i];
            t_points[i].x = t_pt.x;
            t_points[i].y = t_pt.y;
        }
        _dataSwap->writeData(t_points, t_pt_num*sizeof(V2));
    }
    m_pt_count = t_pt_num;
    m_lock->unlock();
}

void SVPenStroke::_unpackCachePt(SVDataSwapPtr _dataSwap, s32 _ptSize){
    if (_dataSwap && _ptSize) {
        f32 *points = (f32 *)_dataSwap->getData();
        for (s32 i = 0; i<_ptSize; i++) {
            f32 t_pt_x = points[2*i];
            f32 t_pt_y = points[2*i+1];
            FVec2 t_pt;
            t_pt.set(t_pt_x, t_pt_y);
            if (i == 0) {
                _addPoint(t_pt.x, t_pt.y, SV_ADD_DRAWBEGIN);
            }else if (i == _ptSize - 1){
                _addPoint(t_pt.x, t_pt.y, SV_ADD_DRAWEND);
            }else{
                _addPoint(t_pt.x, t_pt.y, SV_ADD_DRAWING);
            }
        }
        m_pt_count = _ptSize;
    }
}

//序列化接口
void SVPenStroke::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue, cptr8 _path){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    locationObj.AddMember("name", "stroke", _allocator);
    //points data
    u32 t_offset = (u32)SVPenPackData::checkPenStrokeDataLength(mApp, _path);
    SVDataSwapPtr t_penData = MakeSharedPtr<SVDataSwap>();
    _packCachePt(t_penData);
    if (SVPenPackData::writePenData(mApp, t_penData, _path, false)) {
        locationObj.AddMember("pen_data", "pen.bin", _allocator);
        locationObj.AddMember("pen_data_offset", t_offset, _allocator);
        locationObj.AddMember("pen_data_length", t_penData->getSize(), _allocator);
        locationObj.AddMember("pen_pt_size", m_pt_count, _allocator);
    };
    //face data
    //face_center;
    RAPIDJSON_NAMESPACE::Value t_faceCenterArray(RAPIDJSON_NAMESPACE::kArrayType);
    t_faceCenterArray.PushBack(m_cache_faceParam.faceCenter.x, _allocator);
    t_faceCenterArray.PushBack(m_cache_faceParam.faceCenter.y, _allocator);
    t_faceCenterArray.PushBack(m_cache_faceParam.faceCenter.z, _allocator);
    locationObj.AddMember("face_center", t_faceCenterArray, _allocator);
    //face_rot;
    RAPIDJSON_NAMESPACE::Value t_faceRotArray(RAPIDJSON_NAMESPACE::kArrayType);
    t_faceRotArray.PushBack(m_cache_faceParam.faceRot.x, _allocator);
    t_faceRotArray.PushBack(m_cache_faceParam.faceRot.y, _allocator);
    t_faceRotArray.PushBack(m_cache_faceParam.faceRot.z, _allocator);
    locationObj.AddMember("face_rot", t_faceRotArray, _allocator);
    //face eye dis
    locationObj.AddMember("face_eyedis", m_cache_faceParam.eyeDis, _allocator);
    //stroke
    locationObj.AddMember("stroke_width", m_stroke_raw_width, _allocator);
    RAPIDJSON_NAMESPACE::Value t_stroke_color(RAPIDJSON_NAMESPACE::kArrayType);
    t_stroke_color.PushBack(m_stroke_color.x, _allocator);
    t_stroke_color.PushBack(m_stroke_color.y, _allocator);
    t_stroke_color.PushBack(m_stroke_color.z, _allocator);
    t_stroke_color.PushBack(m_stroke_color.w, _allocator);
    locationObj.AddMember("stroke_color", t_stroke_color, _allocator);
    //glow
    locationObj.AddMember("stroke_glowidth", m_glow_raw_width, _allocator);
    RAPIDJSON_NAMESPACE::Value t_glow_color(RAPIDJSON_NAMESPACE::kArrayType);
    t_glow_color.PushBack(m_glow_color.x, _allocator);
    t_glow_color.PushBack(m_glow_color.y, _allocator);
    t_glow_color.PushBack(m_glow_color.z, _allocator);
    t_glow_color.PushBack(m_glow_color.w, _allocator);
    locationObj.AddMember("stroke_glowcolor", t_glow_color, _allocator);
    _objValue.AddMember("SVStroke", locationObj, _allocator);
}

void SVPenStroke::fromJSON(RAPIDJSON_NAMESPACE::Value &_item, cptr8 _path){
    //pen data
    {
        SVString t_penDataName = "";
        if (_item.HasMember("pen_data") && _item["pen_data"].IsString()) {
            RAPIDJSON_NAMESPACE::Value &t_value = _item["pen_data"];
            t_penDataName = t_value.GetString();
        }
        s32 t_penDataOffset = 0;
        if (_item.HasMember("pen_data_offset") && _item["pen_data_offset"].IsInt()) {
            RAPIDJSON_NAMESPACE::Value &t_value = _item["pen_data_offset"];
            t_penDataOffset = t_value.GetInt();
        }
        s32 t_penDataLength = 0;
        if (_item.HasMember("pen_data_length") && _item["pen_data_length"].IsInt()) {
            RAPIDJSON_NAMESPACE::Value &t_value = _item["pen_data_length"];
            t_penDataLength = t_value.GetInt();
        }
        s32 t_penPtSize = 0;
        if (_item.HasMember("pen_pt_size") && _item["pen_pt_size"].IsInt()) {
            RAPIDJSON_NAMESPACE::Value &t_value = _item["pen_pt_size"];
            t_penPtSize = t_value.GetInt();
        }
        SVDataSwapPtr t_penData = MakeSharedPtr<SVDataSwap>();
        SVString t_pen_data_path = SVString(_path) + "/" + t_penDataName;
        SVPenPackData::loadPenData(mApp, t_penData, t_pen_data_path, t_penDataOffset, t_penDataLength);
        _unpackCachePt(t_penData, t_penPtSize);
    }
    //
    //face data
    FVec3 t_faceCenter;
    if (_item.HasMember("face_center") && _item["face_center"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_faceCenterArray = _item["face_center"];
        if (t_faceCenterArray.Size() > 2) {
            t_faceCenter.x = t_faceCenterArray[0].GetFloat();
            t_faceCenter.y = t_faceCenterArray[1].GetFloat();
            t_faceCenter.z = t_faceCenterArray[2].GetFloat();
        }
    }
    FVec3 t_faceRot;
    if (_item.HasMember("face_rot") && _item["face_rot"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_faceRotArray = _item["face_rot"];
        if (t_faceRotArray.Size() > 2) {
            t_faceRot.x = t_faceRotArray[0].GetFloat();
            t_faceRot.y = t_faceRotArray[1].GetFloat();
            t_faceRot.z = t_faceRotArray[2].GetFloat();
        }
    }
    f32 eyeDis = 1.0;
    if (_item.HasMember("face_eyedis") && _item["face_eyedis"].IsFloat()) {
        RAPIDJSON_NAMESPACE::Value &t_faceEyeDis= _item["face_eyedis"];
        eyeDis = t_faceEyeDis.GetFloat();
    }
    genFaceRawParam(t_faceCenter, t_faceRot, eyeDis);
}
