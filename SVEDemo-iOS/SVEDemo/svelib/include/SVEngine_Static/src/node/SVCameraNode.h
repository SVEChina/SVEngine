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
            
            void setLockTarget(bool _enable);
            
            FVec3& getDirection();
            
            f32 *getProjectMat();
            
            f32 *getCameraMat();
            
            f32 *getVPMat();
            
            FMat4& getProjectMatObj();
            
            FMat4& getViewMatObj();
            
            FMat4& getVPMatObj();
            
            void updateProjMat();
            
            void updateCameraMat(bool _bUpdateDir = true);
            
            void updateViewProj();
            
            f32 m_width;
            f32 m_height;
            f32 m_fovy;
            
            //投影
            f32 m_p_zn;
            f32 m_p_zf;
            
            void active();
            
            void unactive();
            
        protected:
            bool m_active;
            bool m_lockTarget;
            
            FVec3 m_targetEx;
            FVec3 m_upEx;
            FVec3 m_direction;
            
            FMat4 m_mat_proj;
            FMat4 m_mat_view;
            FMat4 m_mat_vp;
        };

        
    }//!namespace node
    
}//!namespace sv



#endif //SV_CAMERANODE_H
