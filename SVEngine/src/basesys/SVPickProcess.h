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
            bool pickScene(s32 _sx,s32 _sy);
            //UI拾取
            bool pickUI(s32 _sx,s32 _sy);
            //移动节点
            void moveNode(s32 _sx,s32 _sy);
            //获取交叉点
            bool getCrossPoint(s32 _sx,s32 _sy,FVec3& _crosspt);
            //
            SVNodePtr getPickNode();
            //
            virtual bool procEvent(SVEventPtr _event);
            
        protected:
            //
            void _pick(SVNodePtr _node);
            //获取射线
            bool _getRay(s32 _sx,s32 _sy,FVec3& _rayStart,FVec3& _rayEnd);
            //
            bool m_enablePick;
            SVNodePtr m_curPickNode;
        };
        
    }//!namespace logic

}//!namespace sv




#endif //SV_PICKMODULE_H
