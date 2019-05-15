//
// SVFileMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFileMgr.h"
#include "../base/SVLock.h"
#include "../base/SVDataSwap.h"
SVFileMgr::SVFileMgr(SVInst *_app)
:SVSysBase(_app) {
    m_fileLock = MakeSharedPtr<SVLock>();
}

SVFileMgr::~SVFileMgr() {
    clearRespath();
    m_fileLock = nullptr;
}

//获取搜索路径数目
s32 SVFileMgr::getSearchPathNum() {
    return m_searchPathPool.size();
}

bool SVFileMgr::_hasRespath(cptr8 _path) {
    for (s32 i = 0; i < m_searchPathPool.size(); i++) {
        if (strcmp(m_searchPathPool[i].c_str(), _path) == 0)
            return true;
    }
    return false;
}

void SVFileMgr::addRespath(cptr8 _path) {
    m_fileLock->lock();
    if (!_hasRespath(_path)) {
        m_searchPathPool.append(_path);
    }
    m_fileLock->unlock();
}

void SVFileMgr::delRespath(cptr8 _path) {
    m_fileLock->lock();
    for (s32 i = 0; i < m_searchPathPool.size(); i++) {
        if (strcmp(m_searchPathPool[i].c_str(), _path) == 0) {
            m_searchPathPool.removeForce(i);
            m_fileLock->unlock();
            return;
        }
    }
    m_fileLock->unlock();
}

void SVFileMgr::clearRespath(){
    m_searchPathPool.destroy();
}

bool SVFileMgr::saveFileData(SVDataSwapPtr _data, cptr8 _fname){
    if (!_data)
        return false;
    SVString t_fullname = getFileFullName(_fname);
    SV_LOG_ERROR("SVFileMgr::saveFileData file name %s\n", t_fullname.c_str());
    if ( !t_fullname.empty() ) {
        FILE *fp = fopen(t_fullname.c_str(), "w");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            fwrite((_data->getData()), sizeof(c8), _data->getSize(), fp);
            fclose(fp);
            return true;
        }
    }
    return false;
}

bool SVFileMgr::loadFileContent(SVDataChunk *_datachunk,cptr8 _fname) {
    if (!_datachunk)
        return false;
    SVString t_fullname = getFileFullName(_fname);
    SV_LOG_ERROR("SVFileMgr::loadFileContent file name %s\n", t_fullname.c_str());
    if ( !t_fullname.empty() ) {
        FILE *fp = fopen(t_fullname.c_str(), "r");
        if (fp) {
            fseek(fp, 0, SEEK_END);
            s64 t_file_len = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            _datachunk->apply((s32)t_file_len);
            fread(_datachunk->m_data, t_file_len, 1, fp);
            fclose(fp);
            return true;
        }
    }
    return false;
}

bool SVFileMgr::loadFileContentStr(SVDataChunk *_datachunk,cptr8 _fname) {
    if (!_datachunk)
        return false;
    SVString t_fullname = getFileFullName(_fname);
    if (strcmp(t_fullname.c_str(), "") != 0) {
        SV_LOG_ERROR("SVFileMgr::getFileFullName %s\n", t_fullname.c_str());
        FILE *fp = fopen(t_fullname.c_str(), "r");
        fseek(fp, 0, SEEK_END);
        s32 t_file_len = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        _datachunk->apply(t_file_len + 1);
        fread(_datachunk->m_data, t_file_len, 1, fp);
        _datachunk->toString();
        fclose(fp);
        return true;
    }
    return false;
}

SVString SVFileMgr::getFileFullName(cptr8 _fname) {
    //返回为"" 证明不在SD卡里面
    SVString tFileName = "";
    m_fileLock->lock();
    for (s32 i = 0; i < m_searchPathPool.size(); i++) {
        SVString t_fullpath = m_searchPathPool[i] + _fname;
        SV_LOG_ERROR("SVFileMgr::getFileFullName %s\n", t_fullpath.c_str());
        FILE *fp = fopen(t_fullpath.c_str(), "r");
        if (fp) {
            fclose(fp);
            tFileName = t_fullpath;
            break;
        }
    }
    m_fileLock->unlock();
    SV_LOG_ERROR("find file %s\n", tFileName.c_str());
    return tFileName;
}




