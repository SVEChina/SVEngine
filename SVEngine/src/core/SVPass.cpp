//
// SVPass.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPass.h"
#include "../app/SVInst.h"
#include "../mtl/SVMtlCore.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVPass::SVPass(){
    m_pMtl = nullptr;
    m_outTex = nullptr;
    m_pMesh= nullptr;
    m_outTexType = E_TEX_END;
}

SVPass::~SVPass(){
    m_pMtl = nullptr;
    m_outTex = nullptr;
    m_outTexType = E_TEX_END;
}

bool SVPass::setInTex(s32 _index,SVTexturePtr _tex){
    if( m_pMtl ){
        m_pMtl->setTexture(_index, _tex);
        return true;
    }
    return false;
}

bool SVPass::setInTex(s32 _index,SVTEXTYPE _tt){
    if( m_pMtl ){
        m_pMtl->setTexture(_index, _tt);
        return true;
    }
    return false;
}

bool SVPass::setOutTex(SVTexturePtr _tex){
    m_outTex = _tex;
    return true;
}

bool SVPass::setOutTex(SVTEXTYPE _tt) {
    m_outTexType = _tt;
    return true;
}

void SVPass::setMtl(SVMtlCorePtr _mtl){
    m_pMtl = _mtl;
}

void SVPass::setMesh( SVRenderMeshPtr _pMesh){
    m_pMesh=_pMesh;
}

