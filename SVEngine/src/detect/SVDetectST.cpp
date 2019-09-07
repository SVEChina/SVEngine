//
// SVDetectST.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVDetectST.h"
#include "SVPerson.h"
#include "../basesys/SVConfig.h"
SVDetectST::SVDetectST(SVInst *_app)
:SVDetectBase(_app) {
     memset(stData, 0, SUPPORT_MAX_PERSON_NUM * sizeof(PERSON_STDATA));
     memset(stDataOriginal, 0, SUPPORT_MAX_PERSON_NUM * sizeof(PERSON_STDATA));
}

SVDetectST::~SVDetectST() {
}

s32 SVDetectST::transformIndex(s32 index) {
    return 0;
}
void SVDetectST::pushData(void *_faceData){
    _parseData(_faceData);
    //将数据分解到person上
}

void SVDetectST::_parseData(void *data) {
    if (!data)
        return;
    m_listenDirty = true;
    //解析人脸数目
    int t_facecount = 0;
    memcpy(&t_facecount, data, sizeof(int));
    char *t_stream = (char *) data;
    t_stream += sizeof(s32);
    
    for (s32 i = 0; i < SUPPORT_MAX_PERSON_NUM; i++) {
        stData[i].has = false;
    }
    char *t_data_point = (char *) t_stream;
    for (s32 i = 0; i < t_facecount; i++) {
        stData[i].has = true;
        //rotate
        size_t off_len = sizeof(f32);
        memcpy(&stData[i].yaw, t_data_point, off_len);
        t_data_point += off_len;
        memcpy(&stData[i].pitch, t_data_point, off_len);
        t_data_point += off_len;
        memcpy(&stData[i].roll, t_data_point, off_len);
        t_data_point += off_len;
        //rect
        off_len = sizeof(s32);
        memcpy(&stData[i].rectleft, t_data_point, off_len);
        memcpy(&stDataOriginal[i].rectleft,t_data_point, off_len);
        t_data_point += off_len;
        
        memcpy(&stData[i].recttop, t_data_point, off_len);
        memcpy(&stDataOriginal[i].recttop,t_data_point, off_len);
        t_data_point += off_len;
        
        memcpy(&stData[i].rectright, t_data_point, off_len);
        memcpy(&stDataOriginal[i].rectright,t_data_point, off_len);
        t_data_point += off_len;
        
        memcpy(&stData[i].rectbottom, t_data_point, off_len);
        memcpy(&stDataOriginal[i].rectbottom,t_data_point, off_len);
        t_data_point += off_len;

        s32 data_len = sizeof(f32) * 2 * 106;
        memcpy(stData[i].pointdata,t_data_point, data_len);
        memcpy(stDataOriginal[i].pointdata,t_data_point, data_len);
        t_data_point+=data_len;
        _transformFaceDataToCenter(&(stData[i]));
    }
}

PERSON_STDATA *SVDetectST::getData(s32 _personid) {
    if (_personid >= 1 && _personid <= SUPPORT_MAX_PERSON_NUM)
        return &(stData[_personid - 1]);
    return nullptr;
}

PERSON_STDATA *SVDetectST::getDataOriginal(s32 _personid) {
    if (_personid >= 1 && _personid <= SUPPORT_MAX_PERSON_NUM)
        return &(stDataOriginal[_personid - 1]);
    return nullptr;
}

void SVDetectST::_transformFaceDataToCenter(PERSON_STDATA *_stData){
    f32 t_camera_w = mApp->getConfig()->getCameraWidth()*1.0f;
    f32 t_camera_h = mApp->getConfig()->getCameraHeight()*1.0f;
    for (s32 j=0; j<106; j++) {
        _stData->pointdata[j].x = (_stData->pointdata[j].x - t_camera_w*0.5);
        _stData->pointdata[j].y =  (t_camera_h*0.5 - _stData->pointdata[j].y);
    }
}

