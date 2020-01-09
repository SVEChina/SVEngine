//
//  ZCStoryMgr.cpp
//

#include "ZCStoryMgr.h"
#include "ZCStory.h"
#include "ZCStoryJC.h"
#include "../../app/SVInst.h"
#include "../../file/SVFileMgr.h"
#include "../../file/SVLoaderBat.h"

using namespace zc;

//
ZCStoryMgr::ZCStoryMgr(SVInst *_app)
:SVGBase(_app){
}

ZCStoryMgr::~ZCStoryMgr(){
}

void ZCStoryMgr::load(cptr8 _name) {
    SVLoaderBat t_loader(mApp);
    t_loader.loadFromFile(_name,m_tbl);
    //
    s32 t_cnt = m_tbl.getCtxNum();
    for(s32 i=0;i<t_cnt;i++) {
        ZCStoryJCPtr t_story = MakeSharedPtr<ZCStoryJC>(mApp);
        //作者
        SVString t_author = m_tbl.getCtx(i,"作者");
        t_story->setAuthor(t_author.c_str());
//        //故事名称
//        SVString t_author = m_tbl.getCtx(i,"名称");
//        t_story->setAuthor(t_author.c_str());
//        //故事编号
//        SVString t_author = m_tbl.getCtx(i,"编号");
//        t_story->setAuthor(t_author.c_str());
//        //更新章节数
//        SVString t_author = m_tbl.getCtx(i,"封面");
//        t_story->setAuthor(t_author.c_str());
//        //更新章节数
//        SVString t_author = m_tbl.getCtx(i,"更新章节");
//        t_story->setAuthor(t_author.c_str());
        //配表
        SVString t_list = m_tbl.getCtx(i,"目录");
        t_story->setList(t_list.c_str());
        //
        m_storyVec.append(t_story);
    }
}

void ZCStoryMgr::loadStory(cptr8 _name) {
    
}
