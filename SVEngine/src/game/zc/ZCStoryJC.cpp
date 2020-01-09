//
//  ZCStoryJC.cpp
//

#include "ZCStoryJC.h"
#include "ZCOrgMgr.h"
#include "ZCRoleMgr.h"
#include "../../app/SVInst.h"
#include "../../file/SVFileMgr.h"
#include "../../file/SVLoaderBat.h"

using namespace zc;

ZCStoryJC::ZCStoryJC(SVInst *_app)
:ZCStory(_app){
    //组织
    m_pOrgMgr = MakeSharedPtr<ZCOrgMgr>(mApp);
    //角色
    m_pRoleMgr = MakeSharedPtr<ZCRoleMgr>(mApp);
}

ZCStoryJC::~ZCStoryJC(){
    m_pOrgMgr = nullptr;
    m_pRoleMgr = nullptr;
}

void ZCStoryJC::load() {
    ZCStory::load();
}
