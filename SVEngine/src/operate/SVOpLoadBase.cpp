//
// SVOpLoadBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpLoadBase.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPickProcess.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVCameraMgr.h"
#include "../node/SVNode.h"
#include "../node/SVCameraNode.h"

SVOpLoadBase::SVOpLoadBase(SVInst *_app)
: SVOpBase(_app) {
}

SVOpLoadBase::~SVOpLoadBase() {
}

//回收节点有问题
SVOpRecycleNode::SVOpRecycleNode(SVInst *_app, SVNodePtr _node)
:SVOpBase(_app) {
    m_node = _node;
}

void SVOpRecycleNode::_process(f32 dt){
    m_node = nullptr;
}

//拾取包
SVOpPick::SVOpPick(SVInst *_app, s32 _x,s32 _y)
:SVOpBase(_app){
    m_ui_x = _x;
    m_ui_y = _y;
}

//
SVOpPick::~SVOpPick() {
}

//
void SVOpPick::_process(f32 dt){
    SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
    if(t_pickModule){
        f32 t_new_x = m_ui_x;
        f32 t_new_y = m_ui_y;
        //反适配
        mApp->getSceneMgr()->uiToScene(t_new_x,t_new_y);
        SVCameraNodePtr t_camera = mApp->m_pGlobalMgr->m_pCameraMgr->getMainCamera();
        if( t_pickModule->pickScene(t_camera,t_new_x, t_new_y) ){
            SVNodePtr t_pickNode = t_pickModule->getPickNode();
            if( t_pickNode ) {
                t_pickNode->setdrawAABB(true);
            }
        }
    }
}
