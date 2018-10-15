//
// SVUIPanel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVUIPanel.h"
#include "../act/SVMovie.h"
#include "../act/SVDragma.h"
#include "../act/SVKeyFrame.h"
#include "../rendercore/SVRenderObject.h"

//
SVUIPanel::SVUIPanel(SVInst *_app)
:SVSpriteNode(_app) {
    ntype = "SVUIPanel";
    m_rsType = RST_UI;
    m_width = 100;
    m_height = 100;
    m_archo = E_ARCHO_CC;
    //
    m_pRenderObj = MakeSharedPtr<SVRenderObject>(mApp);
    m_pMesh = nullptr;
    m_dirty = true;
}

SVUIPanel::~SVUIPanel() {
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
}

void SVUIPanel::setSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_dirty = true;
}

void SVUIPanel::setArcho(EUIARCHO _archo) {
    m_archo = _archo;
    m_dirty = true;
}

EUIARCHO SVUIPanel::getArcho() {
    return m_archo;
}

void SVUIPanel::_computeRect() {
    FVec2 t_off;
    if( m_archo == E_ARCHO_LT ) {
        t_off.set(0.5f*m_width, -0.5f*m_height);
    }else if( m_archo == E_ARCHO_CT ) {
        t_off.set(0.0f, -0.5f*m_height);
    }else if( m_archo == E_ARCHO_RT ) {
        t_off.set(-0.5f*m_width, -0.5f*m_height);
    }else if( m_archo == E_ARCHO_LC ) {
        t_off.set(0.5f*m_width, 0.0f);
    }else if( m_archo == E_ARCHO_CC ) {
        t_off.set(0.0f, 0.0f);
    }else if( m_archo == E_ARCHO_RC ) {
        t_off.set(-0.5f*m_width, 0.0f);
    }else if( m_archo == E_ARCHO_LB ) {
        t_off.set(0.5f*m_width, 0.5f*m_height);
    }else if( m_archo == E_ARCHO_CB ) {
        t_off.set(0.0f, 0.5f*m_height);
    }else if( m_archo == E_ARCHO_RB ) {
        t_off.set(-0.5f*m_width, 0.5f*m_height);
    }
    //
    SVDataSwapPtr t_dataswap = MakeSharedPtr<SVDataSwap>(mApp);
    //渲染数据
    V2_T0 VerData[6];
    VerData[0].t0x = 0.0f;
    VerData[0].t0y = 0.0f;
    VerData[1].t0x = 1.0f;
    VerData[1].t0y = 0.0f;
    VerData[2].t0x = 0.0f;
    VerData[2].t0y = 1.0f;
    VerData[3].t0x = 0.0f;
    VerData[3].t0y = 1.0f;
    VerData[4].t0x = 1.0f;
    VerData[4].t0y = 0.0f;
    VerData[5].t0x = 1.0f;
    VerData[5].t0y = 1.0f;
    VerData[0].x = -0.5f*m_width + t_off.x;
    VerData[0].y = -0.5f*m_height + t_off.y;
    VerData[1].x = 0.5f*m_width + t_off.x;
    VerData[1].y = -0.5f*m_height + t_off.y;
    VerData[2].x = -0.5f*m_width + t_off.x;
    VerData[2].y = 0.5f*m_height + t_off.y;
    VerData[3].x = -0.5f*m_width + t_off.x;
    VerData[3].y = 0.5f*m_height + t_off.y;
    VerData[4].x = 0.5f*m_width + t_off.x;
    VerData[4].y = -0.5f*m_height + t_off.y;
    VerData[5].x = 0.5f*m_width + t_off.x;
    VerData[5].y = 0.5f*m_height + t_off.y;
    t_dataswap->writeData(&VerData[0],sizeof(V2_T0)*6);
    if(!m_pMesh){
        m_pMesh = MakeSharedPtr<SVRenderMesh>(mApp);
        m_pMesh->setVertexType(E_VF_V2_T0);
        m_pMesh->setVertexDataNum(6);
    }
    m_pMesh->setVertexData(t_dataswap);
    m_pMesh->createMesh();
    t_dataswap = nullptr;
}

void SVUIPanel::update(f32 dt) {
    SVNode::update(dt);
    if(m_dirty) {
        m_dirty = false;
        _computeRect();
    }
}

void SVUIPanel::render() {
    if ( m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVUIPanel");
        }
    }
}
