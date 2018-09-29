#include "SVIDPool.h"

SVIDPool::SVIDPool(){
    m_initialized = 0;
    m_seed = 0;
}

SVIDPool::~SVIDPool() {
    m_idpool.destroy();
}

u32 SVIDPool::applyUID() {
    u32 tmpID = _createID();
    m_idpool.append(tmpID); //表示已经存在的ID
    return tmpID;
}

void SVIDPool::returnUID(u32 uid) {
    for(s32 i=0;i<m_idpool.size();i++){
        if(m_idpool[i] == uid){
            m_idpool.remove(i);
        }
    }
}

u32 SVIDPool::_random8() {
    if(m_initialized == 0) {
        m_seed = time(NULL);
        m_initialized = 1;
    }
    m_seed = m_seed * 1664525 + 1013904223;
    return (s32)((m_seed >> 20) & 0xff);
}

u32 SVIDPool::_createID() {
    s32 t_id;
    do {
        t_id = _random8();
        t_id |= _random8() << 8;
        t_id |= _random8() << 16;
        t_id |= (_random8() & 0x7f) << 24;
    } while(_hasID(t_id));
    return t_id;
}

bool SVIDPool::_hasID(u32 _uid) {
    for(s32 i=0;i<m_idpool.size();i++) {
        if(m_idpool[i] == _uid) {
            return true;
        }
    }
    return false;
}
