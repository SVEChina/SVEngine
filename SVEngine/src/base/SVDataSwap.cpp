// SVDataSwap.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVDataSwap.h"
#include <stdlib.h>

SVDataSwap::SVDataSwap()
:m_pData(nullptr)
,m_cacheSize(0)
,m_realSize(0){
    m_swapLock = MakeSharedPtr<SVLock>();
}

SVDataSwap::~SVDataSwap() {
    reback();
    m_swapLock = nullptr;
}

//重新分配空间
void SVDataSwap::resize(u32 _size) {
    m_swapLock->lock();
    if (m_cacheSize != _size) {
        free(m_pData);
        m_pData = malloc(_size);
        memset(m_pData,0,_size);
        m_cacheSize = _size;
        m_realSize = 0;
    }
    m_swapLock->unlock();
}

//扩展空间(保留原始数据)
void SVDataSwap::extendSize(u32 _size){
    if(_size<=m_cacheSize)
        return ;    //如果扩展的数据小于原来的数据 则不扩展
    m_swapLock->lock();
    void* t_pNewdata = malloc(_size);
    //置0
    memset(t_pNewdata,0,_size);
    //保留旧数据
    memcpy(t_pNewdata,m_pData,m_realSize);
    //更新缓存区大小
    m_cacheSize = _size;
    //释放旧的空间
    free(m_pData);
    //更新旧数据指针
    m_pData = t_pNewdata;
    m_swapLock->unlock();
}

void SVDataSwap::reback() {
    if (m_pData) {
        free(m_pData);
        m_pData = nullptr;
    }
    m_cacheSize = 0;
    m_realSize = 0;
}

//全长写入
void SVDataSwap::writeData(void *_data,u32 _len){
    if(_len>m_cacheSize){
        resize(_len);   //重新分配空间
    }
    m_swapLock->lock();
    memcpy(m_pData, _data, _len);
    m_realSize = _len;
    m_swapLock->unlock();
}

//追加数据
void SVDataSwap::appendData(void *_data,u32 _len){
    if(_len + m_realSize> m_cacheSize ){
        extendSize(_len + m_realSize);  //不丢失数据的拷贝
    }
    m_swapLock->lock();
    memcpy((char*)m_pData + m_realSize, _data, _len);
    m_realSize = m_realSize + _len;
    m_swapLock->unlock();
}

//获取数据
void SVDataSwap::fetchData(void *_data,u32 _len){
    m_swapLock->lock();
    if (_len <= m_cacheSize) {
        memcpy(_data, m_pData, _len);
    }
    m_swapLock->unlock();
}

void *SVDataSwap::getData() {
    return m_pData;
}

u32 SVDataSwap::getSize() {
    return m_realSize;
}

u32 SVDataSwap::getCacheSize(){
    return m_cacheSize;
}

void SVDataSwap::lockData() {
    m_swapLock->lock();
}

void SVDataSwap::unlockData() {
    m_swapLock->unlock();
}

void SVDataSwap::unlockData(u32 _realSize) {
    if(_realSize<m_cacheSize) {
        m_realSize = _realSize;
    }
    m_swapLock->unlock();
}

void SVDataSwap::swap(SVDataSwapPtr pTarget) {
    if (m_pData == nullptr || pTarget == nullptr || pTarget->getData() == nullptr)
        return;

    if (m_realSize != pTarget->getSize())
        return;
    
    void *pCurData = m_pData;
    m_pData = pTarget->m_pData;
    pTarget->m_pData = pCurData;
}
