//
//  ZCMgr.cpp
//

#include "ZCMgr.h"
#include "ZCCtrl.h"
#include "ZCRoleMgr.h"
#include "ZCStory.h"
#include "ZCOrgMgr.h"
#include "ZCStoryMgr.h"
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
    m_pStoryMgr = MakeSharedPtr<ZCStoryMgr>(mApp);
    m_pStoryMgr->load("list/story-list.svbat");
    //test
    /*
    m_pStoryMgr->loadStory("我的修融时代");
    m_pStoryMgr->activeStory("我的修融时代");
    ZCStoryPtr t_activeStory = m_pStoryMgr->getActiveStory();
    t_activeStory->loadChapter(0);
    t_activeStory->activeChapter(0);
    ZCChapterPtr t_activeChapter = t_activeStory->getActiveChapter();
    ZCContextPtr t_context = t_activeChapter->getCtx();
     */
    //控制
    m_pCtrl = MakeSharedPtr<ZCCtrl>(mApp);
}

void ZCMgr::destroy() {
//    m_pStory->destroy();
//    m_pCtrl->destroy();
}

void ZCMgr::update(f32 _dt) {
}

void ZCMgr::reset() {
    
}

ZCStoryMgrPtr ZCMgr::getStroyMgr(){
    return m_pStoryMgr;
}
