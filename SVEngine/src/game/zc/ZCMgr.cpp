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
    m_pStoryMgr = nullptr;
    m_pCtrl = nullptr;
}

ZCMgr::~ZCMgr(){
    m_pStoryMgr = nullptr;
    m_pCtrl = nullptr;
}

void ZCMgr::init() {
    m_pStoryMgr = MakeSharedPtr<ZCStory>(mApp);
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
