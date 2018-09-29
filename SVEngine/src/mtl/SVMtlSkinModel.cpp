//
// SVMtlSkinModel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlSkinModel.h"
#include "../rendercore/renderer/SVRendererBase.h"

#ifdef CONFIG_IS_LOAD_ASSIMP

#include "assimp/anim.h"

SVMtlSkinModel::SVMtlSkinModel(SVInst *_app)
:SVMtlCore(_app,"SkinModel") {
    memset(m_boneLocation, 0, sizeof(m_boneLocation));
}

SVMtlSkinModel::SVMtlSkinModel(SVMtlSkinModel* _mtl)
:SVMtlCore(_mtl){
    for(s32 i=0;i<MAX_BONES;i++){
        m_boneLocation[i] = _mtl->m_boneLocation[i];
    }
    //SVArray<aiMatrix4x4> m_vecBoneMatrix;
}

SVMtlSkinModel::~SVMtlSkinModel() {
}

SVMtlCorePtr SVMtlSkinModel::clone() {
    return PointerSharedPtr<SVMtlSkinModel>(new SVMtlSkinModel(this));
}

void SVMtlSkinModel::setBoneTransform(s32 iIndex, const aiMatrix4x4 &Transform) {
    if (iIndex >= MAX_BONES)
        return;
    m_vecBoneMatrix.set_used(iIndex + 1);
    m_vecBoneMatrix[iIndex] = Transform;
}

void SVMtlSkinModel::update(f32 dt) {
}

bool SVMtlSkinModel::submitMtl() {
    //return;
    m_LogicParamDepth.enable = true;
    m_LogicParamDepth.testMethod = GL_LEQUAL;
//    int iError = GLERROR;
    bool bSuc = SVMtlCore::submitMtl();
    if (bSuc == false)
        return false;
    
//    iError = GLERROR;
    s32 iSize =m_vecBoneMatrix.size();
    if (iSize > MAX_BONES){
        iSize = MAX_BONES;
    }
    //
    for (u32 i = 0; i < iSize; i++) {
        aiMatrix4x4 identity;
        s32 iSize = sizeof(ai_real);
        identity = m_vecBoneMatrix[i];
        identity.Transpose();
        //
        char Name[128];
        memset(Name, 0, sizeof(Name));
        sprintf(Name, "gBones[%d]", i);
        m_boneLocation[i] = glGetUniformLocation(m_programID, Name);
        _render->submitUniformf4v("u_color", (const GLfloat *) &identity);
        glUniformMatrix4fv(m_boneLocation[i], 1, GL_FALSE, );
    }
    return true;
}

//
SVMatSkinModelSelect::SVMatSkinModelSelect(SVInst *_app)
:SVMtlSkinModel(_app){
}

SVMatSkinModelSelect::SVMatSkinModelSelect(SVMatSkinModelSelect* _mtl)
:SVMtlCore(_mtl){
    mColor = _mtl->mColor;
}

SVMtlCorePtr SVMatSkinModelSelect::clone() {
    return PointerSharedPtr<SVMatSkinModelSelect>(new SVMatSkinModelSelect(this));
}

void SVMatSkinModelSelect::setColor(u32 colorid) {
    mColor.setColorARGB(colorid);
}

void SVMatSkinModelSelect::_submitMtl(SVRendererBasePtr _render) {
    _render->submitUniformf4("u_color", mColor.r, mColor.g, mColor.b, mColor.a);
}

#endif //CONFIG_IS_LOAD_ASSIMP
