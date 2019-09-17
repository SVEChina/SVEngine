//
// SVCameraMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CAMERAMGR_H
#define SV_CAMERAMGR_H

#include "../basesys/SVSysBase.h"
#include "../base/SVMap.h"
namespace sv {

    class SVCameraMgr : public SVSysBase {
    public:
        SVCameraMgr(SVInst *_app);
        
        ~SVCameraMgr();
        
        void init();
        
        void destroy();

        void update(f32 dt);
        
        void resize(f32 _w,f32 _h);
        
        SVCameraNodePtr getMainCamera();
        
        SVCameraNodePtr getUICamera();
        
    protected:
        SVLockPtr m_cameraLock;
        //主相机
        SVCameraNodePtr m_mainCamera;
        //ui相机
        SVCameraNodePtr m_uiCamera;
    };
    
}//!namespace




#endif //SV_CAMERAMGR_H
