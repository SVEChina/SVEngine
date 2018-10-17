//
// SVObjLoader.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVObjLoader.h"
#include "../core/SVModel.h"

SVObjLoader::SVObjLoader(SVInst *_app)
:SVFileLoader(_app) {
}

SVModelPtr SVObjLoader::loadModel(cptr8 _fname) {
    SVModelPtr t_model = MakeSharedPtr<SVModel>();
    return t_model;
}
