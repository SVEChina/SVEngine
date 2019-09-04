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
        
        //正交
        class SVProjMethod : public SVObject {
        public:
            SVProjMethod();
            
            FMat4& getMat();

            f32* getMatPoint();
            
            virtual void reset();
            
            virtual void refresh();
            
            void setWidth(f32 _w);
            
            void setHeight(f32 _h);
            
            void setNear(f32 _near);
            
            void setFar(f32 _far);
            
            f32 getWidth(){ return m_width; }
            
            f32 getHeight(){ return m_height; }
            
            f32 getNear(){ return m_znear; }
            
            f32 getFar(){ return m_zfar; }
            
            
        protected:
            FMat4 m_projMat;
            f32 m_width;
            f32 m_height;
            f32 m_zfar;
            f32 m_znear;
        };
        
        //投影
        class SVProject : public SVProjMethod {
        public:
            SVProject();
            
            void reset();
            
            void refresh();
            
            void setFovy(f32 _fovy);
            
        protected:
            f32 m_fovy;
        };
        
        //正交
        class SVOrtho : public SVProjMethod {
        public:
            SVOrtho();
            
            void reset();
            
            void refresh();
            
        protected:
            
        };
        
        //相机节点 不是节点
        class SVCameraNode : public SVNode {
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

            //
            void setZ(f32 _near, f32 _far);
            
            f32 *getProjectMat();
            
            f32 *getCameraMat();
            
            f32 *getVPMat();
            
            FMat4& getProjectMatObj();
            
            FMat4& getViewMatObj();
            
            FMat4& getVPMatObj();
    
            virtual void updateViewProj();

            //设置控制器
            void setCtrl(SVCameraCtrlPtr _ctr);
            //获取控制器,相机控制,主要是产生视矩阵
            SVCameraCtrlPtr getCtrl();
            //投影方法
            SVProjMethodPtr getProjMethod();
            //
            void setProject();
            //
            void ortho();
            
        protected:
            //马上更新
            void _updateForce();
            
            void _removeUnuseLinkFboObject();

            //各种矩阵
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
        };
        

    }//!namespace node
    
}//!namespace sv



#endif //SV_CAMERANODE_H
