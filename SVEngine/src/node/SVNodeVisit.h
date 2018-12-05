//
// SVNodeVisit.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_NODEVISIT_H
#define SV_NODEVISIT_H

#include "../base/SVObject.h"
#include "../node/SVNodeDeclare.h"

namespace sv {
    
    namespace node{
        
        class SVVisitorBase : public SVObject {
        public:
            virtual bool visit(SVNodePtr _node);
        };
        
        //获取目标RESID节点
        class SVVisitCollectResID : public SVVisitorBase {
        public:
            SVVisitCollectResID(u32 resid);
            
            virtual bool visit(SVNodePtr _node);
            
        protected:
            typedef SVArray<u32> RESSET;
            RESSET m_resUIDSet;
            u32 m_targetResUID;
        };
        
        //获取系统中spineName为特定值的所有spine节点
        class SVSpineNodeNameVisitor : public SVVisitorBase {
            typedef SVArray<SVNodePtr> SPINENODESET;
        public:
            SVSpineNodeNameVisitor(cptr8 nodeName);
            
            virtual bool visit(SVNodePtr _node);
            
        protected:
            SPINENODESET m_spineNodeSet;
            SVString m_nodeName;
        };
        
        //获取场景的所有子节点
        class SVVisitCollectAllNode : public SVVisitorBase {
        public:
            SVVisitCollectAllNode();
            
            virtual bool visit(SVNodePtr _node);
            
        public:
            typedef SVArray<SVNodePtr> NodeArray;
            NodeArray m_nodearray;
        };
        
        //获取系统中spineName为特定值的所有spine节点
        class SVVisitorNodeName : public SVVisitorBase {
            typedef SVArray<SVNodePtr> NODESET;
        public:
            SVVisitorNodeName(cptr8 nodeName);
            
            virtual bool visit(SVNodePtr _node);
            
        public:
            NODESET m_nodeSet;
            SVString m_nodeName;
        };
        
    }//!namespace node
    
}//!namespace sv



#endif //SV_NODEVISIT_H
