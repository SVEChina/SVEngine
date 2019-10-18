//
// SVPickProcess.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PICKMODULE_H
#define SV_PICKMODULE_H

#include "SVProcess.h"
#include "../base/SVVec3.h"
#include "../node/SVNodeDeclare.h"
#include "../node/SVNodeVisit.h"
#include "../base/SVMat4.h"

//获取射线拾取(拣选器)

typedef void (*pick_callback)(void* _obj,void* _node,int _flag);

namespace sv {
    
    namespace node {
        
        class SVVisitRayPick : public SVVisitorBase {
        public:
            SVVisitRayPick(FVec3& _rayStart,FVec3& _rayEnd);
            
            ~SVVisitRayPick();
            
            virtual bool visit(SVNodePtr _node);
            
            SVNodePtr getCrossNode(FVec3& _campos);
            
        protected:
            FVec3 m_rayStart;
            FVec3 m_rayEnd;
            typedef SVArray<SVNodePtr> NodeArray;
            NodeArray m_nodearray;
        };
    }
    
    namespace logic{
        
        class SVPickProcess : public SVProcess {
        public:
            SVPickProcess(SVInst* _app);
            
            ~SVPickProcess();
            //
            void enablePick();
            //
            void disablePick();
            //
            void clear();
            //场景拾取
            bool pickScene(SVCameraNodePtr _cam,s32 _sx,s32 _sy);
            //UI拾取
            bool pickUI(s32 _sx,s32 _sy);
            //移动节点
            void moveNode(SVCameraNodePtr _cam,s32 _sx,s32 _sy);
            //获取交叉点
            bool getCrossPoint(SVCameraNodePtr _cam,s32 _sx,s32 _sy,FVec3& _crosspt);
            //
            bool getCrossPointUI(SVCameraNodePtr _cam,s32 _sx,s32 _sy,FVec3& _crosspt);
            //
            bool getCrossPointWithPlane(SVCameraNodePtr _cam,s32 _sx,s32 _sy,FVec3& _crosspt, FVec4& _plane);
            //
            SVNodePtr getPickNode();
            //
            virtual bool procEvent(SVEventPtr _event);
            
            void transScreenPtToWorld(FVec2 &_screenPt, FVec3 &_worldPt);
            
        protected:
            //
            void _pick(SVNodePtr _node);
            //获取射线
            bool _getRay(SVCameraNodePtr _cam,s32 _sx,s32 _sy,FVec3& _rayStart,FVec3& _rayEnd);
            bool _getRayMat(SVCameraNodePtr _cam,FMat4 _vpMat,s32 _sx,s32 _sy,FVec3& _rayStart,FVec3& _rayEnd);
            //
            bool m_enablePick;
            SVNodePtr m_curPickNode;
        };
        
    }//!namespace logic

}//!namespace sv




#endif //SV_PICKMODULE_H
