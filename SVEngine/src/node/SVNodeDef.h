//
// SVNodeDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_NODEDEF_H
#define SV_NODEDEF_H

#include "SVNodeDeclare.h"

//动画状态(停止 播放 暂停)
//停止:不更新+不渲染
//播放:更新+渲染
//暂停:不更新+渲染

enum E_ANISTATE{
    tANI_STATE_STOP = 0,
    tANI_STATE_PLAY,
    tANI_STATE_PAUSE,
    tANI_STATE_WAIT
};

//锚点
enum EUIARCHO {
    E_ARCHO_LT = 0,
    E_ARCHO_CT,
    E_ARCHO_RT,
    E_ARCHO_LC,
    E_ARCHO_CC,
    E_ARCHO_RC,
    E_ARCHO_LB,
    E_ARCHO_CB,
    E_ARCHO_RB
};

#endif //SV_NODEDEF_H
