//
// SVPreDeclare.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PREDECLARE_H
#define SV_PREDECLARE_H

#include "../base/SVCompileConfig.h"
#include <memory>

//定义shareptr宏
#define DECLARE_SHAREPTR(classname) \
class classname;\
typedef std::shared_ptr<classname> classname##Ptr;\

namespace sv {

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
    DECLARE_SHAREPTR(SVFaceDataMesh);
    
    //基础
    DECLARE_SHAREPTR(SVRandom);
    DECLARE_SHAREPTR(SVLock);
    DECLARE_SHAREPTR(SVSignLock);
    DECLARE_SHAREPTR(SVThread);
    DECLARE_SHAREPTR(SVThreadWork);
    DECLARE_SHAREPTR(SVThreadMain);
    DECLARE_SHAREPTR(SVThreadSync);
    DECLARE_SHAREPTR(SVThreadRender);
    DECLARE_SHAREPTR(SVThreadHelp);
    DECLARE_SHAREPTR(SVDataSwap);
    DECLARE_SHAREPTR(SVModify);

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
    DECLARE_SHAREPTR(SVRenderMgr);
    DECLARE_SHAREPTR(SVShaderMgr);
    DECLARE_SHAREPTR(SVSceneMgr);
    DECLARE_SHAREPTR(SVDeformMgr);
    DECLARE_SHAREPTR(SVCameraMgr);
    DECLARE_SHAREPTR(SVTexMgr);
    DECLARE_SHAREPTR(SVUIMgr);
    DECLARE_SHAREPTR(SVLightSys);
    DECLARE_SHAREPTR(SVPythonSys);
    DECLARE_SHAREPTR(SVModelMgr);
    //模块
    DECLARE_SHAREPTR(SVModuleBase);
    DECLARE_SHAREPTR(SVModuleSys);
    DECLARE_SHAREPTR(SVModuleDelaySuspend);
    //静态类......
    DECLARE_SHAREPTR(SVGeoGen);
    //操作Operate
    DECLARE_SHAREPTR(SVOpBase);
    DECLARE_SHAREPTR(SVOpPushFaceData);
    DECLARE_SHAREPTR(SVOpCreateScene);
    DECLARE_SHAREPTR(SVOpCreateTest);
    DECLARE_SHAREPTR(SVOpCreateDebugFacePt);
    DECLARE_SHAREPTR(SVOpEngineResume);
    DECLARE_SHAREPTR(SVOpEngineSuspend);
    DECLARE_SHAREPTR(SVOpEngineDelaySuspend);
    DECLARE_SHAREPTR(SVOpCreateDivision);
    DECLARE_SHAREPTR(SVOpCreateMark);
    DECLARE_SHAREPTR(SVOpMarkContent);
    DECLARE_SHAREPTR(SVOpMarkAlpha);
    DECLARE_SHAREPTR(SVOpMarkScale);
    DECLARE_SHAREPTR(SVOpMarkRotation);
    DECLARE_SHAREPTR(SVOpMarkAlphaTime);
    DECLARE_SHAREPTR(SVOpMarkAlphaRange);
    DECLARE_SHAREPTR(SVOpMarkAppearTime);
    DECLARE_SHAREPTR(SVOpMarkPosition);
    DECLARE_SHAREPTR(SVOpMarkEnableRandom);
    
