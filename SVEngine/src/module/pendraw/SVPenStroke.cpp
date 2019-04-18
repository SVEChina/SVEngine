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
#include "../../mtl/SVMtlStrokeBase.h"
#include "../../mtl/SVMtlNocolor.h"
SVPenStroke::SVPenStroke(SVInst *_app)
:SVGameBase(_app) {
//    m_penCurve = MakeSharedPtr<SVPenCurve>(_app);
    m_ptPool.clear();
    m_screenPtPool.clear();
    m_rectanglePool.clear();
    m_localMat.setIdentity();
    m_lock = MakeSharedPtr<SVLock>();
    m_pVertData = MakeSharedPtr<SVDataSwap>();
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMesh = _app->getRenderMgr()->createMeshRObj();
    m_pMesh->createMesh();
    m_pMesh->setVertexType(E_VF_V3_C_T0);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    m_lastEdge.p0.set(-10000, -10000, -10000);
    m_lastEdge.p1.set(-10000, -10000, -10000);
    m_lastPoint.set(-10000, -10000, -10000);
    m_stroke = 0.008;
    m_density = 0.05;
    m_vertexNum = 0;
    m_drawBox = false;
    m_isFirstTouch = true;
    setDrawBox(true);
}

SVPenStroke::~SVPenStroke() {
    m_penCurve = nullptr;
    m_pVertData = nullptr;
    m_lock = nullptr;
    m_screenPtPool.clear();
    m_ptPool.clear();
    m_rectanglePool.clear();
    m_aabbBox.clear();
}

void SVPenStroke::setStrokeWidth(f32 _width){
    m_stroke = _width;
}

void SVPenStroke::setDrawBox(bool _drawBox){
    m_drawBox = _drawBox;
}

//绘制一笔
void SVPenStroke::update(f32 _dt) {
    m_lock->unlock();
    //转化成世界坐标系下的坐标点
    _genPoints();
    //
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
    m_lastEdge.p0.set(-10000, -10000, -10000);
    m_lastEdge.p1.set(-10000, -10000, -10000);
    m_lastPoint.set(-10000, -10000, -10000);
    m_screenPtPool.append(SVStrokePoint(_px,_py,_pz));
    if (m_penCurve) {
        m_penCurve->reset();
        SVArray<FVec2> t_ptArray;
        m_penCurve->addPoint(_px, _py, m_stroke, m_density, SVPenCurve::SV_ADD_DRAWBEGIN, t_ptArray);
    }
    m_lock->unlock();
}

void SVPenStroke::end(f32 _px,f32 _py,f32 _pz) {
    m_lock->lock();
    if (m_penCurve) {
        SVArray<FVec2> t_ptArray;
        m_penCurve->addPoint(_px, _py, m_stroke, m_density, SVPenCurve::SV_ADD_DRAWEND, t_ptArray);
        _updatePtPool(t_ptArray, m_screenPtPool);
    }else{
        m_screenPtPool.append(SVStrokePoint(_px,_py,_pz));
    }
    m_lock->unlock();
}

void SVPenStroke::draw(f32 _px,f32 _py,f32 _pz) {
    m_lock->lock();
    if (m_penCurve) {
        SVArray<FVec2> t_ptArray;
        m_penCurve->addPoint(_px, _py, m_stroke, m_density, SVPenCurve::SV_ADD_DRAWING, t_ptArray);
        _updatePtPool(t_ptArray, m_screenPtPool);
    }else{
        m_screenPtPool.append(SVStrokePoint(_px,_py,_pz));
    }
    m_lock->unlock();
}

void SVPenStroke::_updatePtPool(SVArray<FVec2> &_inPtPool, SVArray<SVStrokePoint> &_outPtPool){
    if (_inPtPool.size() <= 0)
        return;
    for (s32 i = 0; i<_inPtPool.size(); i++) {
        FVec2 t_pt = _inPtPool[i];
        SVStrokePoint t_n_pt = SVStrokePoint(t_pt.x, t_pt.y, 0.0f);
        _outPtPool.append(t_n_pt);
    }
}

