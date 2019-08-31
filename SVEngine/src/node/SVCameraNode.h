//
// SVCameraNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CAMERANODE_H
#define SV_CAMERANODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        //相机节点 不是节点
        
        class SVCameraNode : public SVNode {
        public:
            SVCameraNode(SVInst *_app);
            
            ~SVCameraNode();
            
            //link fbo相关
            void addLinkFboObject(SVFboObjectPtr _fbo);
            
            bool removeLinkFboObject(SVFboObjectPtr _fbo);
            
            //
            void update(f32 _dt);
            
            void resetDefaultCamera();
            
            virtual void resetCamera(f32 w, f32 h);

            void setZ(f32 _near, f32 _far);
            
            void setPosition(FVec3& _pos);
            
            void setTarget(f32 _x, f32 _y, f32 _z);
            
            void setDirection(f32 _x, f32 _y, f32 _z);
            
            void setUp(f32 _x, f32 _y, f32 _z);
            
            void setPose(f32 _x, f32 _y, f32 _z);
            
            void syncViewMatrix(FMat4 &_mat);
            
            void syncProjectMatrix(FMat4 &_mat);
            
            FVec3& getDirection();
            
            FVec3& getUp();
            
            f32 *getProjectMat();
            
            f32 *getCameraMat();
            
            f32 *getVPMat();
            
            FMat4& getProjectMatObj();
            
            FMat4& getViewMatObj();
            
            FMat4& getVPMatObj();
            
            virtual void updateProjMat();
            
            virtual void updateCameraMat();
            
            virtual void updateViewProj();

            //重制
            void reset();
            
            //获取控制器
            SVNodeCtrlCameraPtr getCtrl();
            
            //设置控制器
            void setCtrl(SVNodeCtrlCameraPtr _ctr);
            
        protected:
            void _removeUnuseLinkFboObject();
            
            f32 m_width;    //宽
            f32 m_height;   //高
            f32 m_p_zn;     //近
            f32 m_p_zf;     //远
            //姿态确认
            FVec3 m_att_pos;    //位置
            FVec3 m_att_rot;    //欧拉角
            FVec3 m_att_scale;  //缩放
            //目标，方向，向上 计算姿态
            FVec3 m_pos;
            FVec3 m_targetEx;
            FVec3 m_upEx;
            FVec3 m_direction;
            //各种矩阵
            FMat4 m_mat_proj;
            FMat4 m_mat_view;
            FMat4 m_mat_vp;
            //
            f32 m_angle_yaw;
            f32 m_angle_pitch;
            //
            SVLockPtr m_resLock;
            //
            SVNodeCtrlCameraPtr m_pCtrl;
            //
            typedef SVArray<SVFboObjectPtr> FBOBJECTPOOL;
            FBOBJECTPOOL m_fbobjectPool;
        };
        
        //透视投影相机
        class SVCameraProjNode : public SVCameraNode {
        public:
            SVCameraProjNode(SVInst *_app);
            
            ~SVCameraProjNode();
            
            void setProjectParam(f32 _znear, f32 _zfar, f32 _fovy, f32 _aspect);
            //
            virtual void resetCamera(f32 w, f32 h);
            
            virtual void updateProjMat();
            
        protected:
            f32 m_fovy;
            
        };
        
        //正交相机节点
        class SVCameraOrthoNode : public SVCameraNode {
        public:
            SVCameraOrthoNode(SVInst *_app);
            
            ~SVCameraOrthoNode();
            //
            virtual void resetCamera(f32 w, f32 h);
            //
            virtual void updateProjMat();   //投影矩阵（正交）
        };
        
        //
        
    }//!namespace node
    
}//!namespace sv



#endif //SV_CAMERANODE_H
