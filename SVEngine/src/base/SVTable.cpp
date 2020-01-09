//
// SVTable.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVTable.h"

SVTable::SVTable(){
    _hasHead = false;
}

SVTable::~SVTable() {
    //卸载内容
    clearCxt();
    //卸载头
    m_tblHead.clear();
}

void SVTable::reset() {
    //
    _hasHead = false;
    //卸载内容
    clearCxt();
    //卸载头
    m_tblHead.clear();
}

void SVTable::setHead(SVStringArray<>& _head) {
    if(!_hasHead) {
        _hasHead = true;
        for(s32 i=0;i<_head.size();i++) {
            m_tblHead.append(_head[i]);
        }
    }
}

void SVTable::pushCxt(SVStringArray<>& _value) {
    if(!_hasHead)
        return;
    if(_value.size() == m_tblHead.size() ) {
        SVArray<SVString> t_array;
        for(s32 i=0;i<_value.size();i++){
            t_array.append(_value[i]);
        }
        m_tblCtx.append(t_array);
    }
}

void SVTable::removeCxt(s32 _key) {
    if(!_hasHead)
        return;
}

void SVTable::clearCxt() {
    if(!_hasHead)
        return;
//    SVMap<s32,SVArray<SVString>>::Iterator it = m_tblCtx.begin();
//    while(it!=m_tblCtx.end()) {
//        it->data.clear();
//        it++;
//    }
//    m_tblCtx.clear();
}

s32 SVTable::getCtxNum() {
    return m_tblCtx.size();
}

SVString SVTable::getCtx(s32 _index,cptr8 _name) {
    if(_index>=m_tblCtx.size())
        return "nil";
    if(!_hasHead)
        return "nil";
    s32 t_index = -1;
    for(s32 i=0;i<m_tblHead.size();i++) {
        if(m_tblHead[i] == _name) {
            t_index = i;
            break;
        }
    }
    //
    if(t_index!=-1) {
        return m_tblCtx[_index][t_index];
    }
    return "nil";
}

f32 SVTable::getCtxF(s32 _index,cptr8 _name) {
    SVString t_value = getCtx(_index,_name);
    if(t_value!="nil") {
        return SVString::atof(t_value.c_str());
    }
    return 0.0f;
}

s32 SVTable::getCtxI(s32 _index,cptr8 _name) {
    SVString t_value = getCtx(_index,_name);
    if(t_value!="nil") {
        return SVString::atoi(t_value.c_str());
    }
    return 1;
}
