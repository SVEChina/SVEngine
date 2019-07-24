//
// SVModel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModel.h"

SVModel::SVModel(SVInst* _app)
:SVGBase(_app){
    m_meshPool.clear();
}

SVModel::~SVModel() {
    clear();
}

cptr8 SVModel::getName(){
    return m_name.c_str();
}

void SVModel::setName(cptr8 _name){
    m_name = _name;
}

void SVModel::addMesh(SVMeshPtr _mesh) {
    m_meshPool.append(_mesh);
    //m_pMesh = _mesh;
}

SVMeshPtr SVModel::getMesh(s32 _index) {
    return nullptr;
}

void SVModel::clearMesh() {
    m_meshPool.clear();
}

SVBoundBox SVModel::getBox() {
    return m_box;
}

void SVModel::update(f32 _dt) {
//    if(m_pMesh) {
//        m_pMesh->update(_dt);
//    }
}

void SVModel::render() {
//    if( m_pMesh ) {
//        m_pMesh->render();
//    }
}

//
SVModelMgr::SVModelMgr(SVInst* _app)
:SVGBase(_app){
    
}

SVModelMgr::~SVModelMgr() {
    
}
