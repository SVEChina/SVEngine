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
        SVStringArray t_str_array;
        t_str_array.setData(tSVDataChunk.m_data,'&');
        for(s32 i=0;i<t_str_array.size();i++) {
            SVString t_line_value = t_str_array[i];
            if(i == 0) {
                _tbl.setHead(t_line_value.c_str());
            }else{
                _tbl.pushCxt(t_line_value.c_str());
            }
        }
    }
    return true;
}

void SVLoaderBat::_parse() {
    s32 t_len = m_dataCxt.size();
    for(s32 i=0;i<t_len;i++) {
        SVStringArray t_str_array;
        t_str_array.setData(m_dataCxt[i],',');
        for(s32 j=0;j<t_str_array.size();j++) {
            SVString t_str = t_str_array[j];
        }
    }
}
