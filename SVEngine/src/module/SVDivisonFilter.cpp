//
// SVDivisonFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVDivisonFilter.h"
#include "../node/SVSpriteNode.h"
#include "../node/SVScene.h"
#include "../basesys/SVSceneMgr.h"
#include "../mtl/SVMtlCore.h"
SVDivisonFilter::SVDivisonFilter(SVInst *_app)
:SVModuleBase(_app){
    m_module_name = "";
    m_isOpen = false;
}

SVDivisonFilter::~SVDivisonFilter() {
}

void SVDivisonFilter::init() {
    m_backgroundNode = MakeSharedPtr<SVSpriteNode>(mApp,100, 100);
    SVMtlCorePtr t_pMtl = MakeSharedPtr<SVMtlCore>(mApp,"screennorbgra");
    m_backgroundNode->setMaterial(t_pMtl);
}

void SVDivisonFilter::destroy() {
    m_backgroundNode = nullptr;
}

void SVDivisonFilter::update(f32 _dt) {
}

void SVDivisonFilter::open() {
    if (!m_isOpen) {
        m_isOpen = true;
        SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
        if(t_sc && m_backgroundNode){
            t_sc->addNode(m_backgroundNode);
            m_backgroundNode->setMesh(mApp->getDataMgr()->m_screenMesh);
            m_backgroundNode->setTexture(E_TEX_INSTREAM);
        }
    }
}

void SVDivisonFilter::close() {
    if (m_isOpen) {
        m_isOpen = false;
        m_backgroundNode->removeFromParent();
    }
}

bool SVDivisonFilter::isOpen(){
    return m_isOpen;
}

void SVDivisonFilter::setDivisionFilterType(SVStaticData::EFFICACYMESHTYPE _type){
    if (_type == SVStaticData::EFFICACY_MESH_TYPE_NONE) {
        m_backgroundNode->setMesh(mApp->getDataMgr()->m_screenMesh);
    }else{
        m_backgroundNode->setMesh(mApp->getDataMgr()->m_screenTwoDivisionMesh);
    }
}
