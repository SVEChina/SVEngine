//
// SVFileWriter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFileWriter.h"

SVFileWriter::SVFileWriter(SVInst *_app)
:SVGBase(_app)
,m_fp(nullptr){
}

SVFileWriter::~SVFileWriter() {
    destroyFileHandle();
}

void SVFileWriter::setPath(cptr8 _path){
    m_path = _path;
}

void* SVFileWriter::createFileHandle(cptr8 _filename){
    SVString t_fullname = m_path + _filename;
    m_fp = fopen(_filename, "wb");
    return m_fp;
}

void SVFileWriter::destroyFileHandle(){
    if(m_fp){
        fclose(m_fp);
        m_fp = nullptr;
    }
}

bool SVFileWriter::write(void* data,s32 len,s32 count) {
    if(!m_fp)
        return false;
    u64 t_len = fwrite(data,len,count, m_fp);
    if(t_len>0)
        return true;
    return false;
}