    DECLARE_SHAREPTR(SVOpDestroyScene);
    DECLARE_SHAREPTR(SVOpCameraControl);
    DECLARE_SHAREPTR(SVOpCreateSkyDome);
    DECLARE_SHAREPTR(SVOpCreateParticle);
    DECLARE_SHAREPTR(SVOpCreateEffcet);
    DECLARE_SHAREPTR(SVOpCloseDetect);
    DECLARE_SHAREPTR(SVOpSetBeautyFilter);
    DECLARE_SHAREPTR(SVOpUpdateFilterSmooth);
    DECLARE_SHAREPTR(SVOpShapeFaceSmoothFilter);
    DECLARE_SHAREPTR(SVOpLUTFilterCreate);
    DECLARE_SHAREPTR(SVOpUpdateBSplineFilter);
    DECLARE_SHAREPTR(SVOpCloseBeautyFilter);
    //
    DECLARE_SHAREPTR(SVOpLoadBase);
    DECLARE_SHAREPTR(SVOpPick);
    DECLARE_SHAREPTR(SVOpRecycleNode);
    //
    DECLARE_SHAREPTR(SVOpOpenPen);
    DECLARE_SHAREPTR(SVOpClosePen);
    DECLARE_SHAREPTR(SVOpSetPenEffcet);
    DECLARE_SHAREPTR(SVOpSavePenEffcet);
    DECLARE_SHAREPTR(SVOpClearPen);
    DECLARE_SHAREPTR(SVOpPenUndo);
    DECLARE_SHAREPTR(SVOpPenRedo);
    DECLARE_SHAREPTR(SVOpPenUndoIsEnable);
    DECLARE_SHAREPTR(SVOpPenRedoIsEnable);
    DECLARE_SHAREPTR(SVOpPenMode);
    //
    DECLARE_SHAREPTR(SVOpCreateIOSTexIDInstream);
    DECLARE_SHAREPTR(SVOpCreateIOSInstream);
    DECLARE_SHAREPTR(SVOpDestroyIOSInstream);
    DECLARE_SHAREPTR(SVOpCreateIOSOutstream);
    DECLARE_SHAREPTR(SVOpDestroyIOSOutstream);
    DECLARE_SHAREPTR(SVOpOpenIOSOutstream);
    DECLARE_SHAREPTR(SVOpCloseIOSOutstream);
    DECLARE_SHAREPTR(SVOpDestroyTextureOutstream);
    DECLARE_SHAREPTR(SVOpCreateAndriodCamera);
    DECLARE_SHAREPTR(SVOpCreateTextureCamera);
    DECLARE_SHAREPTR(SVOpCreateStreamCamera);
    DECLARE_SHAREPTR(SVOpDestroyStreamCamera);
    DECLARE_SHAREPTR(SVOpParse);
    DECLARE_SHAREPTR(SVOperateDelEft);
    DECLARE_SHAREPTR(SVOpSaveEft);
    DECLARE_SHAREPTR(SVOpFilterEft);
    DECLARE_SHAREPTR(SVOpGenerate);
    DECLARE_SHAREPTR(SVOpTexLoad);
    DECLARE_SHAREPTR(SVOpTexAttachment);
    
    DECLARE_SHAREPTR(SVOpDestroyEffect);
    DECLARE_SHAREPTR(SVOpDestroyDivision);

#if defined(SV_IOS)
    DECLARE_SHAREPTR(SVOpCreateRenderderMTL);
    DECLARE_SHAREPTR(SVOpCreateRenderderGLIOS);
#elif defined(SV_ANDROID)
    DECLARE_SHAREPTR(SVOpCreateRenderderVK);
    DECLARE_SHAREPTR(SVOpCreateRenderderGLAND);
#elif defined(SV_OSX)
   DECLARE_SHAREPTR(SVOpCreateRenderderGLOSX);
#elif defined(SV_WIN)
    DECLARE_SHAREPTR(SVOpCreateRenderderGLWIN);
#endif
    
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
        DECLARE_SHAREPTR(SVBone);
        DECLARE_SHAREPTR(SVSkeleton);
        
        DECLARE_SHAREPTR(SVASKey);
        DECLARE_SHAREPTR(SVChannel);
        DECLARE_SHAREPTR(SVAnimateSkin);
        DECLARE_SHAREPTR(SVSkinMgr);
        DECLARE_SHAREPTR(SVAnimateMorph);
        DECLARE_SHAREPTR(SVSpine);
        DECLARE_SHAREPTR(SVParticlesWorldBase);
        DECLARE_SHAREPTR(SVParticles);
        DECLARE_SHAREPTR(SVImageUsingMove);
        //
        DECLARE_SHAREPTR(SVMesh);
        DECLARE_SHAREPTR(SVModel);
        //
        DECLARE_SHAREPTR(SVPass);
        DECLARE_SHAREPTR(SVParamDeform);
        DECLARE_SHAREPTR(SVDeformScale);
        DECLARE_SHAREPTR(SVDeformImageMove);
        //font
        DECLARE_SHAREPTR(SVFont);
        DECLARE_SHAREPTR(SVBMFont);
        //image
        DECLARE_SHAREPTR(SVImage);
        