void SVPenStroke::_genPoints(){
    for (s32 i = 0; i<m_screenPtPool.size(); i++) {
        FVec2 t_pt = FVec2(m_screenPtPool[i].x,  m_screenPtPool[i].y);
        SVStrokePoint t_worldPt;
        _screenPointToWorld(t_pt, t_worldPt);
        m_ptPool.append(t_worldPt);
    }
    m_screenPtPool.clear();
}

void SVPenStroke::_genPolygon(){
    SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
    FMat4 t_cameraMatrix = mainCamera->getViewMatObj();
    SVStrokePoint t_cameraTarget = SVStrokePoint(-t_cameraMatrix[2], -t_cameraMatrix[6], -t_cameraMatrix[10]);
    t_cameraTarget = t_cameraTarget.normalize();
    s32 t_pt_num = m_ptPool.size();
    if (t_pt_num == 0) {
        return;
    }
//    if (m_isFirstTouch) {
//        m_isFirstTouch = false;
//        SVStrokePoint t_pt = m_ptPool[0];
//        SVStrokeRectangle t_rectangle;
//        SVStrokeEdge t_edge0;
//        SVStrokeEdge t_edge1;
//        t_edge0.p0 = SVStrokePoint(t_pt.x - m_stroke*0.5, t_pt.y + m_stroke*0.5, t_pt.z);
//        t_edge0.p1 = SVStrokePoint(t_pt.x + m_stroke*0.5, t_pt.y + m_stroke*0.5, t_pt.z);
//        t_edge1.p0 = SVStrokePoint(t_pt.x - m_stroke*0.5, t_pt.y - m_stroke*0.5, t_pt.z);
//        t_edge1.p1 = SVStrokePoint(t_pt.x + m_stroke*0.5, t_pt.y - m_stroke*0.5, t_pt.z);
//        t_rectangle.edge0 = t_edge0;
//        t_rectangle.edge1 = t_edge1;
//        m_lastEdge = t_edge1;
//        m_rectanglePool.append(t_rectangle);
//    }
//    return;
    if (m_lastPoint.x == -10000) {
        SVStrokePoint t_pt = m_ptPool[0];
        m_lastPoint = t_pt;
    }
    
    if (t_pt_num == 1) {
        SVStrokePoint t_pt = m_ptPool[0];
        if (m_lastPoint != t_pt) {
            SVStrokePoint t_pt0 = m_lastPoint;
            SVStrokePoint t_pt1 = t_pt;
            SVStrokePoint t_p1p0 = (t_pt1 - t_pt0).normalize();
            SVStrokePoint t_perpVec = cross(t_p1p0, t_cameraTarget).normalize();
            SVStrokePoint p0 = t_pt1 - t_perpVec*stroke;
            SVStrokePoint p1 = t_pt1 + t_perpVec*stroke;
            //判断两条边是否能够相交
            FVec2 t_t_p0 = FVec2(p0.x, p0.y);
            FVec2 t_t_p1 = FVec2(p1.x, p1.y);
            FVec2 t_t_p3 = FVec2(m_lastEdge.p0.x, m_lastEdge.p0.y);
            FVec2 t_t_p4 = FVec2(m_lastEdge.p1.x,m_lastEdge.p1.y);
            SVStrokeEdge t_edge;
            if (!getTwoLinesIntersection(t_t_p0, t_t_p4, t_t_p1, t_t_p3)) {
                t_edge.p0 = p1;
                t_edge.p1 = p0;
            }else{
                t_edge.p0 = p0;
                t_edge.p1 = p1;
            }
            //
            SVStrokeRectangle t_rectangle;
            t_rectangle.edge0 = m_lastEdge;
            t_rectangle.edge1 = t_edge;
            m_lastEdge = t_edge;
            m_rectanglePool.append(t_rectangle);
        }
    }else{
        for (s32 i = 0; i<t_pt_num; i++) {
            SVStrokePoint t_pt1 = m_ptPool[i];
            SVStrokePoint t_perpVec;
            f32 stroke = m_stroke*0.5f;
            bool  isRectangle = true;
            if (i == 0) {
                SVStrokePoint t_pt2 = m_ptPool[i+1];
                SVStrokePoint t_p2p1 = (t_pt2 - t_pt1).normalize();
                t_perpVec = cross(t_p2p1, t_cameraTarget).normalize();
            }else if (i == t_pt_num - 1){
                SVStrokePoint t_pt0 = m_ptPool[i-1];
                SVStrokePoint t_p1p0 = t_pt1 - t_pt0;
                t_perpVec = cross(t_p1p0, t_cameraTarget).normalize();
            }else{
                SVStrokePoint t_pt2 = m_ptPool[i+1];
                SVStrokePoint t_pt0 = m_ptPool[i-1];
                SVStrokePoint t_p2p1 = (t_pt2 - t_pt1).normalize();
                SVStrokePoint t_p1p0 = (t_pt1 - t_pt0).normalize();
                SVStrokePoint t_tangent = (t_p1p0+t_p2p1).normalize();
                SVStrokePoint t_normal = cross(t_p1p0, t_p2p1).normalize();
                SVStrokePoint t_miter = cross(t_cameraTarget, t_tangent).normalize();
                t_perpVec = cross(t_p1p0, t_cameraTarget).normalize();
                stroke = m_stroke*0.5 / dot(t_miter, t_perpVec);
            }
            if (isRectangle) {
                SVStrokePoint p0 = t_pt1 - t_perpVec*stroke;
                SVStrokePoint p1 = t_pt1 + t_perpVec*stroke;
                //判断两条边是否能够相交
                FVec2 t_t_p0 = FVec2(p0.x, p0.y);
                FVec2 t_t_p1 = FVec2(p1.x, p1.y);
                FVec2 t_t_p3 = FVec2(m_lastEdge.p0.x, m_lastEdge.p0.y);
                FVec2 t_t_p4 = FVec2(m_lastEdge.p1.x,m_lastEdge.p1.y);
                SVStrokeEdge t_edge;
                if (!getTwoLinesIntersection(t_t_p0, t_t_p4, t_t_p1, t_t_p3)) {
                    t_edge.p0 = p1;
                    t_edge.p1 = p0;
                }else{
                    t_edge.p0 = p0;
                    t_edge.p1 = p1;
                }
                //
                SVStrokeRectangle t_rectangle;
                t_rectangle.edge0 = m_lastEdge;
                t_rectangle.edge1 = t_edge;
                m_lastEdge = t_edge;
                m_rectanglePool.append(t_rectangle);
            }
            
        }
    }
    m_ptPool.clear();
}

