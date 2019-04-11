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
#include "../../base/SVDataSwap.h"
#include "../../rendercore/SVRenderObject.h"
#include "../../rendercore/SVRenderMesh.h"
#include "../../app/SVInst.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../../mtl/SVMtlCore.h"
SVPenStroke::SVPenStroke(SVInst *_app)
:SVGameBase(_app) {
    m_ptPool.clear();
    m_localMat.setIdentity();
    m_lock = MakeSharedPtr<SVLock>();
    m_pVertData = MakeSharedPtr<SVDataSwap>();
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_pMesh = _app->getRenderMgr()->createMeshRObj();
    m_pMesh->createMesh();
    m_pMesh->setVertexType(E_VF_V2_C);
    m_pMesh->setDrawMethod(E_DM_TRIANGLES);
    m_pointSize = 10;
    m_vertexNum = 0;
    setupLocal = false;
}

SVPenStroke::~SVPenStroke() {
    m_ptPool.clear();
    m_pVertData = nullptr;
    m_lock = nullptr;
}

//绘制一笔
void SVPenStroke::update(f32 _dt) {
    m_lock->unlock();
    //插值生成面片
//    if(1) {
        _genMesh();
//    }
    //绘制dataswap
    _drawMesh();
    m_lock->unlock();
}

void SVPenStroke::begin(f32 _px,f32 _py,f32 _pz) {
    m_lock->lock();
    m_ptPool.append(FVec3(_px,_py,_pz));
    m_lock->unlock();
}

void SVPenStroke::end(f32 _px,f32 _py,f32 _pz) {
    m_lock->lock();
    m_ptPool.append(FVec3(_px,_py,_pz));
    m_lock->unlock();
}

void SVPenStroke::draw(f32 _px,f32 _py,f32 _pz) {
    m_lock->lock();
    m_ptPool.append(FVec3(_px,_py,_pz));
    m_lock->unlock();
}

//生成数据
void SVPenStroke::_genMesh() {
    //
    s32 t_pt_num = m_ptPool.size();
    s32 t_vertex_num = t_pt_num*6;
    s32 t_vertex_size = t_vertex_num*sizeof(V2_C);
    V2_C verts[t_vertex_num];
    V2_C *t_verts = verts;
    for (s32 i = 0; i<t_pt_num; i++) {
        FVec3 t_pt = m_ptPool[i];
        t_verts[i*6 + 0].x = -m_pointSize*0.5;
        t_verts[i*6 + 0].y = m_pointSize*0.5;
        t_verts[i*6 + 0].r = 0.0f;
        t_verts[i*6 + 0].g = 255.0f;
        t_verts[i*6 + 0].b = 0.0f;
        t_verts[i*6 + 0].a = 255.0f;

        t_verts[i*6 + 1].x = -m_pointSize*0.5;
        t_verts[i*6 + 1].y = -m_pointSize*0.5;
        t_verts[i*6 + 1].r = 0.0f;
        t_verts[i*6 + 1].g = 255.0f;
        t_verts[i*6 + 1].b = 0.0f;
        t_verts[i*6 + 1].a = 255.0f;

        t_verts[i*6 + 2].x = m_pointSize*0.5;
        t_verts[i*6 + 2].y = -m_pointSize*0.5;
        t_verts[i*6 + 2].r = 0.0f;
        t_verts[i*6 + 2].g = 255.0f;
        t_verts[i*6 + 2].b = 0.0f;
        t_verts[i*6 + 2].a = 255.0f;

        t_verts[i*6 + 3].x = m_pointSize*0.5;
        t_verts[i*6 + 3].y = -m_pointSize*0.5;
        t_verts[i*6 + 3].r = 0.0f;
        t_verts[i*6 + 3].g = 255.0f;
        t_verts[i*6 + 3].b = 0.0f;
        t_verts[i*6 + 3].a = 255.0f;

        t_verts[i*6 + 4].x = m_pointSize*0.5;
        t_verts[i*6 + 4].y = m_pointSize*0.5;
        t_verts[i*6 + 4].r = 0.0f;
        t_verts[i*6 + 4].g = 255.0f;
        t_verts[i*6 + 4].b = 0.0f;
        t_verts[i*6 + 4].a = 255.0f;

        t_verts[i*6 + 5].x = -m_pointSize*0.5;
        t_verts[i*6 + 5].y = m_pointSize*0.5;
        t_verts[i*6 + 5].r = 0.0f;
        t_verts[i*6 + 5].g = 255.0f;
        t_verts[i*6 + 5].b = 0.0f;
        t_verts[i*6 + 5].a = 255.0f;
    }
    m_pVertData->appendData(t_verts, t_vertex_size);
    m_vertexNum += t_vertex_num;
    m_ptPool.clear();
}

void SVPenStroke::_drawMesh() {
    if (m_pMesh && m_pRenderObj) {
        SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "penstroke_base");
        t_mtl->setBlendEnable(true);
        t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        //
//        FMat4 t_localMat;
//        t_localMat.setIdentity();
        t_mtl->setModelMatrix(m_localMat);
//        t_mtl->update(dt);
        //更新顶点数据
        m_pMesh->setVertexDataNum(m_vertexNum);
        m_pMesh->setVertexData(m_pVertData);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(t_mtl);
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        m_pRenderObj->pushCmd(t_rs, RST_FREETYPE, "SVPenStroke");
    }
}

void SVPenStroke::setModelMatrix(FMat4 &_matrix){
    if (!setupLocal) {
        setupLocal = true;
        m_localMat = _matrix;
    }
}
