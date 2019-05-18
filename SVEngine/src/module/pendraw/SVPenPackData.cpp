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

SVPenPackData::~SVPenPackData() {
    
}

bool SVPenPackData::loadPenData(SVInst* _app, SVDataSwapPtr _data, cptr8 _path, s32 _offset, s32 _length){
    if (_data && _length) {
        SVDataChunk dataChunk;
        bool t_flag = _app->getFileMgr()->loadFileData(&dataChunk, _path, _offset, _length);
        if (!t_flag) {
            SV_LOG_ERROR("SVPenPackData::Load Stroke Data Error %s\n", _path);
            return false;
        }else {
            _data->writeData(dataChunk.m_data, dataChunk.m_size);
        }
    }
    return true;
}

bool SVPenPackData::writePenData(SVInst* _app, SVDataSwapPtr _data, cptr8 _path, bool _clearData){
    if (_data && _data->getSize()) {
        SVDataChunk dataChunk;
        u32 dataSize = _data->getSize();
        dataChunk.apply(dataSize);
        memcpy(dataChunk.m_data, _data->getData(), _data->getSize());
        bool t_flag = _app->getFileMgr()->writeFileData(&dataChunk, _path, dataSize, _clearData);
        if (!t_flag) {
            SV_LOG_ERROR("SVPenPackData::Save Stroke Data Error %s\n", _path);
            return false;
        }
    }
    return true;
}

u64 SVPenPackData::checkPenStrokeDataLength(SVInst* _app, cptr8 _path){
    return _app->getFileMgr()->checkFileDataLength(_path);
    
}