//生成矩形
void SVPenStroke::_genRectangle(){
    /*
    //
    SVStrokeRectangle t_rectangle;
    s32 t_pt_num = m_ptPool.size();
    for (s32 i = 0; i<t_pt_num; i++) {
        SVStrokePoint t_pt = m_ptPool[i];
        //0
        FVec2 t_pt0 = FVec2(t_pt.x - m_stroke*0.5, t_pt.y - m_stroke*0.5);
        t_rectangle.edge0.p0 = t_pt0;
        //1
        FVec2 t_pt1;
        t_pt1 = FVec2(t_pt.x - m_stroke*0.5, t_pt.y + m_stroke*0.5);
        t_rectangle.edge0.p1 = t_pt1;
        //2
        FVec2 t_pt2;
        t_pt2 = FVec2(t_pt.x + m_stroke*0.5, t_pt.y - m_stroke*0.5);
        t_rectangle.edge1.p0 = t_pt2;
        //3
        FVec2 t_pt3;
        t_pt3 = FVec2(t_pt.x + m_stroke*0.5, t_pt.y + m_stroke*0.5);
        t_rectangle.edge1.p1 = t_pt3;
        //
        m_rectanglePool.append(t_rectangle);
    }
    m_ptPool.clear();
     */
}

//生成数据
void SVPenStroke::_genMesh() {
    //
    s32 t_rectangle_num = m_rectanglePool.size();
    s32 t_vertex_num = t_rectangle_num*6;
    s32 t_vertex_size = t_vertex_num*sizeof(V3_C_T0);
    V3_C_T0 verts[t_vertex_num];
    V3_C_T0 *t_verts = verts;
    for (s32 i = 0; i<t_rectangle_num; i++) {
        SVStrokeRectangle t_rectangle = m_rectanglePool[i];
        //0
        SVStrokePoint t_worldPt0 = t_rectangle.edge0.p0;
        m_aabbBox.expand(t_worldPt0);
        //1
        SVStrokePoint t_worldPt1 = t_rectangle.edge0.p1;
        m_aabbBox.expand(t_worldPt1);
        //2
        SVStrokePoint t_worldPt2 = t_rectangle.edge1.p0;
        m_aabbBox.expand(t_worldPt2);
        //3
        SVStrokePoint t_worldPt3 = t_rectangle.edge1.p1;
        m_aabbBox.expand(t_worldPt3);
        //
        t_verts[i*6 + 0].x = t_worldPt0.x;
        t_verts[i*6 + 0].y = t_worldPt0.y;
        t_verts[i*6 + 0].z = t_worldPt0.z;
        t_verts[i*6 + 0].t0x = 0.0f;
        t_verts[i*6 + 0].t0y = 0.0f;
        t_verts[i*6 + 0].r = 0.0f;
        t_verts[i*6 + 0].g = 255.0f;
        t_verts[i*6 + 0].b = 0.0f;
        t_verts[i*6 + 0].a = 255.0f;

        t_verts[i*6 + 1].x = t_worldPt1.x;
        t_verts[i*6 + 1].y = t_worldPt1.y;
        t_verts[i*6 + 1].z = t_worldPt1.z;
        t_verts[i*6 + 1].t0x = 0.0f;
        t_verts[i*6 + 1].t0y = 1.0f;
        t_verts[i*6 + 1].r = 0.0f;
        t_verts[i*6 + 1].g = 255.0f;
        t_verts[i*6 + 1].b = 0.0f;
        t_verts[i*6 + 1].a = 255.0f;

        t_verts[i*6 + 2].x = t_worldPt2.x;
        t_verts[i*6 + 2].y = t_worldPt2.y;
        t_verts[i*6 + 2].z = t_worldPt2.z;
        t_verts[i*6 + 2].t0x = 1.0f;
        t_verts[i*6 + 2].t0y = 0.0f;
        t_verts[i*6 + 2].r = 0.0f;
        t_verts[i*6 + 2].g = 255.0f;
        t_verts[i*6 + 2].b = 0.0f;
        t_verts[i*6 + 2].a = 255.0f;

        t_verts[i*6 + 3].x = t_worldPt2.x;
        t_verts[i*6 + 3].y = t_worldPt2.y;
        t_verts[i*6 + 3].z = t_worldPt2.z;
        t_verts[i*6 + 3].t0x = 1.0f;
        t_verts[i*6 + 3].t0y = 0.0f;
        t_verts[i*6 + 3].r = 0.0f;
        t_verts[i*6 + 3].g = 255.0f;
        t_verts[i*6 + 3].b = 0.0f;
        t_verts[i*6 + 3].a = 255.0f;

        t_verts[i*6 + 4].x = t_worldPt1.x;
        t_verts[i*6 + 4].y = t_worldPt1.y;
        t_verts[i*6 + 4].z = t_worldPt1.z;
        t_verts[i*6 + 4].t0x = 0.0f;
        t_verts[i*6 + 4].t0y = 1.0f;
        t_verts[i*6 + 4].r = 0.0f;
        t_verts[i*6 + 4].g = 255.0f;
        t_verts[i*6 + 4].b = 0.0f;
        t_verts[i*6 + 4].a = 255.0f;

        t_verts[i*6 + 5].x = t_worldPt3.x;
        t_verts[i*6 + 5].y = t_worldPt3.y;
        t_verts[i*6 + 5].z = t_worldPt3.z;
        t_verts[i*6 + 5].t0x = 1.0f;
        t_verts[i*6 + 5].t0y = 1.0f;
        t_verts[i*6 + 5].r = 0.0f;
        t_verts[i*6 + 5].g = 255.0f;
        t_verts[i*6 + 5].b = 0.0f;
        t_verts[i*6 + 5].a = 255.0f;
    }
    m_pVertData->appendData(t_verts, t_vertex_size);
    m_vertexNum += t_vertex_num;
    m_rectanglePool.clear();
}

