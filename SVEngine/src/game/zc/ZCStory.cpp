//
//  ZCStory.cpp
//

#include "ZCStory.h"

using namespace zc;

ZCChapter::ZCChapter(SVInst *_app):SVGBase(_app) {
    m_lock = true;
    m_name = "";
    m_curCtxCode = 0;
    m_percent = 0.0f;
}

ZCChapter::~ZCChapter() {
    
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

cptr8 ZCChapter::getName() {
    return m_name.c_str();
}


/*
    故事
 */

ZCStory::ZCStory(SVInst *_app)
:SVGBase(_app){
}

ZCStory::~ZCStory(){
}

//加载
void ZCStory::load(cptr8 _name) {
    
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
