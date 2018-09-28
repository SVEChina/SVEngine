//
// SVPreDeclare.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PREDECLARE_H
#define SV_PREDECLARE_H

#include <memory>

//定义shareptr宏
#define DECLARE_SHAREPTR(classname) \
class classname;\
typedef std::shared_ptr<classname> classname##Ptr;\

namespace sv {

    DECLARE_SHAREPTR(SVObject);
    DECLARE_SHAREPTR(SVObject);
    
    //MainApp
    DECLARE_SHAREPTR(SVInst);
    DECLARE_SHAREPTR(SVGlobalMgr);
    DECLARE_SHAREPTR(SVGlobalParam);
    
    //模块module
    DECLARE_SHAREPTR(SVThreadPool);
    DECLARE_SHAREPTR(SVFileMgr);
    DECLARE_SHAREPTR(SVConfig);
    DECLARE_SHAREPTR(SVStaticData);
    DECLARE_SHAREPTR(SVMeshData);
    
    //基础
    DECLARE_SHAREPTR(SVRandom);
    DECLARE_SHAREPTR(SVLock);
    DECLARE_SHAREPTR(SVSignLock);
    DECLARE_SHAREPTR(SVThread);
    DECLARE_SHAREPTR(SVThreadWork);
    DECLARE_SHAREPTR(SVThreadMain);
    DECLARE_SHAREPTR(SVThreadRender);
    DECLARE_SHAREPTR(SVThreadHelp);
    DECLARE_SHAREPTR(SVDataSwap);

    //回调
    DECLARE_SHAREPTR(SVCallbackBase);
    DECLARE_SHAREPTR(SVOpCallback);
    DECLARE_SHAREPTR(SVThreadCallback);
    
    //sysbase - event
    DECLARE_SHAREPTR(SVEventProc);
    DECLARE_SHAREPTR(SVSysBase);
    DECLARE_SHAREPTR(SVBasicSys);
    DECLARE_SHAREPTR(SVDetectMgr);
    DECLARE_SHAREPTR(SVEventMgr);
    DECLARE_SHAREPTR(SVLogicMgr);
    DECLARE_SHAREPTR(SVRenderMgr);
    DECLARE_SHAREPTR(SVShaderMgr);
    DECLARE_SHAREPTR(SVSceneMgr);
    DECLARE_SHAREPTR(SVTexMgr);
    DECLARE_SHAREPTR(SVUIMgr);
    //模块
    DECLARE_SHAREPTR(SVModuleBase);
    DECLARE_SHAREPTR(SVModuleSys);
    DECLARE_SHAREPTR(SVModuleDelaySuspend);
    //静态类......
    DECLARE_SHAREPTR(SVGeoGen);
    //操作Operate
    DECLARE_SHAREPTR(SVOpBase);
    DECLARE_SHAREPTR(SVOpCreateScene);
    DECLARE_SHAREPTR(SVOpCreateTest);
    DECLARE_SHAREPTR(SVOpOpenDelaySuspend);
    
