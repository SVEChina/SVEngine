//
// SVModel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModel.h"

SVModel::SVModel() {
    m_pMesh = nullptr;
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

void SVModel::setMesh(SVMeshPtr _mesh) {
    m_pMesh = _mesh;
}

SVMeshPtr SVModel::getMesh() {
    return m_pMesh;
}

void SVModel::clearMesh() {
    m_pMesh = nullptr;
}

SVBoundBox SVModel::getBox() {
    return m_box;
}

//
SVModelMgr::SVModelMgr(SVInst* _app)
:SVGBase(_app){
    
}

SVModelMgr::~SVModelMgr() {
    
}
