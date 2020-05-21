//
// SVResVBO.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResVBO.h"
#include "../app/SVInst.h"
#include "../base/SVLock.h"
#include "SVRenderer.h"

s32 SVResVBO::getVertexFormateSize(VFTYPE _type) {
    if (_type == E_VF_V2) {
        return sizeof(V2);
    } else if (_type == E_VF_V2_T0) {
        return sizeof(V2_T0);
    } else if (_type == E_VF_V2_T0_T1) {
        return sizeof(V2_T0_T1);
    } else if (_type == E_VF_V2_C) {
        return sizeof(V2_C);
    } else if (_type == E_VF_V2_C_T0) {
        return sizeof(V2_C_T0);
    } else if (_type == E_VF_V2_C_T0_T1) {
        return sizeof(V2_C_T0_T1);
    } else if (_type == E_VF_V3) {
        return sizeof(V3);
    } else if (_type == E_VF_V3_T0) {
        return sizeof(V3_T0);
    } else if (_type == E_VF_V3_T0_T1) {
        return sizeof(V3_T0_T1);
    } else if (_type == E_VF_V3_N) {
        return sizeof(V3_N);
    } else if (_type == E_VF_V3_N_T0) {
        return sizeof(V3_N_T0);
    } else if (_type == E_VF_V3_N_T0_T1) {
        return sizeof(V3_N_T0_T1);
    } else if (_type == E_VF_V3_C) {
        return sizeof(V3_C);
    } else if (_type == E_VF_V3_C_T0) {
        return sizeof(V3_C_T0);
    } else if (_type == E_VF_V3_C_T0_T1) {
        return sizeof(V3_C_T0_T1);
    } else if (_type == E_VF_V3_N_C) {
        return sizeof(V3_N_C);
    } else if (_type == E_VF_V3_N_C_T0) {
        return sizeof(V3_N_C_T0);
    } else if (_type == E_VF_V3_N_C_T0_T1) {
        return sizeof(V3_N_C_T0_T1);
    } else if (_type == E_VF_V3_N_T0_BONE_W) {
        return sizeof(V3_N_T0_BONE4);
    } else if (_type == E_VF_V3_PARTICLE) {
        return sizeof(V3_PARTICLE);
    }else if (_type == E_VF_V3_N_T_T0) {
        return sizeof(V3_N_T_T0);
    }else if (_type == E_VF_V3_N_C_T0_BONE_W) {
        return sizeof(V3_N_C_T0_B_W);
    }
    return 0;
}

SVResVBO::SVResVBO(SVInst* _app)
:SVRObjBase(_app){
    m_uid = mApp->m_IDPool.applyUID();
}

SVResVBO::~SVResVBO(){
    mApp->m_IDPool.returnUID(m_uid);
}

void SVResVBO::create(SVRendererPtr _renderer) {
}

void SVResVBO::destroy(SVRendererPtr _renderer) {
}

void SVResVBO::render(SVRendererPtr _renderer) {
    
}

void SVResVBO::updateConf(RENDERMESHCONF& _conf) {
    
}

void SVResVBO::updateData(RENDERMESHDATA& _data) {
    
}

