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
    m_dataLength = 0;
}

SVPenPackData::~SVPenPackData() {
    
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
        }else{
            m_dataLength += _data->getSize();
        }
    }
    return true;
}

u32 SVPenPackData::getDataLength(){
    return m_dataLength;
}
