//
// SVPhysicsBodySoft.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//
#include "SVPhysicsBodySoft.h"
#include "SVPhysicsBodyRigid.h"
#include "../../base/SVVec3.h"
SVPhysicsBodySoft::SVPhysicsBodySoft(SVInst* _app):SVPhysicsBody(_app) {
    m_type = E_PHYSICS_BODY_SOFT;
    m_softBody = nullptr;
}

SVPhysicsBodySoft::~SVPhysicsBodySoft() {
    m_softBody = nullptr;
}

void SVPhysicsBodySoft::init(){
    
}

void SVPhysicsBodySoft::destroy(){
    if (m_softBody) {
        delete m_softBody;
    }
}

void SVPhysicsBodySoft::update(f32 _dt){
    
}

btSoftBody* SVPhysicsBodySoft::getBody(){
    return m_softBody;
}

void SVPhysicsBodySoft::setTotalMass(f32 _mass){
    if (m_softBody) {
        m_softBody->setTotalMass(_mass);
    }
}

void SVPhysicsBodySoft::setMass(s32 _bulletnode, f32 _mass){
    if (m_softBody) {
        m_softBody->setMass(_bulletnode, _mass);
    }
}

void SVPhysicsBodySoft::setTimeScale(f32 _timescale){
    if (m_softBody) {
         m_softBody->m_cfg.timescale = _timescale;
    }
}

void SVPhysicsBodySoft::appendAnchor(s32 _node,
                  SVPhysicsBodyRigidPtr _body, bool _disableCollisionBetweenLinkedBodies, f32 _influence){
    if (m_softBody) {
        m_softBody->appendAnchor(_node, _body->getBody(), _disableCollisionBetweenLinkedBodies, _influence);
    }
}

void SVPhysicsBodySoft::getNodes(SVSoftBodyNodeArray &_nodearray){
    if (m_softBody) {
        for (s32 i = 0; i < m_softBody->m_nodes.size(); i++) {
            const btSoftBody::Node& n = m_softBody->m_nodes[i];
            _nodearray.append(n);
        }
    }
}

s32 SVPhysicsBodySoft::getNodesSize(){
    if (m_softBody) {
        return m_softBody->m_nodes.size();
    }
    return 0;
}

void SVPhysicsBodySoft::getLinks(SVSoftBodyLinkArray &_linkarray){
    if (m_softBody) {
        for (s32 i = 0; i < m_softBody->m_links.size(); i++) {
            const btSoftBody::Link& l = m_softBody->m_links[i];
            _linkarray.append(l);
        }
    }
}

s32 SVPhysicsBodySoft::getLinksSize(){
    if (m_softBody) {
        return m_softBody->m_links.size();
    }
    return 0;
}

void SVPhysicsBodySoft::getFaces(SVSoftBodyFaceArray &_facearray){
    if (m_softBody) {
        for (s32 i = 0; i < m_softBody->m_faces.size(); i++) {
            const btSoftBody::Face& f = m_softBody->m_faces[i];
            _facearray.append(f);
        }
    }
}

s32 SVPhysicsBodySoft::getFacesSize(){
    if (m_softBody) {
        return m_softBody->m_faces.size();
    }
    return 0;
}

