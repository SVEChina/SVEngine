//
// SVModel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModel.h"
#include "SVMesh.h"
#include "../mtl/SVMtlGLTF.h"

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
    if(m_meshPool.size() == 1) {
        m_box = _mesh->getBox();
    }else{
        m_box.expand(_mesh->getBox());
    }
}

SVMeshPtr SVModel::getMesh(s32 _index) {
    if(_index<0 || _index>=m_meshPool.size() )
        return nullptr;
    return m_meshPool[_index];
}

void SVModel::clearMesh() {
    m_meshPool.destroy();
}

SVBoundBox SVModel::getBox() {
    return m_box;
}

void SVModel::update(f32 _dt,FMat4& _mat) {
    for (s32 i = 0; i < m_meshPool.size(); i++) {
        m_meshPool[i]->update(_dt,_mat);
    }
}

void SVModel::render() {
    for (s32 i = 0; i < m_meshPool.size(); i++) {
        m_meshPool[i]->render();
    }
}

void SVModel::createShadow() {
    for (s32 i = 0; i < m_meshPool.size(); i++) {
        m_meshPool[i]->createShadow();
    }
}

void SVModel::bindSke(SVSkeletonPtr _ske) {
    for (s32 i = 0; i < m_meshPool.size(); i++) {
        SVMtlCorePtr t_mtl = m_meshPool[i]->getMtl();
        SVMtlGLTFSkinPtr skin_mtl = DYN_TO_SHAREPTR(SVMtlGLTFSkin, t_mtl);
        if(skin_mtl) {
            skin_mtl->bindSke(_ske);
        }
    }
}

void SVModel::unbindSke() {
    for (s32 i = 0; i < m_meshPool.size(); i++) {
        SVMtlCorePtr t_mtl = m_meshPool[i]->getMtl();
        SVMtlGLTFSkinPtr skin_mtl = DYN_TO_SHAREPTR(SVMtlGLTFSkin, t_mtl);
        if(skin_mtl) {
            skin_mtl->unbindSke();
        }
    }
}
