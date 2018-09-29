//
// SVMtlFaceShape.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlFaceShape.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectST.h"
#include "../detect/SVPerson.h"
#include "../rendercore/renderer/SVRendererBase.h"

SVMtlFaceShape::SVMtlFaceShape(SVInst *_app)
:SVMtlCore(_app,"faceShape"){
    memset(m_location0,0,2*sizeof(f32));
    memset(m_location1,0,2*sizeof(f32));
    memset(m_location2,0,2*sizeof(f32));
    memset(m_location3,0,2*sizeof(f32));
    memset(m_location4,0,2*sizeof(f32));
    memset(m_location5,0,2*sizeof(f32));
    memset(m_location6,0,2*sizeof(f32));
    memset(m_location7,0,2*sizeof(f32));
    memset(m_location8,0,2*sizeof(f32));
    memset(m_location9,0,2*sizeof(f32));
    memset(m_location10,0,2*sizeof(f32));
    memset(m_location11,0,2*sizeof(f32));
    memset(m_location12,0,2*sizeof(f32));
    memset(m_location13,0,2*sizeof(f32));
    memset(m_location14,0,2*sizeof(f32));
    memset(m_location15,0,2*sizeof(f32));
    memset(m_location16,0,2*sizeof(f32));
    memset(m_location17,0,2*sizeof(f32));
    memset(m_location18,0,2*sizeof(f32));
    memset(m_location19,0,2*sizeof(f32));
    memset(m_location20,0,2*sizeof(f32));
    memset(m_location21,0,2*sizeof(f32));
    memset(m_location22,0,2*sizeof(f32));
    memset(m_location23,0,2*sizeof(f32));
    //左眼放大的值
    m_eyeLarge[0] = 0.0;
    //右眼放大的值
    m_eyeLarge[1] = 0.0;
    //左右脸的缩小值
    m_faceSlim[0] = 1.00001f;
    //下巴的缩小值
    m_faceSlim[1] = 1.00001f;
    //左右颚的缩小值
    m_faceSlim[2] = 1.00001f;
    //左右下颚的缩小值
    m_faceSlim[3] = 1.00001f;
}

SVMtlFaceShape::SVMtlFaceShape(SVMtlFaceShape *_mtl)
:SVMtlCore(_mtl){
    m_location0[0] = _mtl->m_location0[0];
    m_location0[1] = _mtl->m_location0[1];
    m_location1[0] = _mtl->m_location1[0];
    m_location1[1] = _mtl->m_location1[1];
    m_location2[0] = _mtl->m_location2[0];
    m_location2[1] = _mtl->m_location2[1];
    m_location3[0] = _mtl->m_location3[0];
    m_location3[1] = _mtl->m_location3[1];
    m_location4[0] = _mtl->m_location4[0];
    m_location4[1] = _mtl->m_location4[1];
    m_location5[0] = _mtl->m_location5[0];
    m_location5[1] = _mtl->m_location5[1];
    m_location6[0] = _mtl->m_location6[0];
    m_location6[1] = _mtl->m_location6[1];
    m_location7[0] = _mtl->m_location7[0];
    m_location7[1] = _mtl->m_location7[1];
    m_location8[0] = _mtl->m_location8[0];
    m_location8[1] = _mtl->m_location8[1];
    m_location9[0] = _mtl->m_location9[0];
    m_location9[1] = _mtl->m_location9[1];
    m_location10[0] = _mtl->m_location10[0];
    m_location10[1] = _mtl->m_location10[1];
    m_location11[0] = _mtl->m_location11[0];
    m_location11[1] = _mtl->m_location11[1];
    m_location12[0] = _mtl->m_location12[0];
    m_location12[1] = _mtl->m_location12[1];
    m_location13[0] = _mtl->m_location13[0];
    m_location13[1] = _mtl->m_location13[1];
    m_location14[0] = _mtl->m_location14[0];
    m_location14[1] = _mtl->m_location14[1];
    m_location15[0] = _mtl->m_location15[0];
    m_location15[1] = _mtl->m_location15[1];
    m_location16[0] = _mtl->m_location16[0];
    m_location16[1] = _mtl->m_location16[1];
    m_location17[0] = _mtl->m_location17[0];
    m_location17[1] = _mtl->m_location17[1];
    m_location18[0] = _mtl->m_location18[0];
    m_location18[1] = _mtl->m_location18[1];
    m_location19[0] = _mtl->m_location19[0];
    m_location19[1] = _mtl->m_location19[1];
    m_location20[0] = _mtl->m_location20[0];
    m_location20[1] = _mtl->m_location20[1];
    m_location21[0] = _mtl->m_location21[0];
    m_location21[1] = _mtl->m_location21[1];
    m_location22[0] = _mtl->m_location22[0];
    m_location22[1] = _mtl->m_location22[1];
    m_location23[0] = _mtl->m_location23[0];
    m_location23[1] = _mtl->m_location23[1];
    //左眼放大的值
    m_eyeLarge[0] = _mtl->m_eyeLarge[0];
    //右眼放大的值
    m_eyeLarge[1] = _mtl->m_eyeLarge[1];
    //左右脸的缩小值
    m_faceSlim[0] = _mtl->m_faceSlim[0];
    //下巴的缩小值
    m_faceSlim[1] = _mtl->m_faceSlim[1];
    //左右颚的缩小值
    m_faceSlim[2] = _mtl->m_faceSlim[2];
    //左右下颚的缩小值
    m_faceSlim[3] = _mtl->m_faceSlim[3];
}

