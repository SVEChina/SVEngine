//
// SVNodeDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_NODEDEF_H
#define SV_NODEDEF_H

#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace node {
        
        DECLARE_SHAREPTR(SVParticlesWorld);
        DECLARE_SHAREPTR(SVParticlesNode);
        
    }//!namespace node
}//!namespace sv

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



#endif //SV_NODEDEF_H
