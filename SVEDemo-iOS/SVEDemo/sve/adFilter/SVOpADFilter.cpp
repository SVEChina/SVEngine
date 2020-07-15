//
// SVOpADFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li，daming Li
//

#include "SVOpADFilter.h"
#include "app/SVInst.h"
#include "basesys/SVBasicSys.h"
#include "module/SVModuleSys.h"
#include "module/SVModuleBase.h"
#include "SVADFilter.h"
#include "SVADFilterDef.h"
//
SVOpLoadADFilter::SVOpLoadADFilter(SVInst *_app)
:SVOpBase(_app){
    
}

SVOpLoadADFilter::~SVOpLoadADFilter(){
    
}

void SVOpLoadADFilter::_process(f32 dt){
    SVString t_name = "SV_ADFilter";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr == nullptr) {
        SVADFilterPtr t_filterModule = MakeSharedPtr<SVADFilter>(mApp);
        if (t_filterModule) {
            t_filterModule->open();
            mApp->getModuleSys()->regist(t_filterModule, t_name.c_str());
        }
    }
}

SVOpDestroyADFilter::SVOpDestroyADFilter(SVInst *_app)
:SVOpBase(_app) {
    
}

SVOpDestroyADFilter::~SVOpDestroyADFilter(){
    
}

void SVOpDestroyADFilter::_process(float dt) {
    SVString t_name = "SV_ADFilter";
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_module) {
        mApp->getModuleSys()->unregist(t_name.c_str());
        t_module->close();
        t_module->destroy();
    }
    
}

