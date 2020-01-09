//
//  ZCRoleMgr.cpp
//

#include "ZCRoleMgr.h"
#include "ZCRole.h"
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
    if(  t_loader.loadFromFile(_name,m_tbl) ) {
        //构建角色
        s32 t_ctx_num = m_tbl.getCtxNum();
        for(s32 i=0;i<t_ctx_num;i++) {
            ZCRolePtr t_role = MakeSharedPtr<ZCRole>(mApp);
//            //构建章节
//            s32 t_id = m_tbl.getCtxI(i,"编号");
//            t_chapter->setID(t_id);
//            //故事编号
//            SVString t_name = m_tbl.getCtx(i,"名称");
//            t_chapter->setName(t_name.c_str());
//            //资源
//            SVString t_res = m_tbl.getCtx(i,"资源");
//            t_chapter->setRes(t_res.c_str());
            //
            m_roleVec.append(t_role);
        }
        //m_chapters
    }
}
