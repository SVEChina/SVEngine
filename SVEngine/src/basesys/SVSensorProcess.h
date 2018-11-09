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
namespace sv {
    namespace logic{
        
        class SVSensorProcess : public SVProcess {
        public:
            SVSensorProcess(SVInst* _app);
            
            ~SVSensorProcess();
            //
            virtual bool procEvent(SVEventPtr _event);
            
            void update(f32 _dt);
            
            void startSensor();
            
            void disableSensor();
            
            FVec3& getVelocity();
            
            FVec3& getDistance();
        protected:
            void _computePosition();
            timeval m_timTagLast;
            f32 m_deltaTime;
            bool m_isFitst;
            bool m_isEnable;
            FVec3 m_acc0;//加速度
            FVec3 m_acc1;//加速度
            FVec3 m_velocity0;//速度
            FVec3 m_velocity1;//速度
            FVec3 m_distance0;//路程
            FVec3 m_distance1;//路程
        };
        
    }//!namespace logic

}//!namespace sv




#endif //SV_SENSORMODULE_H
