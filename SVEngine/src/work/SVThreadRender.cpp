// SVThreadRender.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <unistd.h>
#include "SVThreadRender.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderMgr.h"
#include "../basesys/SVSceneMgr.h"

SVThreadRender::SVThreadRender(SVInst *_app)
: SVThreadWork(_app,"SVThreadRender")
,m_first(true) {
}

SVThreadRender::~SVThreadRender() {
    SV_LOG_INFO("SVThreadRender::~SVThreadRender\n");
}

void SVThreadRender::_innerUpdate(){
    SVThreadWork::_innerUpdate();
//    //逻辑更新
//    mApp->m_pGlobalMgr->update(_getDert());
//    //渲染
//    mApp->getRenderMgr()->render();
}

void SVThreadRender::_innerDestroy(){
    //销毁渲染器资源
    mApp->getRenderMgr()->recycleRes();
    SVThreadWork::_innerDestroy();
}

f32 SVThreadRender::_getDert() {
    f32 dt = 0;
    if(mApp->getTimeState() == ENG_TS_NOR ){
        if (m_first) {
            m_first = false;
            timeval t_curT;
            gettimeofday(&t_curT, NULL);
            m_lastT = t_curT;
            dt = 0.33f;    //按照30帧算的
        } else {
            timeval t_curT;
            gettimeofday(&t_curT, NULL);
            s32 t_ms = s32( (t_curT.tv_sec - m_lastT.tv_sec)*1000+(t_curT.tv_usec - m_lastT.tv_usec) * 0.001f);
            dt = t_ms * 0.001f;
            m_lastT = t_curT;
            if(dt>0.03333f){
                dt = 1.0f/30.0f;
            }
        }
    }else if(mApp->getTimeState() == ENG_TS_FIX ){
        dt = 1.0f/30.0f;
    }else if(mApp->getTimeState() == ENG_TS_STOP ){
        dt = 0.0f;
    }
    return dt;
}

