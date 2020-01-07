//
//  ZCMgr.cpp
//

#include "ZCMgr.h"
#include "ZCCtrl.h"
#include "ZCRoleMgr.h"
#include "ZCStory.h"
#include "ZCOrgMgr.h"
#include "../../app/SVInst.h"
#include "../../file/SVFileMgr.h"

using namespace zc;

ZCMgr::ZCMgr(SVInst *_app)
:SVModuleBase(_app){
    m_pOrgMgr = nullptr;
    m_pStory = nullptr;
    m_pRoleMgr = nullptr;
    m_pCtrl = nullptr;
}

ZCMgr::~ZCMgr(){
    m_pOrgMgr = nullptr;
    m_pStory = nullptr;
    m_pRoleMgr = nullptr;
    m_pCtrl = nullptr;
}

void ZCMgr::init() {
    //角色
    m_pRoleMgr = MakeSharedPtr<ZCRoleMgr>(mApp);
    m_pRoleMgr->load("zc-role.csv");
    //组织
    m_pOrgMgr = MakeSharedPtr<ZCOrgMgr>(mApp);
    m_pOrgMgr->load("zc-org.csv");
    //故事
    m_pStory = MakeSharedPtr<ZCStory>(mApp);
    m_pStory->load("zc-story.csv");
    //控制
    m_pCtrl = MakeSharedPtr<ZCCtrl>(mApp);
}

void ZCMgr::destroy() {
//    m_pOrgMgr->destroy();
//    m_pStory->destroy();
//    m_pRoleMgr->destroy();
//    m_pCtrl->destroy();
}

void ZCMgr::update(f32 _dt) {
}

void ZCMgr::reset() {
    
}
