//
// SVNodeCtrlCamera.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_NODECTRLCAMERA_H
#define SV_NODECTRLCAMERA_H

#include "SVNodeCtrl.h"

namespace sv {
    
    namespace logic {
        
        //相机节点控制
        
        class SVNodeCtrlCamera : public SVNodeCtrl {
        public:
            SVNodeCtrlCamera(SVInst* _app);
            
            ~SVNodeCtrlCamera();
            
            virtual void run(SVCameraNodePtr _nodePtr, f32 dt);
            
        protected:
            
        };

        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_NODECTRLCAMERA_H
