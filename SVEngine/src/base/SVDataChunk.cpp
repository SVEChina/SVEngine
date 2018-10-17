//
// SVDataChunk.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVDataChunk.h"

//数据块
SVDataChunk::SVDataChunk()
:m_size(0)
,m_data(nullptr) {
}

SVDataChunk::~SVDataChunk() {
    reback();
}

void SVDataChunk::apply(s32 _size) {
    m_size = _size;
    if (m_size > 0) {
        m_data = (char *) malloc(m_size);
    }
}

void SVDataChunk::reback() {
    if (m_size > 0) {
        free(m_data);
        m_size = 0;
        m_data = NULL;
    }
}

void SVDataChunk::toString() {
    if (m_size > 0) {
        m_data[m_size - 1] = '\0';
    }
}
