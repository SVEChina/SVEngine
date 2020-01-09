//
//  ZCStory.cpp
//

#include "ZCStory.h"
#include "ZCRoleMgr.h"
#include "../../app/SVInst.h"
#include "../../file/SVFileMgr.h"
#include "../../file/SVLoaderBat.h"

using namespace zc;

ZCChapter::ZCChapter(SVInst *_app):SVGBase(_app) {
    m_lock = true;
    m_name = "sve";
    m_curCtxCode = 0;
    m_percent = 0.0f;
    m_id = 0;
    m_ctxpath = "sve";
}

ZCChapter::~ZCChapter() {
    
}

void ZCChapter::setID(s32 _id) {
    m_id = _id;
}

void ZCChapter::setName(cptr8 _name) {
    m_name = _name;
}

cptr8 ZCChapter::getName() {
    return m_name.c_str();
}

void ZCChapter::setRes(cptr8 _res) {
    m_ctxpath = _res;
}

//解锁
void ZCChapter::unlock() {
    m_lock = false;
}
 
//是否锁定
bool ZCChapter::isLock() {
    return m_lock;
}

ZCContextPtr ZCChapter::getCtx() {
    return nullptr;
}
 
ZCContextPtr ZCChapter::nxt() {
    return nullptr;
}

//加载章节
void ZCChapter::load() {
    
}


/*
    故事
 */

ZCStory::ZCStory(SVInst *_app)
:SVGBase(_app){
    m_author = "";
    m_list = "";
    m_roletbl = "";
    //故事id
    m_id = 0;
    //故事名称
    m_name = "sve";
    //角色
    m_pRoleMgr = MakeSharedPtr<ZCRoleMgr>(mApp);
}

ZCStory::~ZCStory(){
    m_pRoleMgr = nullptr;
}

//
void ZCStory::setName(cptr8 _name) {
    m_name = _name;
}

cptr8 ZCStory::getName() {
    return m_name.c_str();
}

void ZCStory::setID(s32 _id) {
    m_id = _id;
}

s32 ZCStory::getID() {
    return m_id;
}

void ZCStory::setAuthor(cptr8 _name) {
    m_author = _name;
}

cptr8 ZCStory::getAuthor() {
    return m_author.c_str();
}

void ZCStory::setList(cptr8 _name) {
    m_list = _name;
}

void ZCStory::setRoleTbl(cptr8 _roletbl) {
    m_roletbl = _roletbl;
}

//加载
void ZCStory::load() {
    SVLoaderBat t_loader(mApp);
    //加载目录
    if( t_loader.loadFromFile(m_list.c_str(),m_tbl) ) {
        //加载完毕之后，构建章节
        s32 t_ctx_num = m_tbl.getCtxNum();
        for(s32 i=0;i<t_ctx_num;i++) {
            ZCChapterPtr t_chapter = MakeSharedPtr<ZCChapter>(mApp);
            //构建章节
            s32 t_id = m_tbl.getCtxI(i,"编号");
            t_chapter->setID(t_id);
            //故事编号
            SVString t_name = m_tbl.getCtx(i,"名称");
            t_chapter->setName(t_name.c_str());
            //资源
            SVString t_res = m_tbl.getCtx(i,"资源");
            t_chapter->setRes(t_res.c_str());
            //
            m_chapters.append(t_chapter);
        }
        //m_chapters
    }
    //加载角色表
    m_pRoleMgr->load(m_roletbl.c_str());
}

//跳转到目标章节的，目标行数
void ZCStory::jump(s32 _chapter,s32 _code) {
    
}

//执行下一句
void ZCStory::nxt() {
    if(m_curChapter) {
        m_curChapter->nxt();
    }
}


