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
        DECLARE_SHAREPTR(SVMtlBrightnessContrast);
        DECLARE_SHAREPTR(SVMtlSaturation);
        DECLARE_SHAREPTR(SVMtlVibrance);
        DECLARE_SHAREPTR(SVMtlGeo3d);
        DECLARE_SHAREPTR(SVMtlBeauty);
        DECLARE_SHAREPTR(SVMtlMask);
        DECLARE_SHAREPTR(SVMtlRGB2YUV);
        DECLARE_SHAREPTR(SVMtlFace2D);
        DECLARE_SHAREPTR(SVMtlAni2D);
        //美颜
        DECLARE_SHAREPTR(SVMtlFilterBrilaterl);
        DECLARE_SHAREPTR(SVMtlFilterBrilaterl2);
        DECLARE_SHAREPTR(SVMtlGaussian);
        DECLARE_SHAREPTR(SVMtlBlurFair);
        DECLARE_SHAREPTR(SVMtlFairLtraLow);
        DECLARE_SHAREPTR(SVMtlFaceShape);
        DECLARE_SHAREPTR(SVMtlFaceShapeVaried);
        DECLARE_SHAREPTR(SVMtlBlur);
        //
        DECLARE_SHAREPTR(SVMtlParticleAni);
        //滤镜
        DECLARE_SHAREPTR(SVMtlBasedOn);
        DECLARE_SHAREPTR(SVMtlAcutance);
        //texture
        DECLARE_SHAREPTR(SVTexture);
        DECLARE_SHAREPTR(SVTextureInputTexID);
        DECLARE_SHAREPTR(SVTextureSet);
        DECLARE_SHAREPTR(SVTexturePList);
        DECLARE_SHAREPTR(SVTextureIOS);
        //modify
        DECLARE_SHAREPTR(SVGLModify);
        DECLARE_SHAREPTR(SVGLModifyTex);
        DECLARE_SHAREPTR(SVGLModifyTexParam);
        DECLARE_SHAREPTR(SVGLModifyLineWidth);
        DECLARE_SHAREPTR(SVGLModifyPointSize);
        DECLARE_SHAREPTR(SVGLModifyBlend);
        DECLARE_SHAREPTR(SVGLModifyCullFace);
        DECLARE_SHAREPTR(SVGLModifyDepth);
        DECLARE_SHAREPTR(SVGLModifyStencil);
        DECLARE_SHAREPTR(SVGLModifyShader);
        DECLARE_SHAREPTR(SVGLModifyUnif32);
        DECLARE_SHAREPTR(SVGLModifyUniMatrix);
        
    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTLDECLARE_H
