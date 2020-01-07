//
//  ZCRoleMgr.cpp
//

#include "ZCRoleMgr.h"
#include "../../app/SVInst.h"
#include "../../file/SVFileMgr.h"
#include "../../file/SVLoaderBat.h"

using namespace zc;

ZCRoleMgr::ZCRoleMgr(SVInst *_app)
:SVGBase(_app){
}

ZCRoleMgr::~ZCRoleMgr(){
}

//加载角色表
void ZCRoleMgr::load(cptr8 _name) {
    SVLoaderBat t_loader(mApp);
    t_loader.loadFromFile(_name,m_tbl);
    int a = 0;
}