SVMtlFaceShape::~SVMtlFaceShape(){
}

SVMtlCorePtr SVMtlFaceShape::clone() {
    return PointerSharedPtr<SVMtlFaceShape>(new SVMtlFaceShape(this));
}

void SVMtlFaceShape::update(f32 dt){
    f32 fWidth =  mApp->m_pGlobalParam->m_inner_width;
    f32 fHeight =  mApp->m_pGlobalParam->m_inner_height;
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
    if(t_person){
        ST_PT *faceData = (ST_PT*)t_person->getFaceDataOriginal();
        m_location0[0] = faceData[5].x / fWidth;
        m_location0[1] = faceData[5].y / fHeight;
        
        m_location1[0] = faceData[16].x / fWidth;
        m_location1[1] = faceData[16].y / fHeight;
        
        m_location2[0] = faceData[27].x / fWidth;
        m_location2[1] = faceData[27].y / fHeight;
        
        m_location3[0] = faceData[46].x / fWidth;
        m_location3[1] = faceData[46].y / fHeight;
        
        m_location4[0] = faceData[74].x / fWidth;
        m_location4[1] = faceData[74].y / fHeight;
        
        m_location5[0] = faceData[77].x / fWidth;
        m_location5[1] = faceData[77].y / fHeight;
        
        m_location6[0] = faceData[84].x / fWidth;
        m_location6[1] = faceData[84].y / fHeight;
        
        m_location7[0] = faceData[90].x / fWidth;
        m_location7[1] = faceData[90].y / fHeight;
        
        m_location8[0] = faceData[10].x / fWidth;
        m_location8[1] = faceData[10].y / fHeight;
        
        m_location9[0] = faceData[22].x / fWidth;
        m_location9[1] = faceData[22].y / fHeight;
        
        m_location10[0] = faceData[13].x / fWidth;
        m_location10[1] = faceData[13].y / fHeight;
        
        m_location11[0] = faceData[19].x / fWidth;
        m_location11[1] = faceData[19].y / fHeight;
    }
}

void SVMtlFaceShape::_submitMtl(SVRendererBasePtr _render){
    _render->submitUniformf2v("location0", m_location0);
    _render->submitUniformf2v("location1", m_location1);
    _render->submitUniformf2v("location2", m_location2);
    _render->submitUniformf2v("location3", m_location3);
    _render->submitUniformf2v("location4", m_location4);
    _render->submitUniformf2v("location5", m_location5);
    _render->submitUniformf2v("location6", m_location6);
    _render->submitUniformf2v("location7", m_location7);
    _render->submitUniformf2v("location8", m_location8);
    _render->submitUniformf2v("location9", m_location9);
    _render->submitUniformf2v("location10", m_location10);
    _render->submitUniformf2v("location11", m_location11);
    _render->submitUniformf2v("eyelarge", m_eyeLarge);
    _render->submitUniformf4v("faceslim", m_faceSlim);
}

void SVMtlFaceShape::setEyeLarge(f32 fLeftLarge , f32 fRightLarge){
    m_eyeLarge[0] = fLeftLarge;
    m_eyeLarge[1] = fRightLarge;
}

//算法不支持1这个数值，从set上根本上规避
void SVMtlFaceShape::setFaceSlim(f32 fSlim){
    if (fabs(fSlim - 1.0) < 0.00001){
        fSlim = 1.00001;
    }
    m_faceSlim[0] = fSlim;
}

void SVMtlFaceShape::setChinUpSlim(f32 fSlim){
    if (fabs(fSlim - 1.0) < 0.00001){
        fSlim = 1.00001;
    }
    m_faceSlim[2] = fSlim;
}

void SVMtlFaceShape::setChinMiddleSlim(f32 fSlim){
    if (fabs(fSlim - 1.0) < 0.00001){
        fSlim = 1.00001;
    }
    m_faceSlim[3] = fSlim;
}

void SVMtlFaceShape::setChinSlim(f32 fSlim){
    if (fabs(fSlim - 1.0) < 0.00001){
        fSlim = 1.00001;
    }
    m_faceSlim[1] = fSlim;
}

FVec2 SVMtlFaceShape::getEyeLarge(){
    return FVec2(m_eyeLarge[0], m_eyeLarge[1]);
}

f32 SVMtlFaceShape::getFaceSlim(){
    return m_faceSlim[0];
}

f32 SVMtlFaceShape::getChinUpSlim(){
    return m_faceSlim[2];
}

f32 SVMtlFaceShape::getChinMiddleSlim(){
    return m_faceSlim[3];
}

f32 SVMtlFaceShape::getChinSlim(){
    return m_faceSlim[1];
}
