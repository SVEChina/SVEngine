//
// SVCameraCtrl.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CAMERA_CTRL_H
#define SV_CAMERA_CTRL_H

#include "../base/SVGBase.h"
#include "../node/SVCameraNode.h"
#include "../base/SVMat4.h"

namespace sv {
    
    namespace logic {
        
        //基础相机控制
        class SVCameraCtrl: public SVGBase {
        public:
            SVCameraCtrl(SVInst* _app);
            
            ~SVCameraCtrl();
            
            virtual bool run(SVCameraNodePtr _nodePtr, f32 dt);
            
            FMat4& getMat();
            
            f32* getMatPoint();
            
            FVec3& getPos();
            
            void bind(SVCameraNodePtr _cam);
            
            void unbind();
            
        protected:
            FMat4 m_mat;
            
            FVec3 m_pos;
            
            SVCameraNodePtr m_linkCam;
        };
        
        //基础相机控制
        class SVCamCtrlBase : public SVCameraCtrl {
        public:
            SVCamCtrlBase(SVInst* _app);
            
            ~SVCamCtrlBase();
            
            virtual bool run(SVCameraNodePtr _nodePtr, f32 dt);
            
            void setPosition(f32 _x, f32 _y, f32 _);
            
            void setTarget(f32 _x, f32 _y, f32 _z);
            
            void setDirection(f32 _x, f32 _y, f32 _z);
            
            void setUp(f32 _x, f32 _y, f32 _z);
            
            FVec3& getUp();
            
            FVec3& getDirection();

        protected:
            //目标，方向，向上 计算姿态
            FVec3 m_targetEx;
            FVec3 m_upEx;
            FVec3 m_direction;
            bool m_dirty;
        };
        
        //姿态相机控制
        class SVCamCtrlAttr : public SVCameraCtrl {
        public:
            SVCamCtrlAttr(SVInst* _app);
            
            ~SVCamCtrlAttr();
            
            virtual bool run(SVCameraNodePtr _nodePtr, f32 dt);
            
        protected:
            //姿态确认
            FVec3 m_att_pos;    //位置
            FVec3 m_att_rot;    //欧拉角
            FVec3 m_att_scale;  //缩放
        };
        
        //3D相机节点控制
        class SVNodeCtrlCamera : public SVCameraCtrl {
        public:
            SVNodeCtrlCamera(SVInst* _app);
            
            ~SVNodeCtrlCamera();
            
            virtual bool run(SVCameraNodePtr _nodePtr, f32 dt);
            
            //
            void reset();
            //
            void reset(s32 _w,s32 _h);
            //
            void resize(s32 _w,s32 _h);
            //平移 像素dert x,y
            void move(f32 _px,f32 py);
            //角度旋转 像素dert x,y
            void angle(f32 _px,f32 py);
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
            
            virtual bool run(SVCameraNodePtr _nodePtr, f32 dt);
            //
            void reset();
            //平移
            void move(f32 _win_px0,f32 _win_py0,f32 _win_px1,f32 _win_py1);
            //推拉
            void zoom(f32 _dis);
            
            void setZoomMult(f32 _mult){ m_zoom_x = _mult; }
            
            f32 getZoomMult(){ return m_zoom_x; }

        protected:
            f32 m_dis;
            FVec3 m_target;
            f32 m_pixelUnit;
            f32 m_zoom_x;
            bool m_dirty;
        };
        
        //AR控制
        class SVCtrlCamereAR : public SVNodeCtrlCamera {
        public:
            SVCtrlCamereAR(SVInst* _app);
            
            ~SVCtrlCamereAR();
        };
        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_CAMERA_CTRL_H
