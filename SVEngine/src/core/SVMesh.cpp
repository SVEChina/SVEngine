//
// SVMesh.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMesh.h"
#include "../base/SVDataSwap.h"

SVMesh::SVMesh(){
//    m_pDataSwap = MakeSharedPtr<SVDataSwap>();
//    m_vertType = E_VF_V3;
}

SVMesh::~SVMesh() {
    //m_pDataSwap = nullptr;
}

void SVMesh::setName(cptr8 _name) {
    m_name = _name;
}

cptr8 SVMesh::getName(){
    return m_name.c_str();
}

//数据操作
void SVMesh::setData(SVDataSwapPtr _data,VFTYPE _vtf,f32 _weight) {
    MeshData tMeshData;
    tMeshData.m_vertType = _vtf;
    tMeshData.m_pDataSwap = _data;
    m_dataMap.append(_weight,tMeshData);
}

//子mesh操作
void SVMesh::addMesh(SVMeshPtr _mesh) {
    if(_mesh) {
        m_meshPool.append(_mesh);
    }
}

void SVMesh::removeMesh(cptr8 _name) {
    for(s32 i=0;i<m_meshPool.size();i++){
        if(strcmp( m_meshPool[i]->getName(), _name) == 0){
            m_meshPool.removeForce(i);
            return ;
        }
    }
}

void SVMesh::clearMesh() {
    m_meshPool.destroy();
}







