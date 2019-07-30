//
// SVSensorProcess.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SENSORMODULE_H
#define SV_SENSORMODULE_H
#include "SVProcess.h"
#include "../base/SVVec3.h"
#include "../node/SVNodeDeclare.h"
namespace sv {
    namespace logic{
        
        class SVSensorProcess : public SVProcess {
        public:
            SVSensorProcess(SVInst* _app);
            
            ~SVSensorProcess();
            //
            virtual bool procEvent(SVEventPtr _event);
            
            void update(f32 _dt);
            
            SVCameraNodePtr getARCamera(){ return m_pARCamera; }
            
        protected:
            SVCameraNodePtr m_pARCamera;
        };
        
    }//!namespace logic

}//!namespace sv




#endif //SV_SENSORMODULE_H
