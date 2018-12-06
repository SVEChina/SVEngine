//
// SVTransGPU.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVTransGPU.h"
#include "../node/SVCameraNode.h"
#include "../node/SVScene.h"
#include "../node/SVFacePointNode.h"
#include "../node/SVMultPassNode.h"
#include "../core/SVPass.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../rendercore/renderer/SVContextBase.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../core/SVGeoGen.h"

SVTransGPU::SVTransGPU(SVInst *_app)
        : SVTrans(_app) {
    m_texY = nullptr;
    m_texU = nullptr;
    m_texV = nullptr;
    m_texUV = nullptr;
    m_texGray = nullptr;
    m_texBGRA = nullptr;
    //
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_passNode = MakeSharedPtr<SVMultPassNode>(mApp);
    m_pMtl = nullptr;
    m_pMesh = nullptr;
    m_beload = false;

}

SVTransGPU::~SVTransGPU() {
    destroy();
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
}

void SVTransGPU::init(s32 _w, s32 _h, f32 _angle, PICFORMATE _picformate, SVTEXTYPE _tt) {
    SVTrans::init(_w, _h, _angle, _picformate, _tt);
    //
    if (_picformate == SV_PF_GRAY8) {
        m_texGray = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w,
                                                                       _h,
                                                                       GL_LUMINANCE,
                                                                       GL_LUMINANCE);
        mApp->getRenderMgr()->pushRCmdCreate(m_texGray);
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2y");
        m_pMtl->setTexture(0, m_texY);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_YV12) {
        m_texY = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w,
                                                                    _h,
                                                                    GL_LUMINANCE,
                                                                    GL_LUMINANCE);
        m_texUV = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w / 2,
                                                                     _h / 2,
                                                                     GL_LUMINANCE_ALPHA,
                                                                     GL_LUMINANCE_ALPHA);
        mApp->getRenderMgr()->pushRCmdCreate(m_texY);
        mApp->getRenderMgr()->pushRCmdCreate(m_texUV);
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2rgbyv12");
        m_pMtl->setTexture(0, m_texY);
        m_pMtl->setTexture(1, m_texU);
        m_pMtl->setTexture(2, m_texV);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_I420) {
        m_texY = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w,
                                                                    _h,
                                                                    GL_LUMINANCE,
                                                                    GL_LUMINANCE);
        m_texU = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w,
                                                                    _h,
                                                                    GL_LUMINANCE,
                                                                    GL_LUMINANCE);
        m_texV = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w,
                                                                    _h,
                                                                    GL_LUMINANCE,
                                                                    GL_LUMINANCE);
        mApp->getRenderMgr()->pushRCmdCreate(m_texY);
        mApp->getRenderMgr()->pushRCmdCreate(m_texU);
        mApp->getRenderMgr()->pushRCmdCreate(m_texV);
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2rgbi420");
        m_pMtl->setTexture(0, m_texY);
        m_pMtl->setTexture(1, m_texU);
        m_pMtl->setTexture(2, m_texV);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_NV12) {
        m_texY = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w,
                                                                    _h,
                                                                    GL_LUMINANCE,
                                                                    GL_LUMINANCE);
        m_texUV = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w / 2,
                                                                     _h / 2,
                                                                     GL_LUMINANCE_ALPHA,
                                                                     GL_LUMINANCE_ALPHA);
        //
        mApp->getRenderMgr()->pushRCmdCreate(m_texY);
        mApp->getRenderMgr()->pushRCmdCreate(m_texUV);
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2rgb12");
        m_pMtl->setTexture(0, m_texY);
        m_pMtl->setTexture(1, m_texUV);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_NV21) {
        m_texY = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w,
                                                                    _h,
                                                                    GL_LUMINANCE,
                                                                    GL_LUMINANCE);
        m_texUV = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w / 2,
                                                                     _h / 2,
                                                                     GL_LUMINANCE_ALPHA,
                                                                     GL_LUMINANCE_ALPHA);
        mApp->getRenderMgr()->pushRCmdCreate(m_texY);
        mApp->getRenderMgr()->pushRCmdCreate(m_texUV);
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "yuv2rgb21");
        m_pMtl->setTexture(0, m_texY);
        m_pMtl->setTexture(1, m_texUV);
        createPass(_w,_h,_angle,_tt);
    } else if (_picformate == SV_PF_BGRA) {
//        m_texBGRA = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTexture(_w,_h,GL_BGRA,GL_RGBA);
//        m_pMesh = mApp->getDataMgr()->m_screenMeshRot90;
//        mApp->getRenderMgr()->pushRCmdCreate(m_texBGRA);
//        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"yuv2rgb21");
    } else if (_picformate == SV_PF_RGBA) {

    } else if (_picformate == SV_PF_RGB) {
    }
