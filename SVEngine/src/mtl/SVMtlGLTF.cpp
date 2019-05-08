//
// SVMtlGLTF.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlGLTF.h"
#include "../mtl/SVTexture.h"

SVMtlGLTF::SVMtlGLTF(SVInst *_app)
:SVMtlCore(_app,"mask2d") {
}

SVMtlGLTF::SVMtlGLTF(SVMtlGLTF *_mtl)
:SVMtlCore(_mtl){
}

SVMtlGLTF::~SVMtlGLTF() {
}

SVMtlCorePtr SVMtlGLTF::clone() {
    return PointerSharedPtr<SVMtlGLTF>(new SVMtlGLTF(this));
}

//void SVMtlGLTF::update(float dt) {
//}
