//
// SVAniTrigger.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVAniTrigger.h"
#include "../basesys/SVSceneMgr.h"
#include "../node/SVScene.h"
#include "../node/SVNodeVisit.h"
#include "../node/SVSpineNode.h"
SVAniTrigger::SVAniTrigger(SVInst *_app)
:SVGBase(_app) {

}

SVAniTrigger::~SVAniTrigger() {

}

void SVAniTrigger::init() {
}

void SVAniTrigger::destroy() {

}

void SVAniTrigger::enter(){
}

void SVAniTrigger::exit(){
}

void SVAniTrigger::update(f32 _dt) {
    
}

bool SVAniTrigger::isEnd(){
    return true;
}

void SVAniTrigger::noticeTriggerCondition(cptr8 _condition){
    if (strcmp(_condition, m_triggerParam.condition.c_str()) == 0) {
        SVVisitorNodeNamePtr t_visitor = MakeSharedPtr<SVVisitorNodeName>(m_triggerParam.tarName);
        mApp->getSceneMgr()->getScene()->visit(t_visitor);
        if (t_visitor->m_nodeSet.size() > 0) {
            SVNodePtr t_node = t_visitor->m_nodeSet[0];
            if (t_node) {
                if (strcmp(m_triggerParam.action, "appear") == 0) {
                    t_node->setvisible(true);
                    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
                    if (t_spineNode) {
                        t_spineNode->play(t_spineNode->getCurAniName());
                    }
                }
            }
        }
    }
}

void SVAniTrigger::fromJson(RAPIDJSON_NAMESPACE::Value &item){
    if (item.HasMember("tarname") && item["tarname"].IsString()) {
        m_triggerParam.tarName = item["tarname"].GetString();
    }
    if (item.HasMember("action") && item["action"].IsString()) {
        m_triggerParam.action = item["action"].GetString();
    }
    if (item.HasMember("condition") && item["condition"].IsString()) {
        m_triggerParam.condition = item["condition"].GetString();
    }
}
