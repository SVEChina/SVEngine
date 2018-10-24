//
// SVOpDestroy.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpDestroy.h"
#include "../app/SVInst.h"
#include "../module/SVModuleSys.h"
#include "../module/SVModuleBase.h"
#include "../module/SVGameBase.h"
#include "../base/SVPreDeclare.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPickProcess.h"

SVOpDestroyEffect::SVOpDestroyEffect(SVInst *_app, cptr8 _name)
:SVOpBase(_app) {
    m_name = _name;
}

void SVOpDestroyEffect::_process(float dt) {
    SVString m_strPath = m_name;
    s32 t_pos = m_strPath.rfind('.');
    s32 t_len = m_strPath.size();
    if (t_pos) {
        m_strPath = SVString::substr(m_strPath, 0, t_pos);
    }
    t_pos = m_strPath.rfind('/');
    if (t_pos) {
        t_len = m_strPath.size();
        m_strPath = SVString::substr(m_strPath, t_pos+1, t_len - t_pos - 1);
    }
    
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(m_strPath.c_str());
    if (t_module) {
        mApp->getModuleSys()->unregist(m_strPath.c_str());
        t_module->close();
        t_module->destroy();
    }
    
}


SVOpDestroyRedPacket::SVOpDestroyRedPacket(SVInst *_app)
:SVOpBase(_app) {
    
}

void SVOpDestroyRedPacket::_process(float dt) {
    //先在这关闭拾取
    SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
    if (t_pickModule) {
        t_pickModule->disablePick();
    }
    SVString t_name = "sv_game_redpacket";
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_module) {
        mApp->getModuleSys()->unregist(t_name.c_str());
        t_module->close();
        t_module->destroy();
    }
  
}

SVOpDestroyHeartFlutter::SVOpDestroyHeartFlutter(SVInst *_app)
:SVOpBase(_app) {
    
}

void SVOpDestroyHeartFlutter::_process(float dt) {
    //先在这关闭拾取
//    SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
//    if (t_pickModule) {
//        t_pickModule->disablePick();
//    }
    SVString t_name = "sv_game_heartflutter";
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_module) {
        mApp->getModuleSys()->unregist(t_name.c_str());
        t_module->close();
        t_module->destroy();
    }
    
}

SVOpDestroyDivision::SVOpDestroyDivision(SVInst *_app)
:SVOpBase(_app) {
    
}

void SVOpDestroyDivision::_process(float dt) {
    SVString t_name = "sv_filter_division";
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_module) {
        mApp->getModuleSys()->unregist(t_name.c_str());
        t_module->close();
        t_module->destroy();
    }
    
}