    //
    DECLARE_SHAREPTR(SVOpDestroyScene);
    DECLARE_SHAREPTR(SVOpCameraControl);
    DECLARE_SHAREPTR(SVOpCreateSkyDome);
    DECLARE_SHAREPTR(SVOpCreateParticle);
    DECLARE_SHAREPTR(SVOpCreateEffcet);
    DECLARE_SHAREPTR(SVOpLoadRedPacket);
    DECLARE_SHAREPTR(SVOpCreateRedPacket);
    DECLARE_SHAREPTR(SVOpOpenRedPacket);
    DECLARE_SHAREPTR(SVOpRemoveOnePacket);
    DECLARE_SHAREPTR(SVOpCloseDetect);
    DECLARE_SHAREPTR(SVOpSetBeautyFilter);
    DECLARE_SHAREPTR(SVOpUpdateFilterSmooth);
    DECLARE_SHAREPTR(SVOpShapeFaceSmoothFilter);
    DECLARE_SHAREPTR(SVOpUpdateBSplineFilter);
    DECLARE_SHAREPTR(SVOpCloseBeautyFilter);
    //
    DECLARE_SHAREPTR(SVOpLoadBase);
    DECLARE_SHAREPTR(SVOpPick);
    DECLARE_SHAREPTR(SVOpRecycleNode);
    //
    DECLARE_SHAREPTR(SVOpCreateIOSInstream);
    DECLARE_SHAREPTR(SVOpDestroyIOSInstream);
    DECLARE_SHAREPTR(SVOpCreateIOSOutstream);
    DECLARE_SHAREPTR(SVOpDestroyIOSOutstream);
    DECLARE_SHAREPTR(SVOpOpenIOSOutstream);
    DECLARE_SHAREPTR(SVOpCloseIOSOutstream);
    DECLARE_SHAREPTR(SVOpCreateYUVInstream);
    DECLARE_SHAREPTR(SVOpDestroyYUVInstream);
    DECLARE_SHAREPTR(SVOpDestroyTextureOutstream);
    DECLARE_SHAREPTR(SVOpCreateAndriodCamera);
    DECLARE_SHAREPTR(SVOpCreateTextureCamera);
    DECLARE_SHAREPTR(SVOpCreateStreamCamera);
    DECLARE_SHAREPTR(SVOpParse);
    DECLARE_SHAREPTR(SVOperateDelEft);
    DECLARE_SHAREPTR(SVOpSaveEft);
    DECLARE_SHAREPTR(SVOpFilterEft);
    DECLARE_SHAREPTR(SVOpGenerate);
    DECLARE_SHAREPTR(SVOpTexLoad);
    //
    DECLARE_SHAREPTR(SVOpDestroyRedPacket);
    DECLARE_SHAREPTR(SVOpDestroyEffect);
    //
    DECLARE_SHAREPTR(SVOpCreateRenderder);
    DECLARE_SHAREPTR(SVOpDestroyRenderder);
    DECLARE_SHAREPTR(SVOpResizeRenderder);
    DECLARE_SHAREPTR(SVOpSetRenderTarget);
    DECLARE_SHAREPTR(SVOpSetRenderTextureTarget);
    DECLARE_SHAREPTR(SVOpDestroyRenderTarget);
    DECLARE_SHAREPTR(SVOpSetRenderMirror);
    DECLARE_SHAREPTR(SVOpAppMode);
    
    //核心实用库
    namespace util {
        DECLARE_SHAREPTR(SpineMeshData);
        DECLARE_SHAREPTR(SVSpine);
        DECLARE_SHAREPTR(SVParticlesWorldBase);
        DECLARE_SHAREPTR(SVParticles);
        DECLARE_SHAREPTR(SVImageUsingMove);
        //
        DECLARE_SHAREPTR(SVMesh);
        DECLARE_SHAREPTR(SVModel);
        //
        DECLARE_SHAREPTR(SVPass);
    }//!namespace util
    
    //
    namespace detect {
        DECLARE_SHAREPTR(SVDetectBase);
        DECLARE_SHAREPTR(SVDetectST);
        DECLARE_SHAREPTR(SVPerson);
        DECLARE_SHAREPTR(SVPersonModule);
        DECLARE_SHAREPTR(SVPersonTracker);
        DECLARE_SHAREPTR(SVPersonExpressionExt);
    }//!namespace detect
    //
    namespace logic {
        //逻辑系统下的模块
        class SVRecycleModule;
        typedef std::shared_ptr<SVRecycleModule> SVRecycleModulePtr;
        class SVPickModule;
        typedef std::shared_ptr<SVPickModule> SVPickModulePtr;
#ifdef CONFIG_IS_LOAD_FREETYPE
        class SVFontModule;
        typedef std::shared_ptr<SVFontModule> SVFontModulePtr;
#endif
        //功能部分
        class SVAni2DBase;
        typedef std::shared_ptr<SVAni2DBase> SVAni2DBasePtr;
        class SVAni2DStickersMgr;
        typedef std::shared_ptr<SVAni2DStickersMgr> SVAni2DStickersMgrPtr;
        class SVStreamIn;
        typedef std::shared_ptr<SVStreamIn> SVStreamInPtr;
        class SVStreamOut;
        typedef std::shared_ptr<SVStreamOut> SVStreamOutPtr;
        class SVPictureProcess;
        typedef std::shared_ptr<SVPictureProcess> SVPictureProcessPtr;
        //parse部分
        class SVParse;
        typedef std::shared_ptr<SVParse> SVParsePtr;
        class SVParse2DAni;
        typedef std::shared_ptr<SVParse2DAni> SVParse2DAniPtr;
        class SVParseAnimate;
        typedef std::shared_ptr<SVParseAnimate> SVParseAnimatePtr;
        class SVParseMain;
        typedef std::shared_ptr<SVParseMain> SVParseMainPtr;
        class SVParseParticle;
        typedef std::shared_ptr<SVParseParticle> SVParseParticlePtr;
        class SVParseData;
        typedef std::shared_ptr<SVParseData> SVParseDataPtr;
        class SVParseMask;
        typedef std::shared_ptr<SVParseMask> SVParseMaskPtr;
        class SVParseStamp;
        typedef std::shared_ptr<SVParseStamp> SVParseStampPtr;
        class SVParseGame;
        typedef std::shared_ptr<SVParseGame> SVParseGamePtr;
        
