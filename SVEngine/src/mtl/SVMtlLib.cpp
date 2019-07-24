//
// SVMtlLib.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlLib.h"
#include "SVMtlGLTF.h"

SVMtlCorePtr SVMtlLib::m_pSkinMtl = nullptr;

void SVMtlLib::clear() {
    m_pSkinMtl = nullptr;
}

SVMtlCorePtr SVMtlLib::getSkinMtl(SVInst* _app) {
    if(m_pSkinMtl)
        return m_pSkinMtl;
    
    m_pSkinMtl = MakeSharedPtr<SVMtlGLTF>(_app);
    //
    FMat4 tMat_rotx;
    tMat_rotx.setIdentity();
    tMat_rotx.setRotateX(45.0f);

    FMat4 tMat_roty;
    tMat_roty.setIdentity();
    tMat_roty.setRotateY(45.0f);

    FMat4 tMat_sc;
    tMat_sc.setIdentity();
    tMat_sc.setScale(FVec3(200.0f,200.0f,200.0f));
    //
    FMat4 tMat = tMat_sc * tMat_roty * tMat_rotx;
    m_pSkinMtl->setModelMatrix(tMat.get());
    m_pSkinMtl->setBlendEnable(false);
    m_pSkinMtl->setDepthEnable(true);
    //m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    //m_pMtl->setBlendMode(SVMtlAni2D::SV_MTL_BLENDMODE_NORMAL);
    m_pSkinMtl->update(0.03f);
    //
    return m_pSkinMtl;
}

SVMtlCorePtr SVMtlLib::get3DNorMtl(SVInst* _app) {
SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlGLTF>(_app);
return t_mtl;
}
