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
        
        //3D相机节点控制
        class SVNodeCtrlCamera : public SVNodeCtrl {
        public:
            SVNodeCtrlCamera(SVInst* _app);
            
            ~SVNodeCtrlCamera();
            
            virtual void run(SVCameraNodePtr _nodePtr, f32 dt);
            
            //
            void reset();
            //
            void reset(s32 _w,s32 _h);
            //
            void resize(s32 _w,s32 _h);
            //平移 像素dert x,y
            void move(s32 _px,s32 py);
            //角度旋转 像素dert x,y
            void angle(s32 _px,s32 py);
            //推拉
            void zoom(f32 _dis);
            
        protected:
            f32 m_dis;
            s32 m_width;
            s32 m_height;
        };

        //纯平面控制
        class SVCtrlCamera2D : public SVNodeCtrlCamera {
        public:
            SVCtrlCamera2D(SVInst* _app);
            
            ~SVCtrlCamera2D();
            
            virtual void run(SVCameraNodePtr _nodePtr, f32 dt);
            //
            void reset();
            //
            void reset(s32 _w,s32 _h);
            //
            void resize(s32 _w,s32 _h);
            //平移 像素dert x,y
            void move(s32 _px,s32 py);
            //角度旋转 像素dert x,y
            void angle(s32 _px,s32 py);
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
