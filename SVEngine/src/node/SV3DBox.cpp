//
// SV3DBox.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SV3DBox.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderMgr.h"
#include "../core/SVGeoGen.h"
#include "../mtl/SVMtlNocolor.h"

SV3DBox::SV3DBox(SVInst *_app)
: SVNode(_app) {
    ntype = "SV3DBox";
    m_aabbBox.clear();
    m_aabbBox.expand(FVec3(50.0f, 50.0f, 50.0f));
    m_aabbBox.expand(FVec3(-50.0f, -50.0f, -50.0f));
    //
    m_pMesh = SVGeoGen::genAABB(mApp,m_aabbBox);
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_mtl_box = MakeSharedPtr<SVMtlGeo3d>(mApp);
    m_drawBox = true;
    m_rsType = RST_SOLID_3D;
}

SV3DBox::~SV3DBox() {
    m_pRenderObj = nullptr;
    m_pMesh = nullptr;
    m_pMtl = nullptr;
}

void SV3DBox::setMesh(SVRenderMeshPtr _pMesh){
    m_pMesh = _pMesh;
}

void SV3DBox::randomInit(){
    //
    f32 t_sc = 1.0 + rand()%300*0.01f;
    t_sc = 1.0f;
    setScale(t_sc, t_sc, t_sc);
    //
    f32 t_x = rand()%600 - 360.0f;
    f32 t_y = rand()%1000 - 640.0f;
    f32 t_z = rand()%1000 - 500.0f;
    setPosition(t_x, t_y, -t_z);
    setPosition(0.0f,0.0f,0.0f);
    //
    f32 t_rotx = rand()%360;
    f32 t_roty = rand()%360;
    f32 t_rotz = rand()%360;
    setRotation(t_rotx, t_roty, t_rotz);
    //
    f32 t_r = rand()%1000*0.001f;
    f32 t_g = rand()%1000*0.001f;
    f32 t_b = rand()%1000*0.001f;
    f32 t_a = 1.0f;
    m_color.setColor(t_r, t_g, t_b, t_a);

    //m_drawBox
    m_drawBox = true;
    m_canSelect = true;
}

SVMtlCorePtr SV3DBox::getMtl(){
    return m_mtl_box;
}

void SV3DBox::update(f32 dt) {
    SVNode::update(dt);
    if (m_pRenderObj && m_pMesh) {
        //材质独立化
        if(m_pMtl){
            m_pMtl->update(dt);
            m_pMtl->setDepthEnable(true);
            m_pMtl->setModelMatrix(m_absolutMat.get());
            m_pMtl->setTexcoordFlip(1.0, 1.0);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(m_pMtl);
        }else{
            m_mtl_box->setColor(m_color.r, m_color.g, m_color.b, m_color.a);
            m_mtl_box->update(dt * 0.001f);
            m_mtl_box->setModelMatrix(m_absolutMat.get());
            m_mtl_box->setTexcoordFlip(1.0, -1.0f);
            m_mtl_box->setDepthEnable(true);
            m_mtl_box->setBlendEnable(true);
            m_mtl_box->setBlendState(MTL_BLEND_SRC_ALPHA,MTL_BLEND_ONE_MINUS_SRC_ALPHA);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(m_mtl_box);
        }
    }
}

void SV3DBox::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SV3DBox");
        }
    }
    SVNode::render();
}










