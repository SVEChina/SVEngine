//
//  ZCOrgMgr.cpp
//

#include "ZCOrgMgr.h"
#include "../../app/SVInst.h"
#include "../../file/SVFileMgr.h"
#include "../../file/SVLoaderBat.h"

using namespace zc;

ZCOrg::ZCOrg(SVInst *_app)
:SVGBase(_app){
    m_name = "aaa";
    m_dsp = "bbb";
    m_islock = true;
}

ZCOrg::~ZCOrg(){
}

//
ZCOrgMgr::ZCOrgMgr(SVInst *_app)
:SVGBase(_app){
}

ZCOrgMgr::~ZCOrgMgr(){
}

//加载组织
void ZCOrgMgr::load(cptr8 _name) {
    SVLoaderBat t_loader(mApp);
    t_loader.loadFromFile(_name,m_tbl);
}