//    //

}

void SVTransGPU::destroy() {
    m_texY = nullptr;
    m_texUV = nullptr;
    m_passNode = nullptr;
}

void SVTransGPU::pushData(u8 *_srcPtr) {
    m_beload = true;
    if (m_picformate == SV_PF_GRAY8) {

    } else if (m_picformate == SV_PF_YV12) {
        m_texY->setTexData(_srcPtr, m_width * m_height);
        m_texUV->setTexData(_srcPtr + m_width * m_height, m_width * m_height / 2);
    } else if (m_picformate == SV_PF_I420) {
        m_texY->setTexData(_srcPtr, m_width * m_height);
//        m_texU->setTexData(_srcPtr+m_width*m_height,m_width*m_height/2);
//        m_texV->setTexData(_srcPtr+m_width*m_height,m_width*m_height/2);
    } else if (m_picformate == SV_PF_NV12) {
        m_texY->setTexData(_srcPtr, m_width * m_height);
        m_texUV->setTexData(_srcPtr + m_width * m_height, m_width * m_height / 2);
    } else if (m_picformate == SV_PF_NV21) {
        m_texY->setTexData(_srcPtr, m_width * m_height);
        m_texUV->setTexData(_srcPtr + m_width * m_height, m_width * m_height / 2);
    } else if (m_picformate == SV_PF_BGRA) {
        SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_texTT);
        if (t_tex) {
            t_tex->pushData(_srcPtr, m_width, m_height, GL_RGBA);
        }
    } else if (m_picformate == SV_PF_RGBA) {
        SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_texTT);
        if (t_tex) {
            t_tex->pushData(_srcPtr, m_width, m_height, GL_RGBA);
        }
    } else if (m_picformate == SV_PF_RGB) {
    }
}

void SVTransGPU::update(f32 dt) {
    if (!m_beload) {
        return;
    }
    if (m_pMtl) {
        m_pMtl->setTexcoordFlip(1.0f, -1.0f);
        if (m_picformate == SV_PF_GRAY8) {

        } else if (m_picformate == SV_PF_YV12) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_texY);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_texY->getwidth(), 1.0f / m_texY->getheight());
            m_pMtl->setTexture(1, m_texUV);
            m_pMtl->setTexSizeIndex(1, 1.0f / (m_texUV->getwidth() / 2),
                                    1.0f / (m_texUV->getheight() / 2));
        } else if (m_picformate == SV_PF_I420) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_texY);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_texY->getwidth(), 1.0f / m_texY->getheight());
            m_pMtl->setTexture(1, m_texUV);
            m_pMtl->setTexSizeIndex(1, 1.0f / (m_texUV->getwidth() / 2),
                                    1.0f / (m_texUV->getheight() / 2));
        } else if (m_picformate == SV_PF_NV12) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_texY);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_texY->getwidth(), 1.0f / m_texY->getheight());
            m_pMtl->setTexture(1, m_texUV);
            m_pMtl->setTexSizeIndex(1, 1.0f / (m_texUV->getwidth() / 2),
                                    1.0f / (m_texUV->getheight() / 2));
        } else if (m_picformate == SV_PF_NV21) {
            m_pMtl->setTexcoordFlip(1.0,1.0);
            m_pMtl->setTexture(0, m_texY);
            m_pMtl->setTexSizeIndex(0, 1.0f / m_texY->getwidth(), 1.0f / m_texY->getheight());
            m_pMtl->setTexture(1, m_texUV);
            m_pMtl->setTexSizeIndex(1, 1.0f / (m_texUV->getwidth() / 2),
                                    1.0f / (m_texUV->getheight() / 2));
        } else if (m_picformate == SV_PF_BGRA) {
            return;
        } else if (m_picformate == SV_PF_RGBA) {
            return;
        } else if (m_picformate == SV_PF_RGB) {
        }
        //m_pMtl->setModelMatrix(m_absolutMat.get());
        m_pMtl->update(dt);
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
    t_pass->setMtl(m_pMtl);
    if (_angle == 90) {
        t_pass->setMesh(mApp->getDataMgr()->m_screenMeshRot90);
    } else if (_angle == 270) {
        t_pass->setMesh(mApp->getDataMgr()->m_screenMeshRot270);
    } else if(_angle == 180) {
        t_pass->setMesh(mApp->getDataMgr()->m_screenMeshRot180);
    }
    t_pass->setOutTex(_tt);
    m_passNode->addPass(t_pass);
    SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
    if (t_sc && m_passNode) {
        t_sc->addNode(m_passNode);
    }
}
