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
        DECLARE_SHAREPTR(SVMtlGLTF);
        DECLARE_SHAREPTR(SVMtlBrightnessContrast);
        DECLARE_SHAREPTR(SVMtlSaturation);
        DECLARE_SHAREPTR(SVMtlVibrance);
        DECLARE_SHAREPTR(SVMtlWhiteBalance);
        DECLARE_SHAREPTR(SVMtlExposure);
        DECLARE_SHAREPTR(SVMtlGamma);
        DECLARE_SHAREPTR(SVMtlShadowHighlight);
        DECLARE_SHAREPTR(SVMtlWhiteBlackLevel);
        DECLARE_SHAREPTR(SVMtlColorBalance);
        DECLARE_SHAREPTR(SVMtlSmooth);
        DECLARE_SHAREPTR(SVMtlHSL);
        DECLARE_SHAREPTR(SVMtlGeo3d);
        DECLARE_SHAREPTR(SVMtlCoord);
        DECLARE_SHAREPTR(SVMtlBeauty);
        DECLARE_SHAREPTR(SVMtlGradientMap);
        DECLARE_SHAREPTR(SVMtlMask);
        DECLARE_SHAREPTR(SVMtlRGB2YUV);
        DECLARE_SHAREPTR(SVMtlFace2D);
        DECLARE_SHAREPTR(SVMtlAni2D);
        DECLARE_SHAREPTR(SVMtl3D);
        DECLARE_SHAREPTR(SVMtl2D);
        DECLARE_SHAREPTR(SVMtlBillboard);
        DECLARE_SHAREPTR(SVMtlPenStrokeGlow);
        DECLARE_SHAREPTR(SVMtlStrokeBase);
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
        DECLARE_SHAREPTR(SVMtlShinning);
        DECLARE_SHAREPTR(SVMtlBlackWhite);
        DECLARE_SHAREPTR(SVMtlStreak);
        DECLARE_SHAREPTR(SVMtlRainbowColor);
        DECLARE_SHAREPTR(SVMtlFlash);
        DECLARE_SHAREPTR(SVMtlGlitch);
        DECLARE_SHAREPTR(SVMtlDistorted);
        DECLARE_SHAREPTR(SVMtlADFilterBase);
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
