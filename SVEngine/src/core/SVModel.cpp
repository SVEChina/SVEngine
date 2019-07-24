//
// SVModel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModel.h"
#include "SVMesh.h"

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
    m_meshPool.destroy();
}

SVBoundBox SVModel::getBox() {
    return m_box;
}

void SVModel::update(f32 _dt) {
    for (s32 i = 0; i < m_meshPool.size(); i++) {
        m_meshPool[i]->update(_dt);
    }
}

void SVModel::render() {
    for (s32 i = 0; i < m_meshPool.size(); i++) {
        m_meshPool[i]->render();
    }
}

//
SVModelMgr::SVModelMgr(SVInst* _app)
:SVGBase(_app){
    
}

SVModelMgr::~SVModelMgr() {
    
}