void SVPenStroke::_drawMesh() {
    if (m_pMesh && m_pRenderObj && m_vertexNum > 0) {
        if (!m_pMtl) {
            m_pMtl = MakeSharedPtr<SVMtlStrokeBase>(mApp);
            m_pTex = mApp->getTexMgr()->getTexture("svres/textures/a_point.png",true);
            m_pMtl->setTexture(0, m_pTex);
            m_pMtl->setTexcoordFlip(1.0, -1.0);
        }
        SVStrokePoint t_min = m_aabbBox.getMin();
        SVStrokePoint t_max = m_aabbBox.getMax();
        SVStrokePoint t_posW = (t_max + t_min)*0.5f;
        m_pMtl->setQuadPosW(t_posW);
        m_pMtl->setBlendEnable(true);
        m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        m_pMtl->setModelMatrix(m_localMat);
        //更新顶点数据
        m_pMesh->setVertexDataNum(m_vertexNum);
        m_pMesh->setVertexData(m_pVertData);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(m_pMtl);
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        m_pRenderObj->pushCmd(t_rs, RST_FREETYPE, "SVPenStroke");
    }
    if (m_drawBox) {
        _drawBoundBox();
    }
}

void SVPenStroke::_drawBoundBox(){
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    SVMtlGeo3dPtr t_mtl_geo3d = MakeSharedPtr<SVMtlGeo3d>(mApp);
    t_mtl_geo3d->setColor(1.0f, 0.0f, 0.0f, 1.0f);
    FMat4 m_mat_unit = FMat4_identity;
    t_mtl_geo3d->setModelMatrix( m_mat_unit.get() ); SVRenderObjInst::pushAABBCmd(t_rs,RST_DEBUG_INNER,m_aabbBox,t_mtl_geo3d,"SV3DBOX_aabb");
}