        //model data部分
        DECLARE_SHAREPTR(SVDataBase);
        DECLARE_SHAREPTR(SVDataNode);
        DECLARE_SHAREPTR(SVDataNodeSpine);
        DECLARE_SHAREPTR(SVDataNodeBmpFont);
        DECLARE_SHAREPTR(SVDataNodeSprite);
        DECLARE_SHAREPTR(SVDataNodeMask);
        DECLARE_SHAREPTR(SVModelControl);
        DECLARE_SHAREPTR(SVModelSpineControl);
        
        //filter 部分
        DECLARE_SHAREPTR(SVFairData);
        DECLARE_SHAREPTR(SVFairDataLows);
        DECLARE_SHAREPTR(SVFairDataBlur);
        DECLARE_SHAREPTR(SVFairLtraLow);
        DECLARE_SHAREPTR(SVRGBToneCurveFilter);
        DECLARE_SHAREPTR(SVBasedonFilter);
        DECLARE_SHAREPTR(SVAni2DStickersMgr);
        DECLARE_SHAREPTR(SVAni2DBase);
        DECLARE_SHAREPTR(SVAni2DScreen);
    }//!namespace logic
    
    //
    namespace node {
        //visit
        class SVVisitorBase;
        typedef std::shared_ptr<SVVisitorBase> SVVisitorBasePtr;
        class SVVisitCollectResID;
        typedef std::shared_ptr<SVVisitCollectResID> SVVisitCollectResIDPtr;
        class SVSpineNodeNameVisitor;
        typedef std::shared_ptr<SVSpineNodeNameVisitor> SVSpineNodeNameVisitorPtr;
        class SVVisitCollectAllNode;
        typedef std::shared_ptr<SVVisitCollectAllNode> SVVisitCollectAllNodePtr;
        class SVVisitRayPick;
        typedef std::shared_ptr<SVVisitRayPick> SVVisitRayPickPtr;

