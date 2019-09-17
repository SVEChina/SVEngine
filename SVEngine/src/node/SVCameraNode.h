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
        class SVCameraNode : public SVEventProc {
        public:
            SVCameraNode(SVInst *_app);
            
            ~SVCameraNode();
            
            void init();
            
            void destroy();

            //link fbo相关
            void addLinkFboObject(SVFboObjectPtr _fbo);
            
            bool removeLinkFboObject(SVFboObjectPtr _fbo);
            
            //更新
            void update(f32 _dt);
            
            //重制默认矩阵
            void resetDefaultCamera();
            
            //重制
            virtual void resetCamera(f32 w, f32 h);

            void setZ(f32 _near, f32 _far);
            
            f32 *getProjectMat();
            
            f32 *getCameraMat();
            
            f32 *getVPMat();
            
            FVec3& getPosition();
            
            FMat4& getProjectMatObj();
            
            FMat4& getViewMatObj();
            
            FMat4& getVPMatObj();
    
            virtual void updateViewProj();

            //设置控制器
            void setCtrl(SVCameraCtrlPtr _ctr);
            //获取控制器,相机控制,主要是产生视矩阵
            SVCameraCtrlPtr getCtrl();
            //设置投影方法
            void setProjMethod(SVProjMethodPtr _proj);
            //投影方法
            SVProjMethodPtr getProjMethod();
            //
            void project();
            //
            void ortho();
            
        protected:
            //马上更新
            void _updateForce();
            
            void _removeUnuseLinkFboObject();

            //各种矩阵
            FVec3 m_pos;
            FMat4 m_mat_v;
            FMat4 m_mat_p;
            FMat4 m_mat_vp;
            //
            SVLockPtr m_resLock;
            //
            SVCameraCtrlPtr m_pCtrl;
            //
            SVProjMethodPtr m_pProjMethod;
            //
            typedef SVArray<SVFboObjectPtr> FBOBJECTPOOL;
            FBOBJECTPOOL m_fbobjectPool;
            //
            bool m_dirty;
        };
        

    }//!namespace node
    
}//!namespace sv



#endif //SV_CAMERANODE_H