void SVPenStroke::_screenPointToWorld(FVec2 &_point, SVStrokePoint &_worldPoint){
    SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
    FMat4 t_cameraMatrix = mainCamera->getViewMatObj();
    SVStrokePoint t_cameraEye = SVStrokePoint(t_cameraMatrix[12], t_cameraMatrix[13], t_cameraMatrix[14]);
    FVec4 t_plane = FVec4(t_cameraMatrix[2], t_cameraMatrix[6], t_cameraMatrix[10], t_cameraEye.length()+0.05);
    SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
    SVStrokePoint t_pos;
    f32 t_pt_x = _point.x;
    f32 t_pt_y = _point.y;
    f32 t_pt_z = 0.0f;
    if(t_pickModule && t_pickModule->getCrossPointWithPlane(t_pt_x, t_pt_y,t_pos, t_plane) ){
        t_pt_x = t_pos.x;
        t_pt_y = t_pos.y;
        t_pt_z = t_pos.z;
    }
    _worldPoint = SVStrokePoint(t_pt_x, t_pt_y, t_pt_z);
}

bool SVPenStroke::_judgePolygonLineIntersect(SVStrokeRectangle &_rectangle, f32 *_t){
//    float  distAB, theCos, theSin, newX;
//    
//    // FAIL: Line undefined
//    if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)) return false;
//    
//    //  Translate system to make A the origin
//    Bx-=Ax; By-=Ay;
//    Cx-=Ax; Cy-=Ay;
//    Dx-=Ax; Dy-=Ay;
//    
//    // Length of segment AB
//    distAB = sqrtf(Bx*Bx+By*By);
//    
//    // Rotate the system so that point B is on the positive X axis.
//    theCos = Bx/distAB;
//    theSin = By/distAB;
//    newX = Cx*theCos+Cy*theSin;
//    Cy  = Cy*theCos-Cx*theSin; Cx = newX;
//    newX = Dx*theCos+Dy*theSin;
//    Dy  = Dy*theCos-Dx*theSin; Dx = newX;
//    
//    // FAIL: Lines are parallel.
//    if (Cy == Dy) return false;
//    
//    // Discover the relative position of the intersection in the line AB
//    *T = (Dx+(Cx-Dx)*Dy/(Dy-Cy))/distAB;
//    
    // Success.
    return true;
}
