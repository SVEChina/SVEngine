//
//  ZCMgr.cpp
//

#include "ZCMgr.h"

using namespace zc;

ZCMgr::ZCMgr(SVInst *_app)
:SVModuleBase(_app){
}

ZCMgr::~ZCMgr(){
}

void ZCMgr::init() {
//    //
//    m_pCtrl = MakeSharedPtr<SVLTCtrl>(mApp);
//    //
//    m_pWorld= MakeSharedPtr<SVLTWorld>(mApp);
//    m_pWorld->init();
//    //
//    m_pRole = MakeSharedPtr<SVLTLRole>(mApp);
//    m_pRole->init();
//    //挂在节点
//    m_pCtrl->attachNode(m_pRole->getNode());
//    SVCtrlCamereFollow2DTopPtr t_cam_ctrl = m_pWorld->getCamCtrl();
//    if(t_cam_ctrl) {
//        t_cam_ctrl->bindTarget(m_pRole->getNode());
//        m_pCtrl->setCamCtrl(t_cam_ctrl);
//    }
}

void ZCMgr::destroy() {
}

void ZCMgr::update(f32 _dt) {
}

void ZCMgr::reset() {
    
}
