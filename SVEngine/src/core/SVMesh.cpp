//
// SVMesh.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMesh.h"

SVMesh::SVMesh() {
}

SVMesh::~SVMesh() {
}

void SVMesh::setName(cptr8 _name) {
    m_name = _name;
}

cptr8 SVMesh::getName(){
    return m_name.c_str();
}








