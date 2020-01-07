//
// SVLoaderBat.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVLoaderBat.h"
#include "../app/SVInst.h"
#include "../base/SVDataChunk.h"
#include "SVFileMgr.h"

SVLoaderBat::SVLoaderBat(SVInst *_app)
:SVFileLoader(_app) {
    m_dataCxt.clear();
}

bool SVLoaderBat::loadFromFile(cptr8 _filename,SVTable& _tbl) {
    SVDataChunk tSVDataChunk;
    bool t_flag = mApp->getFileMgr()->loadFileContent(&tSVDataChunk, _filename);
    if (t_flag) {
        SVStringArray<> t_str_array = SVString::split(tSVDataChunk.m_data,"\r");
        for(s32 i=0;i<t_str_array.size();i++) {
            SVString t_line_value = t_str_array[i];
            if(i == 0) {
                SVStringArray<> t_tbl_head = SVString::split(t_line_value.c_str(),",");
                _tbl.setHead(t_tbl_head);
            }else{
                SVStringArray<> t_tbl_cxt = SVString::split(t_line_value.c_str(),",");
                if(t_tbl_cxt.size()>0) {
                    s32 t_key = SVString::atoi(t_tbl_cxt[0]);
                    _tbl.pushCxt(t_key, t_tbl_cxt);
                }
            }
        }
    }
    return true;
}

void SVLoaderBat::_parse() {
    s32 t_len = m_dataCxt.size();
    for(s32 i=0;i<t_len;i++) {
        SVStringArray<> t_str_array = SVString::split(m_dataCxt[i],",");
        for(s32 j=0;j<t_str_array.size();j++) {
            SVString t_str = t_str_array[j];
        }
    }
}
