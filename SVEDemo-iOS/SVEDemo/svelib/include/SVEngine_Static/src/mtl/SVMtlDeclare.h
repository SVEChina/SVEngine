//
// SVMtlDeclare.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLDECLARE_H
#define SV_MTLDECLARE_H

#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace mtl {
        
        DECLARE_SHAREPTR(SVMtlCoreParam);
        //mtl
        DECLARE_SHAREPTR(SVMtlCore);
        DECLARE_SHAREPTR(SVMtlGeo3d);
        DECLARE_SHAREPTR(SVMtlBeauty);
        DECLARE_SHAREPTR(SVMtlMask);
        DECLARE_SHAREPTR(SVMtlRGB2YUV);
        DECLARE_SHAREPTR(SVMtlFace2d);
        //美颜
        DECLARE_SHAREPTR(SVMtlFilterBrilaterl);
        DECLARE_SHAREPTR(SVMtlFilterBrilaterl2);
        DECLARE_SHAREPTR(SVMtlGaussian);
        DECLARE_SHAREPTR(SVMtlBlurFair);
        DECLARE_SHAREPTR(SVMtlFairLtraLow);
        DECLARE_SHAREPTR(SVMtlFaceShape);
        DECLARE_SHAREPTR(SVMtlFaceShapeVaried);
        DECLARE_SHAREPTR(SVMtlImageUsingMove);
        DECLARE_SHAREPTR(SVMtlBlur);
        //滤镜
        DECLARE_SHAREPTR(SVMtlBasedOn);
        DECLARE_SHAREPTR(SVMtlAcutance);
        //texture
        DECLARE_SHAREPTR(SVTexture);
        DECLARE_SHAREPTR(SVTextureInputTexID);
        DECLARE_SHAREPTR(SVTextureSet);
        DECLARE_SHAREPTR(SVTexturePList);
        DECLARE_SHAREPTR(SVTextureIOS);
        
    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTLDECLARE_H
