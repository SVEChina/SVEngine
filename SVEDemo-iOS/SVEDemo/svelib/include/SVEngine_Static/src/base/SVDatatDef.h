// SVDataDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DATATDEF_H
#define SV_DATATDEF_H

//识别类型
enum DETECTTYPE {
    DETECT_T_NULL = 0,
    DETECT_T_AS,
    DETECT_T_ST,
    DETECT_T_FP,
    DETECT_T_SELF
};

//数据域类型
enum DATAGENTYPE {
    DATAGEN_T_BASE,
    DATAGEN_T_TEST,
    DATAGEN_T_AS,
    DATAGEN_T_ST,
    DATAGEN_T_FP,
    DATAGEN_T_SELF,
    DATAGEN_T_CAMERA_STEAM,
    DATAGEN_T_CAMERA_OPENGL
};

//相机图片格式
enum PICFORMATE {
    SV_PF_GRAY8 = 0,
    SV_PF_YV12,
    SV_PF_I420,
    SV_PF_NV12,
    SV_PF_NV21,
    SV_PF_BGRA,
    SV_PF_RGBA,
    SV_PF_RGB,
    SV_PF_NULL
};

enum BINDREGION {
    BD_REGION_LEFT,
    BD_REGION_RIHGT,
    BD_REGION_CENTER
};

//最大支持人脸数
#define SUPPORT_MAX_PERSON_NUM 4

#endif //SV_DATATDEF_H
