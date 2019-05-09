//
// SVEffectMusic.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVEffectMusic.h"
#include "../base/SVLock.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../app/SVInst.h"
SVEffectMusic::SVEffectMusic(SVInst* _app)
:SVModuleBase(_app){
    m_lock = MakeSharedPtr<SVLock>();
}

SVEffectMusic::~SVEffectMusic(){
    if (m_lock) {
        m_lock = nullptr;
    }
    _clearAllMusic();
}

void SVEffectMusic::init(){
    SVModuleBase::init();
}

void SVEffectMusic::destroy(){
    
    
}

void SVEffectMusic::open(){
    SVModuleBase::open();
    
}

void SVEffectMusic::reset(){

}

void SVEffectMusic::update(f32 _dt) {
    SVModuleBase::update(_dt);
    
}

void SVEffectMusic::setRootPath(cptr8 _path){
    m_rootPath = _path;
}

void SVEffectMusic::_clearAllMusic(){
    MUSICMAP::Iterator it = m_musicMap.begin();
    while(it != m_musicMap.end()){
        SVEffectMusicEventPtr t_event = MakeSharedPtr<SVEffectMusicEvent>();
        t_event->eventType = EVN_T_EFFECT_MUSIC_UNLOAD;
        t_event->path = it->data;
        mApp->getEventMgr()->pushEvent(t_event);
        it++;
    }
    m_musicMap.clear();
}

void SVEffectMusic::_addMusic(cptr8 _key, cptr8 _musicPath){
    m_lock->lock();
    m_musicMap.append(_key,_musicPath);
    SVEffectMusicEventPtr t_event = MakeSharedPtr<SVEffectMusicEvent>();
    t_event->eventType = EVN_T_EFFECT_MUSIC_LOAD;
    t_event->path = _musicPath;
    mApp->getEventMgr()->pushEvent(t_event);
    if (strcmp(_key, "background") == 0) {
        SVEffectMusicEventPtr t_event1 = MakeSharedPtr<SVEffectMusicEvent>();
        t_event1->eventType = EVN_T_EFFECT_MUSIC_PLAY;
        t_event1->path = _musicPath;
        mApp->getEventMgr()->pushEvent(t_event1);
    }
    m_lock->unlock();
}

bool SVEffectMusic::procEvent(SVEventPtr _event) {
    return  true;
}

void SVEffectMusic::fromJSON(RAPIDJSON_NAMESPACE::Value &_item){
    for(auto iter = _item.MemberBegin(); iter != _item.MemberEnd(); ++iter){
        cptr8 key = (iter->name).GetString();
        if (_item.HasMember(key)) {
            RAPIDJSON_NAMESPACE::Value &value = iter->value;
            SVString musicPath = value.GetString();
            if(m_rootPath.size() > 0){
                musicPath = m_rootPath + musicPath;
            }
            _addMusic(key, musicPath);
        }
    }
}


