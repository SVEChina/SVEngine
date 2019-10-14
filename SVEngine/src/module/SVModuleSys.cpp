//
// SVModuleSys.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModuleSys.h"
#include "SVModuleBase.h"
#include "../base/SVLock.h"
SVModuleSys::SVModuleSys(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVModuleSys";
    m_moduleLock = MakeSharedPtr<SVLock>();
}

SVModuleSys::~SVModuleSys() {
    m_moduleLock = nullptr;
}

void SVModuleSys::init() {

}

void SVModuleSys::destroy() {
    m_moduleLock->lock();
    m_modulePool.clear();
    m_moduleLock->unlock();
}

void SVModuleSys::update(f32 _dt) {
    m_moduleLock->lock();
    MODULEPOOL::Iterator it = m_modulePool.begin();
    while(it!=m_modulePool.end() && it->data) {
        if( it->data->isOpen() ) {
            it->data->update(_dt);
        }
        it++;
    }
    m_moduleLock->unlock();
}

void SVModuleSys::regist(SVModuleBasePtr _module,cptr8 _name) {
    m_moduleLock->lock();
    if (hasRegist(SVString(_name))) {
        return;
    }
    m_modulePool.append(SVString(_name),_module);
    m_moduleLock->unlock();
}

void SVModuleSys::unregist(cptr8 _name) {
    m_moduleLock->lock();
    SVMap<SVString, SVModuleBasePtr>::Iterator it = m_modulePool.find(SVString(_name));
    if (it != m_modulePool.end()) {
        m_modulePool.remove(_name);
    }
    m_moduleLock->unlock();
}

bool SVModuleSys::hasRegist(cptr8 _name){
    SVMap<SVString, SVModuleBasePtr>::Iterator it = m_modulePool.find(SVString(_name));
    if (it != m_modulePool.end()) {
        return true;
    }
    return false;
}

SVModuleBasePtr SVModuleSys::getModule(cptr8 _name){
    SVMap<SVString, SVModuleBasePtr>::Iterator it = m_modulePool.find(SVString(_name));
    if (it != m_modulePool.end()) {
        SVModuleBasePtr t_module = it->data;
        return t_module;
    }
    return nullptr;
}

bool SVModuleSys::openModule(cptr8 _name){
    bool t_ret = false;
    m_moduleLock->lock();
    SVMap<SVString, SVModuleBasePtr>::Iterator it = m_modulePool.find(SVString(_name));
    if (it != m_modulePool.end()) {
        SVModuleBasePtr t_module = it->data;
        if(t_module){
            t_module->open();
            t_ret = true;
        }
    }
    m_moduleLock->unlock();
    return t_ret;
}

bool SVModuleSys::closeModule(cptr8 _name) {
    bool t_ret = false;
    m_moduleLock->lock();
    SVMap<SVString, SVModuleBasePtr>::Iterator it = m_modulePool.find(SVString(_name));
    if (it != m_modulePool.end()) {
        SVModuleBasePtr t_module = it->data;
        if(t_module){
            t_module->close();
            t_ret = true;
        }
    }
    m_moduleLock->unlock();
    return t_ret;
}
