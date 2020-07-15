//
//  SVFaceDeform.cpp
//  SVEngineShow
//
//  Created by 徐子昱 on 2018/10/31.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVFaceDeform.h"
#include "node/SVBackGroundNode.h"
#include "core/SVDeformImageMove.h"
#include "basesys/SVSceneMgr.h"
#include "node/SVScene.h"
#include "mtl/SVTexture.h"
#include "mtl/SVTexMgr.h"
#include "rendercore/SVRenderTexture.h"
#include "rendercore/SVRenderMgr.h"
#include "rendercore/SVRendererBase.h"
#include "basesys/SVDeformMgr.h"

SVFaceDeform::SVFaceDeform(SVInst *_app)
:SVGBase(_app){
    m_deform=nullptr;
    m_node=nullptr;
}

SVFaceDeform::~SVFaceDeform(){
    m_node=nullptr;
    if(m_deform){
        m_deform->m_rule=1;
        m_deform=nullptr;
    }
}

SVDeformImageMovePtr SVFaceDeform::getDeform(){
    return m_deform;
}

SVNodePtr SVFaceDeform::createDeform(){
    if(!m_deform){
        m_deform=MakeSharedPtr<SVDeformImageMove>(mApp);
    }
    SVRendererBasePtr t_renderer =  mApp->getRenderer();
    SVTexturePtr t_innerTex = mApp->getRenderer()->getSVTex(E_TEX_MAIN);
    m_deform->init(t_innerTex,t_innerTex);
    m_deform->setIsDetect(true);
    m_deform->setvisiPoint(true);
    m_deform->setFlip(true);
    m_deform->m_rule=0;
    m_deform->pushScaleCrl(74);
    m_deform->pushScaleCrl(77);
    mApp->getDeformMgr()->pushDeform(m_deform);
    return nullptr;
}


