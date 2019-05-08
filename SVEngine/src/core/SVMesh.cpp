//
// SVMesh.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMesh.h"
#include "../base/SVDataSwap.h"
#include "../base/SVLock.h"
#include "../rendercore/SVRenderMesh.h"

SVMesh::SVMesh(SVInst* _app)
:SVGBase(_app){
    m_pRenderMesh = nullptr;
    m_lock = MakeSharedPtr<SVLock>();
}

SVMesh::~SVMesh() {
    m_pRenderMesh = nullptr;
    m_lock = nullptr;
}

void SVMesh::setName(cptr8 _name) {
    m_name = _name;
}

cptr8 SVMesh::getName(){
    return m_name.c_str();
}

//数据操作
void SVMesh::setData(SVDataSwapPtr _data,VFTYPE _vtf,s32 _count,s32 _seqMode) {
    m_pRenderMesh = MakeSharedPtr<SVRenderMesh>(mApp);
    m_pRenderMesh->setVertexType(_vtf);
    m_pRenderMesh->setSeqMode(_seqMode);
    m_pRenderMesh->setVertexDataNum(_count);
    m_pRenderMesh->setVertexData(_data);
    m_pRenderMesh->createMesh();
}

//子mesh操作
void SVMesh::addMesh(SVMeshPtr _mesh) {
    m_lock->lock();
    if(_mesh) {
        m_meshPool.append(_mesh);
    }
    m_lock->unlock();
}

void SVMesh::removeMesh(cptr8 _name) {
    m_lock->lock();
    for(s32 i=0;i<m_meshPool.size();i++){
        if(strcmp( m_meshPool[i]->getName(), _name) == 0){
            m_meshPool.removeForce(i);
            break;
        }
    }
    m_lock->unlock();
}

void SVMesh::clearMesh() {
    m_lock->lock();
    m_meshPool.destroy();
    m_lock->unlock();
}

void SVMesh::render() {
    //先渲染自己
    //DATAMAP m_dataMap;
    m_lock->lock();
    for(s32 i=0;i<m_meshPool.size();i++) {
        m_meshPool[i]->render();
    }
    m_lock->unlock();
}

//Morph动画的mesh
SVMorphMesh::SVMorphMesh(SVInst* _app)
:SVMesh(_app){
    
}

SVMorphMesh::~SVMorphMesh() {
    
}


