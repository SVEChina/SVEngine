//
// SVSkyDomeNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSkyDomeNode.h"
#include "SVScene.h"
#include "SVCameraNode.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlCore.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"

SVSkyDomeNode::SVSkyDomeNode(SVInst* _app)
:SVNode(_app)
,mDomeTex(nullptr){
    ntype = "SVSkyDomeNode";
    mMesh = MakeSharedPtr<SVRenderMesh>(mApp);
    m_renderObject = MakeSharedPtr<SVRenderObject>();
}

SVSkyDomeNode::~SVSkyDomeNode(){
    mDomeTex = nullptr;
    mMesh = nullptr;
    m_renderObject = nullptr;
}

void SVSkyDomeNode::generateSkyDome(SVTexturePtr _tex ,
                                    s32 _horiRes ,
                                    s32 _vertRes ,
                                    f32 _texCent ,
                                    f32 _sphCent ,
                                    f32 _radius){
    mDomeTex                = _tex;
    mHorizontalResolution   = _horiRes;
    mVerticalResoulution    = _vertRes;
    mTexturePercent         = _texCent;
    mSpherePercent          = _sphCent;
    mRadius                 = _radius;
    _generateMesh();
}

void SVSkyDomeNode::update(f32 _dt){
    SVNode::update(_dt);
    //mMatSkydome->enableCullFace(false);
    static f32 t_rot1 = 0.0f;
    //setRotation(t_rot1 , 0.0f, 0.0f);
    t_rot1 += 0.5f;
    if (t_rot1 > 360.0f)
        t_rot1 = 0.0f;
    SVMtlCorePtr t_mtl_sky = MakeSharedPtr<SVMtlCore>(mApp , "SkyDome");
    t_mtl_sky->m_LogicParamDepth.clear = true;
    t_mtl_sky->m_LogicParamDepth.enable = true;
    t_mtl_sky->m_LogicParamBlend.enable = true;
    t_mtl_sky->m_LogicParamBlend.srcParam = GL_ONE;
    t_mtl_sky->m_LogicParamBlend.dstParam = GL_ONE_MINUS_SRC_ALPHA;
    t_mtl_sky->setModelMatrix(m_absolutMat.get());
    t_mtl_sky->setTexture(0,mDomeTex);
    m_renderObject->setMtl(t_mtl_sky);
}

void SVSkyDomeNode::render(){
    if (!m_visible)
        return;
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVRenderScenePtr t_rs = mApp->m_pGlobalParam->m_curScene->getRenderRS();
    if (m_renderObject) {
        m_renderObject->pushCmd(t_rs, RST_SKY, "SkyDome");
    }
}

void SVSkyDomeNode::_generateMesh(){
    //水平上的step
    f32 fHoriStep = SV_PI * 2.0 / mHorizontalResolution;

    //fRealSpherePercent在0-2之间，2代表半球，1代表四分之一球
    f32 fRealSpherePercent = mSpherePercent;
    if (fRealSpherePercent < 0.0){
        fRealSpherePercent = -fRealSpherePercent;
    }
    if(fRealSpherePercent > 2.0){
        fRealSpherePercent = 2.0;
    }
    f32 fVertiStep = fRealSpherePercent * SV_HALF_PI / mVerticalResoulution;

    f32 tmpHori;
    s32   i;
    const f32 tcV = mTexturePercent / mVerticalResoulution;
    SVArray<V3_T0> VecPoint;
    for (i = 0 , tmpHori = 0.0f; i <= mHorizontalResolution ; ++i) {
        f32 elevation = SV_HALF_PI;
        const f32 tcU = (f32)i / (f32)mHorizontalResolution;
        const f32 sinA = sinf(tmpHori);
        const f32 cosA = cosf(tmpHori);

        for (s32 j = 0; j <= mVerticalResoulution; ++j) {
            const f32 cosEr = mRadius * cosf(elevation);
            V3_T0 vPoint;
            vPoint.x    = cosEr*cosA;
            vPoint.y    = cosEr*sinA;
            vPoint.z    = mRadius*sinf(elevation);
            vPoint.t0x  = tcU;
            vPoint.t0y  = j * tcV;
            VecPoint.append(vPoint);
            elevation -= fVertiStep;
        }
        tmpHori += fHoriStep;
    }
    mMesh->setVertexType(E_VF_V3_T0);
    mMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
    
    SVDataSwapPtr tmpDataSwap = MakeSharedPtr<SVDataSwap>();
    tmpDataSwap->writeData((void*)&VecPoint[0], sizeof(V3_T0) * VecPoint.size() );
    mMesh->setVertexDataNum(VecPoint.size());
    mMesh->setVertexData(tmpDataSwap);
    mMesh->createMesh();
    SVArray<u16> VecIndex;
    for (i = 0; i < mHorizontalResolution; ++i){
        VecIndex.append(mVerticalResoulution + 2 + (mVerticalResoulution + 1)*i);
        VecIndex.append(1 + (mVerticalResoulution + 1)*i);
        VecIndex.append(0 + (mVerticalResoulution + 1)*i);

        for (s32 j = 1; j < mVerticalResoulution; ++j){
            VecIndex.append(mVerticalResoulution + 2 + (mVerticalResoulution + 1)*i + j);
            VecIndex.append(1 + (mVerticalResoulution + 1)*i + j);
            VecIndex.append(0 + (mVerticalResoulution + 1)*i + j);
            VecIndex.append(mVerticalResoulution + 1 + (mVerticalResoulution + 1)*i + j);
            VecIndex.append(mVerticalResoulution + 2 + (mVerticalResoulution + 1)*i + j);
            VecIndex.append(0 + (mVerticalResoulution + 1)*i + j);
        }
    }
    mMesh->setIndexPoolType(GL_DYNAMIC_DRAW);
}




