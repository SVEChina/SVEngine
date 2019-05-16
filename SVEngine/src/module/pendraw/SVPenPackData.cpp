//
// SVPenPackData.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPenPackData.h"
#include "../../app/SVInst.h"
#include "../../base/SVDataSwap.h"
#include "../../file/SVFileMgr.h"
SVPenPackData::SVPenPackData(SVInst *_app)
:SVGBase(_app) {
    
}

void SVPenPackData::reset(){
    
}

SVPenPackData::~SVPenPackData() {
    
}

bool SVPenPackData::loadPenStrokeData(SVDataSwapPtr _data, cptr8 _path, s32 _offset, s32 _length){
    if (_data && _length) {
        SVDataChunk dataChunk;
        bool t_flag = mApp->getFileMgr()->loadFileData(&dataChunk, _path, _offset, _length);
        if (!t_flag) {
            SV_LOG_ERROR("SVPenPackData::Load Stroke Data Error %s\n", _path);
            return false;
        }else {
            _data->writeData(dataChunk.m_data, dataChunk.m_size);
        }
    }
    return true;
}

bool SVPenPackData::savePenJsonData(SVDataSwapPtr _data, cptr8 _path){
    if (_data && _data->getSize()) {
        bool t_flag = mApp->getFileMgr()->writeFileData(_data, _path);
        if (!t_flag) {
            SV_LOG_ERROR("SVPenPackData::Save Stroke Data Error %s\n", _path);
            return false;
        }
    }
    return true;
}

bool SVPenPackData::appendPenStrokeData(SVDataSwapPtr _data, cptr8 _path){
    if (_data && _data->getSize()) {
        bool t_flag = mApp->getFileMgr()->appendFileData(_data, _path);
        if (!t_flag) {
            SV_LOG_ERROR("SVPenPackData::Save Stroke Data Error %s\n", _path);
            return false;
        }
    }
    return true;
}

u64 SVPenPackData::checkPenStrokeDataLength(cptr8 _path){
    return mApp->getFileMgr()->checkFileDataLength(_path);
    
}
