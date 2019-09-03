//
// SVNodeDeclare.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_NODEDECLARE_H
#define SV_NODEDECLARE_H

#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace logic {
        DECLARE_SHAREPTR(SVNodeCtrl);
        DECLARE_SHAREPTR(SVCameraCtrl);
        DECLARE_SHAREPTR(SVCamCtrlBase);
        DECLARE_SHAREPTR(SVCamCtrlAttr);
        DECLARE_SHAREPTR(SVNodeCtrlCamera);
        DECLARE_SHAREPTR(SVCtrlCamera2D);
    }
    
    namespace node {
        
        //visit
        DECLARE_SHAREPTR(SVVisitorBase);
        DECLARE_SHAREPTR(SVVisitCollectResID);
        DECLARE_SHAREPTR(SVSpineNodeNameVisitor);
        DECLARE_SHAREPTR(SVVisitCollectAllNode);
        DECLARE_SHAREPTR(SVVisitRayPick);
        DECLARE_SHAREPTR(SVVisitorNodeName);
        
        //
        DECLARE_SHAREPTR(SVTree4);
        DECLARE_SHAREPTR(SVScene);
        //node
        DECLARE_SHAREPTR(SVNode);
        DECLARE_SHAREPTR(SV2DFaceMaskNode);
        DECLARE_SHAREPTR(SV2DFaceMaskSTNode);
        DECLARE_SHAREPTR(SV3DBox);
        DECLARE_SHAREPTR(SVBitFontNode);
        DECLARE_SHAREPTR(SVBMFontNode);
        
        DECLARE_SHAREPTR(SVProjMethod);
        DECLARE_SHAREPTR(SVProject);
        DECLARE_SHAREPTR(SVOrtho);
        DECLARE_SHAREPTR(SVCameraNode);
        DECLARE_SHAREPTR(SVCoordGridNode);
        DECLARE_SHAREPTR(SVDummyNode);
        DECLARE_SHAREPTR(SVMultPassNode);
        DECLARE_SHAREPTR(SVFrameOutNode);
        DECLARE_SHAREPTR(SVFreeTypeNode);
        DECLARE_SHAREPTR(SVGameWrapNode);
        DECLARE_SHAREPTR(SVIOSInstreamNode);
#ifdef SV_IOS
        DECLARE_SHAREPTR(SVFrameOutIOS);
#endif
        DECLARE_SHAREPTR(SVFrameOutTex);
        DECLARE_SHAREPTR(SVFrameOutRead);
        DECLARE_SHAREPTR(SVSkinNode);
        DECLARE_SHAREPTR(SVSkyDomeNode);
        DECLARE_SHAREPTR(SVSpineNode);
        DECLARE_SHAREPTR(SVSpriteNode);
        DECLARE_SHAREPTR(SVBillboardNode);
        DECLARE_SHAREPTR(SVBackGroundNode);
        DECLARE_SHAREPTR(SVFacePointNode);
        DECLARE_SHAREPTR(SVFaceDetectPointNode);
        DECLARE_SHAREPTR(SVFaceShapeNode);
        DECLARE_SHAREPTR(SVDeformNode);
        DECLARE_SHAREPTR(SVSkinNode);
        DECLARE_SHAREPTR(SVModelNode);
        DECLARE_SHAREPTR(SVMorphNode);
        DECLARE_SHAREPTR(SVShapeVariedNode);
        DECLARE_SHAREPTR(SVParticlesWorld);
        DECLARE_SHAREPTR(SVParticlesNode);
        
    }//!namespace node
    
}//!namespace sv





#endif //SV_NODEDECLARE_H
