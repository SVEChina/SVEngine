//
// SVBackGroundNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBackGroundNode.h"
#include "SVScene.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVOpEvent.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../core/SVGeoGen.h"
#include "../core/SVPass.h"
#include "../app/SVGlobalMgr.h"
#include "../mtl/SVMtlFaceShape.h"
#include "../mtl/SVMtlShapeVaried.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectST.h"
#include "../core/SVImageUsingMove.h"
#include "../core/SVDeformImageMove.h"
#include "../base/SVVec2.h"

SVBackGroundNode::SVBackGroundNode(SVInst *_app)
:SVSpriteNode(_app,720,1280){
    ntype = "SVDeformNode";
    m_rsType = RST_BACKGROUND;
    m_texout=nullptr;
    m_Deform=MakeSharedPtr<SVDeformImageMove>(mApp);
}

SVBackGroundNode::~SVBackGroundNode(){
    
}

void SVBackGroundNode::destroy(){
    exit();
    if(m_Deform){
        m_Deform->destroy();
        m_Deform=nullptr;
    }
    if(m_texout){
        m_texout=nullptr;
    }
}

void SVBackGroundNode::update(f32 _dt){
    SVSpriteNode::update(_dt);
    m_Deform->update(_dt);
    setTexcoord(-1.0, -1.0);
}

void SVBackGroundNode::render(){
    m_Deform->render();
    SVSpriteNode::render();
}

void SVBackGroundNode::setTexture(SVTexturePtr _tex){
    m_texout = mApp->getTexMgr()->createUnctrlTexture(_tex->getwidth(), _tex->getheight(),GL_RGBA, GL_RGBA);
    m_Deform->init(_tex,m_texout);
    SVSpriteNode::setTexture(m_texout);
}

SVTexturePtr SVBackGroundNode::getOutTex(){
    return m_texout;
}

SVDeformImageMovePtr SVBackGroundNode::getDeform(){
    return m_Deform;
}