        //
        class SVTree4;
        typedef std::shared_ptr<SVTree4> SVTree4Ptr;
        //node
        class SVNode;
        typedef std::shared_ptr<SVNode> SVNodePtr;
        class SV2DFaceMaskASNode;
        typedef std::shared_ptr<SV2DFaceMaskASNode> SV2DFaceMaskASNodePtr;
        class SV2DFaceMaskASNoneMouthNode;
        typedef std::shared_ptr<SV2DFaceMaskASNoneMouthNode> SV2DFaceMaskASNoneMouthNodePtr;
        class SV2DFaceMaskASFixLashNode;
        typedef std::shared_ptr<SV2DFaceMaskASFixLashNode> SV2DFaceMaskASFixLashNodePtr;
        class SV2DFaceMaskASNoEyeNode;
        typedef std::shared_ptr<SV2DFaceMaskASNoEyeNode> SV2DFaceMaskASNoEyeNodePtr;
        class SV2DFaceMaskASMouthFaceOutline;
        typedef std::shared_ptr<SV2DFaceMaskASMouthFaceOutline> SV2DFaceMaskASMouthFaceOutlinePtr;
        class SV2DFaceMaskFPNode;
        typedef std::shared_ptr<SV2DFaceMaskFPNode> SV2DFaceMaskFPNodePtr;
        class SV2DFaceMaskNode;
        typedef std::shared_ptr<SV2DFaceMaskNode> SV2DFaceMaskNodePtr;
        class SV2DFaceMaskSTNode;
        typedef std::shared_ptr<SV2DFaceMaskSTNode> SV2DFaceMaskSTNodePtr;
        class SV3DBox;
        typedef std::shared_ptr<SV3DBox> SV3DBoxPtr;
        class SVBitFontNode;
        typedef std::shared_ptr<SVBitFontNode> SVBitFontNodePtr;
        class SVCameraNode;
        typedef std::shared_ptr<SVCameraNode> SVCameraNodePtr;
        class SVCoordGridNode;
        typedef std::shared_ptr<SVCoordGridNode> SVCoordGridNodePtr;
        class SVDummyNode;
        typedef std::shared_ptr<SVDummyNode> SVDummyNodePtr;
        class SVMultPassNode;
        typedef std::shared_ptr<SVMultPassNode> SVMultPassNodePtr;
        class SVFrameOutNode;
        typedef std::shared_ptr<SVFrameOutNode> SVFrameOutNodePtr;
        class SVFreeTypeNode;
        typedef std::shared_ptr<SVFreeTypeNode> SVFreeTypeNodePtr;
        class SVGameWrapNode;
        typedef std::shared_ptr<SVGameWrapNode> SVGameWrapNodePtr;
        #ifdef SV_IOS
        class SVIOSInstreamNode;
        typedef std::shared_ptr<SVIOSInstreamNode> SVIOSInstreamNodePtr;
        class SVIOSOutstreamNode;
        typedef std::shared_ptr<SVIOSOutstreamNode> SVIOSOutstreamNodePtr;
        #endif
        class SVTransTexNode;
        typedef std::shared_ptr<SVTransTexNode> SVTransTexNodePtr;
        class SVYUVInstreamNode;
        typedef std::shared_ptr<SVYUVInstreamNode> SVYUVInstreamNodePtr;
        class SVModelNode;
        typedef std::shared_ptr<SVModelNode> SVModelNodePtr;
        class SVParticleNode;
        typedef std::shared_ptr<SVParticleNode> SVParticleNodePtr;
        class SVScene;
        typedef std::shared_ptr<SVScene> SVScenePtr;
        class SVSkyDomeNode;
        typedef std::shared_ptr<SVSkyDomeNode> SVSkyDomeNodePtr;
        class SVSpineNode;
        typedef std::shared_ptr<SVSpineNode> SVSpineNodePtr;
        class SVSpriteNode;
        typedef std::shared_ptr<SVSpriteNode> SVSpriteNodePtr;
        class SVFacePointNode;
        typedef std::shared_ptr<SVFacePointNode> SVFacePointNodePtr;
        class SVFaceDetectPointNode;
        typedef std::shared_ptr<SVFaceDetectPointNode> SVFaceDetectPointNodePtr;
        class SVFaceShapeNode;
        typedef std::shared_ptr<SVFaceShapeNode> SVFaceShapeNodePtr;
        class SVShapeVariedNode;
        typedef std::shared_ptr<SVShapeVariedNode> SVShapeVariedNodePtr;
        class SVFaceImageUsingMoveNode;
        typedef std::shared_ptr<SVFaceImageUsingMoveNode> SVFaceImageUsingMoveNodePtr;
        
    }//!namespace node
    
}//!namespace sv

//
template <typename T, typename... Args>
inline std::shared_ptr<T> MakeSharedPtr(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
inline std::shared_ptr<T> PointerSharedPtr(T* _pointer)
{
    std::shared_ptr<T> share_obj(_pointer);
    return share_obj;
}

template <typename T>
using ShareFromThis = std::enable_shared_from_this<T> ;


#define DYN_TO_SHAREPTR(classname,param) std::dynamic_pointer_cast<classname>(param);
#define THIS_TO_SHAREPTR(classname) std::dynamic_pointer_cast<classname>(this->shared_from_this()) 

/*
template<typename T , typename T1>
inline std::dynamic_pointer_cast<T> DynamicCastPtr(T1 t1)
{
    return std::dynamic_pointer_cast<T>(t1);
}
 */

#endif //SV_PREDECLARE_H

