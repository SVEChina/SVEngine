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
        //故事名称
        SVString t_story_name = m_tbl.getCtx(i,"名称");
        t_story->setName(t_story_name.c_str());
        //故事编号
        s32 t_id = m_tbl.getCtxI(i,"编号");
        t_story->setID(t_id);
        //更新章节数
        //s32 t_chapternum = m_tbl.getCtxI(i,"更新章节");
        //t_story->setAuthor(t_author.c_str());
        //封面
        SVString t_picture = m_tbl.getCtx(i,"封面");
        //t_story->setAuthor(t_author.c_str());
        //目录
        SVString t_list = m_tbl.getCtx(i,"目录");
        t_story->setList(t_list.c_str());
        //人物表
        SVString t_roleTbl = m_tbl.getCtx(i,"人物表");
        t_story->setRoleTbl(t_roleTbl.c_str());
        //组织表
//        SVString t_roleTbl = m_tbl.getCtx(i,"组织表");
//        t_story->setRoleTbl(t_roleTbl.c_str());
//        //附属表1
//        SVString t_roleTbl = m_tbl.getCtx(i,"附属表1");
//        t_story->setRoleTbl(t_roleTbl.c_str());
//        //附属表2
//        SVString t_roleTbl = m_tbl.getCtx(i,"附属表2");
//        t_story->setRoleTbl(t_roleTbl.c_str());
//        //附属表3
//        SVString t_roleTbl = m_tbl.getCtx(i,"附属表3");
//        t_story->setRoleTbl(t_roleTbl.c_str());
        m_storyVec.append(t_story);
    }
}

//加载故事
void ZCStoryMgr::loadStory(cptr8 _name) {
    for(s32 i=0;i<m_storyVec.size();i++) {
        SVString t_storyname = m_storyVec[i]->getName();
        if( t_storyname == _name) {
            m_storyVec[i]->load();
        }
    }
}

void ZCStoryMgr::getAllStory(SVArray<ZCStoryPtr> &_storyVec){
    for(s32 i=0;i<m_storyVec.size();i++) {
        _storyVec.append(m_storyVec[i]);
    }
}
