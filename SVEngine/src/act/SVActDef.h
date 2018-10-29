//
// SVActDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#ifndef SV_ACTDEF_H
#define SV_ACTDEF_H

#include <memory>
#include "SVActDeclare.h"

typedef sv::logic::SVActBase *(*act_new_func)();

enum TIMELINETYPE{
    E_TL_T_BASE = 0,
    E_TL_T_DEFORM,  //POS,ROT,SCALE
    E_TL_T_MTL,     //材质
    E_TL_T_EVENT,   //事件
    E_TL_T_MAX
};

enum MOVSTATE{
    E_MV_ST_NULL,
    E_MV_ST_READY,
    E_MV_ST_PAUSE,
    E_MV_ST_PLAY
};

#endif //SV_ACTDEF_H