        DECLARE_SHAREPTR(SVNoise);
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
        DECLARE_SHAREPTR(SVRecycleProcess);
        DECLARE_SHAREPTR(SVPickProcess);
        DECLARE_SHAREPTR(SVFontProcess);
        DECLARE_SHAREPTR(SVSensorProcess);
        
        //功能部分
        DECLARE_SHAREPTR(SVAni2DBase);
        DECLARE_SHAREPTR(SVAni2DStickersMgr);
        DECLARE_SHAREPTR(SVStreamIn);
        DECLARE_SHAREPTR(SVStreamOut);
        DECLARE_SHAREPTR(SVPictureProcess);
        //parse部分
        DECLARE_SHAREPTR(SVParse);
        DECLARE_SHAREPTR(SVParse2DAni);
        DECLARE_SHAREPTR(SVParseAnimate);
        DECLARE_SHAREPTR(SVParseMain);
        DECLARE_SHAREPTR(SVParseParticle);
        DECLARE_SHAREPTR(SVParseLUTFilter);
        DECLARE_SHAREPTR(SVParseData);
        DECLARE_SHAREPTR(SVParseMask);
        DECLARE_SHAREPTR(SVParseStamp);
        DECLARE_SHAREPTR(SVParseGame);
        
        //filter 部分
        DECLARE_SHAREPTR(SVFilterBase);
        DECLARE_SHAREPTR(SVFilterLUT);
        DECLARE_SHAREPTR(SVFairDataBlur);
        DECLARE_SHAREPTR(SVFaceBeautyBase);
        DECLARE_SHAREPTR(SVGenFBParam);
        DECLARE_SHAREPTR(SVFairLtraLow);
        DECLARE_SHAREPTR(SVGenLUTParam);
        DECLARE_SHAREPTR(SVDataFilter);
        DECLARE_SHAREPTR(SVFilterGenLUT);
        DECLARE_SHAREPTR(SVFilterClarity);
        DECLARE_SHAREPTR(SVFilterHaze);
        DECLARE_SHAREPTR(SVSharpFilter);
        DECLARE_SHAREPTR(SVFilterGof);
        DECLARE_SHAREPTR(SVFilterGlow);
        DECLARE_SHAREPTR(SVFilterBlur);
        DECLARE_SHAREPTR(SVRGBToneCurveFilter);
        DECLARE_SHAREPTR(SVBasedonFilter);
        DECLARE_SHAREPTR(SVAni2DStickersMgr);
        DECLARE_SHAREPTR(SVAni2DBase);
        DECLARE_SHAREPTR(SVAni2DScreen);
        DECLARE_SHAREPTR(SVFilterShinning);
        DECLARE_SHAREPTR(SVFilterDark);
        DECLARE_SHAREPTR(SVFilterShake);
        DECLARE_SHAREPTR(SVFilterFlashWhite);
        DECLARE_SHAREPTR(SVADFilterBase);
        
        //物理部分
        DECLARE_SHAREPTR(SVPhysicsBase);
        DECLARE_SHAREPTR(SVPhysicsWorld);
        DECLARE_SHAREPTR(SVPhysicsBody);
        DECLARE_SHAREPTR(SVPhysicsBodyRigid);
        DECLARE_SHAREPTR(SVPhysicsShape);
        DECLARE_SHAREPTR(SVPhysicsShapeBox);
        DECLARE_SHAREPTR(SVPhysicsShapeSphere);
        DECLARE_SHAREPTR(SVPhysicsJoint);
        DECLARE_SHAREPTR(SVPhysicsCollider);
    }//!namespace logic
    
    DECLARE_SHAREPTR(SVLoaderGLTF);
    

}//!namespace sv

//
template <typename T, typename... Args>
inline std::shared_ptr<T> MakeSharedPtr(Args&&... args){
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <typename T>
inline std::shared_ptr<T> PointerSharedPtr(T* _pointer){
    std::shared_ptr<T> share_obj(_pointer);
    return share_obj;
}

template <typename T>
using ShareFromThis = std::enable_shared_from_this<T> ;


#define DYN_TO_SHAREPTR(classname,param) std::dynamic_pointer_cast<classname>(param);
#define THIS_TO_SHAREPTR(classname) std::dynamic_pointer_cast<classname>(this->shared_from_this()) 

#endif //SV_PREDECLARE_H

