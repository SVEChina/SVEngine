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
        
        void setMainCamera(SVCameraNodePtr _camera);
        
        SVCameraNodePtr getMainCamera();
        
        void addCamera(cptr8 _name, SVCameraNodePtr _camera);
        
        bool removeCamera(cptr8 _name);
        
        SVCameraNodePtr getCamera(cptr8 _name);
        
        bool hasCamera(cptr8 _name);
    protected:
        SVLockPtr m_cameraLock;
        SVCameraNodePtr m_mainCamera;
        typedef SVMap<SVString, SVCameraNodePtr> CAMERAPOOL;
        CAMERAPOOL m_camerPool;
    };
    
}//!namespace




#endif //SV_CAMERAMGR_H
