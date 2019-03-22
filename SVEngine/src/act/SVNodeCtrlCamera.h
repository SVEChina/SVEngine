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
            
            //平移
            void move(FVec3 _off);
            
            //角度旋转
            void angle(f32 _yaw,f32 _pitch);
            
            //推拉
            void zoom(f32 _dis);
            
//            //推进，推远
//            void ctrlZoom(f32 _dis);
//
//            void ctrlAngle(f32 _yaw,f32 _pitch);
//
//            //前进 后退
//            void ctrlForward(f32 _dis);
//
//            //平移左右
//            void ctrlMoveLR(f32 _dis);
//
//            //平移前后
//            void ctrlMoveFB(f32 _dis);
            
        protected:
            f32 m_dis;
            
        };

        class SVCtrlCamera2D : public SVNodeCtrl {
        public:
            SVCtrlCamera2D(SVInst* _app,s32 _width,s32 _height);
            
            ~SVCtrlCamera2D();
            
            virtual void run(SVCameraNodePtr _nodePtr, f32 dt);

            //
            void reset();
            
            //
            void reset(s32 _w,s32 _h);
            
            //平移
            void move(FVec3 _off);
            
            //角度旋转
            void angle(f32 _angle);
            
            //推拉
            void zoom(f32 _dis);

        protected:
            f32 m_dis;
            f32 m_angle;
            s32 m_width;
            s32 m_height;
            FVec3 m_pos;
            FVec3 m_target;
            bool m_dirty;
        };
        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_NODECTRLCAMERA_H
