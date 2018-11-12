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
        
        class SVCameraNode : public SVNode {
        public:
            SVCameraNode(SVInst *_app);
            
            ~SVCameraNode();
            
            void update(f32 _dt);
            
            void resetDefaultCamera();
            
            void resetCamera(f32 w, f32 h, f32 fovy = 120.0f);
            
            void setProjectParam(f32 _znear, f32 _zfar, f32 _fovy, f32 _aspect);
            
            void setZ(f32 _near, f32 _far);
            
            void setPosition(FVec3& _pos);
            
            void setTarget(f32 _x, f32 _y, f32 _z);
            
            void setDirection(f32 _x, f32 _y, f32 _z);
            
            void setUp(f32 _x, f32 _y, f32 _z);
            
            FVec3& getDirection();
            
            f32 *getProjectMat();
            
            f32 *getCameraMat();
            
            f32 *getVPMat();
            
            FMat4& getProjectMatObj();
            
            FMat4& getViewMatObj();
            
            FMat4& getVPMatObj();
            
            void updateProjMat();
            
            void updateCameraMat();
            
            void updateViewProj();
            
            void addLinkFboObject(SVFboObjectPtr _fbo);
            
            bool removeLinkFboObject(SVFboObjectPtr _fbo);
            
            f32 m_width;
            f32 m_height;
            f32 m_fovy;
            
            //投影
            f32 m_p_zn;
            f32 m_p_zf;
            
            void active();
            
            void unactive();
        
        public:
            //推进，推远
            void ctrlZoom(f32 _dis);
            
            //航向xoz
            void ctrlYaw(f32 _angle);
            
            //俯仰y
            void ctrlPitch(f32 _angle);
            
            //前进 后退
            void ctrlForward(f32 _dis);
            
            //平移左右
            void ctrlMoveLR(f32 _dis);
            
            //平移前后
            void ctrlMoveFB(f32 _dis);
            
            //重制
            void reset();
            
        protected:
            void _removeUnuseLinkFboObject();

            SVLockPtr m_resLock;
            bool m_active;
            
            FVec3 m_targetEx;
            FVec3 m_upEx;
            FVec3 m_direction;
            
            FMat4 m_mat_proj;
            FMat4 m_mat_view;
            FMat4 m_mat_vp;
            
            typedef SVArray<SVFboObjectPtr> FBOBJECTPOOL;
            FBOBJECTPOOL m_fbobjectPool;
        };

        
    }//!namespace node
    
}//!namespace sv



#endif //SV_CAMERANODE_H
