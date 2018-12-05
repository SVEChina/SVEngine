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
    s32 len = m_name.size();
    s32 pos = m_name.rfind('/');
    SVString t_moduleName = SVString::substr(m_name.c_str(), pos+1, len - pos - 1);
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_moduleName.c_str());
    if (t_module) {
        mApp->getModuleSys()->unregist(t_moduleName.c_str());
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

