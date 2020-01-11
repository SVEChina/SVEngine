//
//  ZCStory.cpp
//

#include "ZCStory.h"
#include "ZCRoleMgr.h"
#include "../../app/SVInst.h"
#include "../../file/SVFileMgr.h"
#include "../../file/SVLoaderBat.h"

using namespace zc;

//内容
ZCContext::ZCContext() {
    
}

ZCContext::~ZCContext() {
    
}

//内容选择
ZCContextSelect::ZCContextSelect() {
    
}

ZCContextSelect::~ZCContextSelect() {
    
}

//章节
ZCChapter::ZCChapter(SVInst *_app)
:SVGBase(_app) {
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
    if (m_curCtxCode >= 0 && m_curCtxCode < m_ctxs.size()) {
        return m_ctxs[m_curCtxCode];
    }
    return nullptr;
}
 
ZCContextPtr ZCChapter::nxt() {
    m_curCtxCode++;
    return getCtx();
}

//加载章节
void ZCChapter::load() {
    SVLoaderBat t_loader(mApp);
    //加载目录
    SVTable m_tbl;
    if( t_loader.loadFromFile(m_ctxpath.c_str(),m_tbl) ) {
        //加载内容
        ZCContextPtr t_cxt_ptr;
        s32 t_ctx_num = m_tbl.getCtxNum();
        for(s32 i=0;i<t_ctx_num;i++) {
             //编号,角色,内容类型,内容0,内容1,音乐,背景,特效
            s32 t_ctx_type = m_tbl.getCtxI(i,"内容类型");
            if(t_ctx_type == 1) {
                t_cxt_ptr = MakeSharedPtr<ZCContextSelect>();
            }else{
                t_cxt_ptr = MakeSharedPtr<ZCContext>();
            }
            t_cxt_ptr->m_id = m_tbl.getCtxI(i,"编号");
            t_cxt_ptr->m_player = m_tbl.getCtx(i,"角色");
            t_cxt_ptr->m_context = m_tbl.getCtx(i,"内容0");
            if(t_ctx_type == 1) {
                ZCContextSelectPtr t_ctx_type1 = DYN_TO_SHAREPTR(ZCContextSelect, t_cxt_ptr);
                t_ctx_type1->m_context1 = m_tbl.getCtx(i,"内容1");
            }
            //公共组建
            t_cxt_ptr->m_bg = m_tbl.getCtxI(i,"背景");
            t_cxt_ptr->m_music = m_tbl.getCtxI(i,"音乐");
            t_cxt_ptr->m_effect = m_tbl.getCtxI(i,"特效");
            //
            m_ctxs.append(t_cxt_ptr);
            //
        }
    }
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
    //
    m_curChapter = nullptr;
}

ZCStory::~ZCStory(){
    m_pRoleMgr = nullptr;
    m_curChapter = nullptr;
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

//加载指定章节
void ZCStory::loadChapter(s32 _index) {
    if(_index<m_chapters.size()) {
        m_chapters[_index]->load();
    }
}

bool ZCStory::activeChapter(s32 _index) {
    if(_index<m_chapters.size()) {
        m_curChapter = m_chapters[_index];
        return true;
    }
    return false;
}

s32 ZCStory::getChapterCount(){
    return m_chapters.size();
}

ZCChapterPtr ZCStory::getActiveChapter(){
    return m_curChapter;
}

ZCChapterPtr ZCStory::getChapter(s32 _index){
    if (_index >=0 && _index < m_chapters.size()) {
        return m_chapters[_index];
    }
    return nullptr;
}

//跳转到目标章节的，目标行数
void ZCStory::jump(s32 _index,s32 _code) {
    if(_index>=m_chapters.size())
        return;
    //
    activeChapter(_index);
    //
}

//执行下一句
void ZCStory::nxt() {
    if(m_curChapter) {
        m_curChapter->nxt();
    }
}


