//
//  ZCStoryMgr.cpp
//

#include "ZCStoryMgr.h"
#include "../../app/SVInst.h"
#include "../../file/SVFileMgr.h"
#include "../../file/SVLoaderBat.h"

using namespace zc;

//
ZCStoryMgr::ZCStoryMgr(SVInst *_app)
:SVGBase(_app){
//    //组织
//    m_pOrgMgr = MakeSharedPtr<ZCOrgMgr>(mApp);
//    m_pOrgMgr->load("");
//    //角色
//    m_pRoleMgr = MakeSharedPtr<ZCRoleMgr>(mApp);
//    m_pRoleMgr->load("");
//    //故事
//    m_pStory = MakeSharedPtr<ZCStory>(mApp);
//    m_pStory->load("");
}

ZCStoryMgr::~ZCStoryMgr(){
}

void ZCStoryMgr::load(cptr8 _name) {
    SVLoaderBat t_loader(mApp);
    t_loader.loadFromFile(_name,m_tbl);
}
