//
// SVTransGPU.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVTransGPU.h"
#include "../node/SVCameraNode.h"
#include "../node/SVScene.h"
#include "../node/SVMultPassNode.h"
#include "../core/SVPass.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderer.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../core/SVGeoGen.h"

SVTransGPU::SVTransGPU(SVInst *_app,
                       SVTexturePtr _t0,
                       SVTexturePtr _t1,
                       SVTexturePtr _t2,
                       SVTexturePtr _tOut)
:SVTrans(_app)
,m_tex0(_t0)
,m_tex1(_t1)
,m_tex2(_t2)
,m_texOut(_tOut){
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_passNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_passNode->setRSType(RST_SKY);
    m_pMtl = nullptr;
    m_pMesh = nullptr;
}

SVTransGPU::~SVTransGPU() {
    destroy();
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
}

void SVTransGPU::init(s32 _w, s32 _h, f32 _angle, PICFORMATE _picformate, SVTEXTYPE _tt) {
    SVTrans::init(_w, _h, _angle, _picformate, _tt);
    if (_picformate == SV_PF_GRAY8) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2y");
        m_pMtl->setTexture(0, m_tex0);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_YV12) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2rgbyv12");
        m_pMtl->setTexture(0, m_tex0);
        m_pMtl->setTexture(1, m_tex1);
        m_pMtl->setTexture(2, m_tex2);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_I420) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2rgbi420");
        m_pMtl->setTexture(0, m_tex0);
        m_pMtl->setTexture(1, m_tex1);
        m_pMtl->setTexture(2, m_tex2);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_NV12) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2rgb12");
        m_pMtl->setTexture(0, m_tex0);
        m_pMtl->setTexture(1, m_tex1);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_NV21) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2rgb21");
        m_pMtl->setTexture(0, m_tex0);
        m_pMtl->setTexture(1, m_tex1);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_BGRA) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "bgra2rgba");
        m_pMtl->setTexture(0, m_tex0);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_RGBA) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "rgba");
        m_pMtl->setTexture(0, m_tex0);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_RGB) {
    }
}

void SVTransGPU::destroy() {
    m_tex0 = nullptr;
    m_tex1 = nullptr;
    m_tex2 = nullptr;
    m_texOut = nullptr;
    m_passNode = nullptr;
}

void SVTransGPU::update(f32 dt) {
    if (m_pMtl) {
        m_pMtl->setTexcoordFlip(1.0f, -1.0f);
        if (m_picformate == SV_PF_GRAY8) {
 
        } else if (m_picformate == SV_PF_YV12) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_tex0);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_tex0->getwidth(), 1.0f / m_tex0->getheight());
            m_pMtl->setTexture(1, m_tex1);
            m_pMtl->setTexSizeIndex(1, 1.0f / (m_tex1->getwidth() / 2),
                                    1.0f / (m_tex1->getheight() / 2));
        } else if (m_picformate == SV_PF_I420) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_tex0);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_tex0->getwidth(), 1.0f / m_tex0->getheight());
            m_pMtl->setTexture(1, m_tex1);
            m_pMtl->setTexSizeIndex(1, 1.0f / (m_tex1->getwidth() / 2),
                                    1.0f / (m_tex1->getheight() / 2));
            m_pMtl->setTexture(1, m_tex2);
            m_pMtl->setTexSizeIndex(1, 1.0f / (m_tex2->getwidth() / 2),
                                    1.0f / (m_tex2->getheight() / 2));
        } else if (m_picformate == SV_PF_NV12) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_tex0);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_tex0->getwidth(), 1.0f / m_tex0->getheight());
            m_pMtl->setTexture(1, m_tex1);
            m_pMtl->setTexSizeIndex(1, 1.0f / (m_tex1->getwidth() / 2),
                                    1.0f / (m_tex1->getheight() / 2));
        } else if (m_picformate == SV_PF_NV21) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_tex0);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_tex0->getwidth(), 1.0f / m_tex0->getheight());
            m_pMtl->setTexture(1, m_tex1);
            m_pMtl->setTexSizeIndex(1, 1.0f / (m_tex1->getwidth() / 2),
                                    1.0f / (m_tex1->getheight() / 2));
        } else if (m_picformate == SV_PF_BGRA) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_tex0);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_tex0->getwidth(), 1.0f / m_tex0->getheight());
        } else if (m_picformate == SV_PF_RGBA) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_tex0);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_tex0->getwidth(), 1.0f / m_tex0->getheight());
        } else if (m_picformate == SV_PF_RGB) {
        }
        m_pMtl->update(dt);
    }
    if (m_passNode) {
        m_passNode->update(0.0f);
    }
}

void SVTransGPU::render(){
    if (m_passNode) {
        m_passNode->render();
    }
}

void SVTransGPU::setAngle(f32 _angle){
    if(_angle!=m_angle){
        m_angle=_angle;
        SVPassPtr t_pass=m_passNode->getPass(0);
        if (_angle == 90) {
            t_pass->setMesh(mApp->getDataMgr()->m_screenMeshRot90);
        } else if (_angle == 270) {
            t_pass->setMesh(mApp->getDataMgr()->m_screenMeshRot270);
        } else if(_angle == 180) {
            t_pass->setMesh(mApp->getDataMgr()->m_screenMeshRot180);
        }
    }
}

void SVTransGPU::createPass(s32 _w, s32 _h, f32 _angle, SVTEXTYPE _tt) {
    m_passNode->create(_w, _h);
    SVPassPtr t_pass = MakeSharedPtr<SVPass>();
    m_pMtl->setTexcoordFlip(1.0,1.0);
    m_pMtl->setDepthEnable(false);
    m_pMtl->setTextureParam(0,E_T_PARAM_WRAP_S,E_T_WRAP_CLAMP_TO_EDAGE);
    m_pMtl->setTextureParam(0,E_T_PARAM_WRAP_T,E_T_WRAP_CLAMP_TO_EDAGE);
    m_pMtl->setTextureParam(1,E_T_PARAM_WRAP_S,E_T_WRAP_CLAMP_TO_EDAGE);
    m_pMtl->setTextureParam(1,E_T_PARAM_WRAP_T,E_T_WRAP_CLAMP_TO_EDAGE);
    m_pMtl->setTextureParam(2,E_T_PARAM_WRAP_S,E_T_WRAP_CLAMP_TO_EDAGE);
    m_pMtl->setTextureParam(2,E_T_PARAM_WRAP_T,E_T_WRAP_CLAMP_TO_EDAGE);
    t_pass->setMtl(m_pMtl);
    t_pass->mTag = "SVTransGPU";
    if (_angle == 90) {
        t_pass->setMesh(mApp->getDataMgr()->m_screenMeshRot90);
    } else if (_angle == 270) {
        t_pass->setMesh(mApp->getDataMgr()->m_screenMeshRot270);
    } else if(_angle == 180) {
        t_pass->setMesh(mApp->getDataMgr()->m_screenMeshRot180);
    }
    t_pass->setOutTex(_tt);
    m_passNode->addPass(t_pass);
}
