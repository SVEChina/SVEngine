//
// SVTable.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVTable.h"

SVTableCell::SVTableCell() {
    m_str_value = "";
    m_f_value = 0.0f;
    m_s_value = 0;
};

SVTableCell& SVTableCell::operator=(const SVTableCell &s) {
    m_str_value = s.m_str_value;
    m_f_value = s.m_f_value;
    m_s_value = s.m_s_value;
    return *this;
}

void SVTableCell::setValue(cptr8 _value) {
    m_str_value = _value;
}

void SVTableCell::setValue(f32 _value) {
    m_f_value = _value;
}

void SVTableCell::setValue(s32 _value) {
    m_s_value = _value;
}

//
SVTableLine::SVTableLine() {
}

//SVTableLine::SVTableLine(s32 _num) {
//    m_cells.allocate(_num);
//}

SVTableLine::~SVTableLine() {
    m_cells.destroy();
}

SVTableLine &SVTableLine::operator=(const SVTableLine &s) {
    m_cells.destroy();
    for(s32 i=0;i<s.m_cells.size();i++) {
        m_cells.append(s.m_cells[i]);
    }
    return *this;
}

cptr8 SVTableLine::getStrValue(s32 _index) {
    if(_index<m_cells.size() ) {
        return m_cells[_index].getStrValue();
    }
    return nullptr;
}
//

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

void SVTable::setHead(cptr8 _head) {
    if(!_hasHead) {
        _hasHead = true;
        SVStringArray<> t_tbl_head = SVString::split(_head,",");
        for(s32 i=0;i<t_tbl_head.size();i++) {
            SVString t_str = t_tbl_head[i];
            m_tblHead.append(t_str);
        }
    }
}

void SVTable::pushCxt(cptr8 _value) {
    if(!_hasHead)
        return;
    SVStringArray<> t_tbl_cxt = SVString::split(_value,",");
    if(t_tbl_cxt.size() == m_tblHead.size() ) {
        SVTableLine t_line;
        for(s32 i=0;i<t_tbl_cxt.size();i++){
            SVTableCell t_cell;
            t_cell.setValue( _value[i]);
            t_line.m_cells.append(t_cell);
        }
        m_tblCtx.append(t_line);
    }
}

void SVTable::removeCxt(s32 _key) {
    if(!_hasHead)
        return;
}

void SVTable::clearCxt() {
    if(!_hasHead)
        return;
    m_tblCtx.destroy();
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
        return m_tblCtx[_index].getStrValue(t_index);
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
