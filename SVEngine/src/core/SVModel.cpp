//
// SVModel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModel.h"

SVModel::SVModel() {
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

void SVModel::clear() {
    meshPool.destroy();
}

void SVModel::addMesh(SVMeshPtr _mesh) {
    if(_mesh == nullptr)
        return;
    for(s32 i=0;i<meshPool.size();i++){
        if(meshPool[i] == _mesh)
            return;
    }
    meshPool.append(_mesh);
}

void SVModel::removeMesh(SVMeshPtr _mesh) {
    if(_mesh == nullptr)
        return;
    for(s32 i=0;i<meshPool.size();i++){
        if(meshPool[i] == _mesh){
            meshPool.removeForce(i);
            return ;
        }
    }
}

SVMeshPtr SVModel::getMesh(cptr8 _name) {
    for(s32 i=0;i<meshPool.size();i++){
        if(strcmp( meshPool[i]->getName(),_name) == 0){
            return meshPool[i];
        }
    }
    return nullptr;
}

SVBoundBox SVModel::getMaBox() {
    return m_box;
}
