//
// SVNodeVisit.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVNodeVisit.h"
#include "SVNode.h"
#include "SVSpineNode.h"
#include "../core/SVSpine.h"
#include "../base/SVBounds.h"

bool SVVisitorBase::visit(SVNodePtr _node) {
    return false;
}

//
SVVisitCollectResID::SVVisitCollectResID(u32 _resid)
:m_targetResUID(_resid) {
}

bool SVVisitCollectResID::visit(SVNodePtr _node) {
//    if (_node && _node->getresUID() == m_targetResUID) {
//        m_resUIDSet.append(m_targetResUID);
//    }
    return false;
}

SVSpineNodeNameVisitor::SVSpineNodeNameVisitor(cptr8 _nodeName)
:m_nodeName(_nodeName) {
}

bool SVSpineNodeNameVisitor::visit(SVNodePtr _node) {
    SVSpineNode *t_spianNode = dynamic_cast<SVSpineNode *>(_node.get());
    if (t_spianNode && t_spianNode->getSpine()->m_spineName == m_nodeName) {
        m_spineNodeSet.append(_node);
    }
    return true;
}

//
SVVisitCollectAllNode::SVVisitCollectAllNode() {
}

bool SVVisitCollectAllNode::visit(SVNodePtr _node) {
    m_nodearray.append(_node);
    return true;
}


SVVisitorNodeName::SVVisitorNodeName(cptr8 _nodeName)
:m_nodeName(_nodeName) {
}

bool SVVisitorNodeName::visit(SVNodePtr _node) {
    if (strcmp(_node->getname(), m_nodeName.c_str())  == 0) {
        m_nodeSet.append(_node);
    }
    return true;
}
