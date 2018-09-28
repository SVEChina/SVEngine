//
// SVModuleSys.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODULESYS_H
#define SV_MODULESYS_H

#include "../basesys/SVSysBase.h"
#include "../base/SVMap.h"

namespace sv{
    
    class SVModuleBase;
    
    class SVModuleSys : public SVSysBase {
    public:
        SVModuleSys(SVInst* _app);
        
        ~SVModuleSys();
        
        void init();
        
        void destroy();
        
        void update(f32 _dt);
        
        void regist(SVModuleBasePtr _module,cptr8 _name);
        
        void unregist(cptr8 _name);
        
        bool hasRegist(cptr8 _name);
        
        SVModuleBasePtr getModule(cptr8 _name);
        
        bool openModule(cptr8 _name);
        
        bool closeModule(cptr8 _name);

    protected:
        typedef SVMap<SVString,SVModuleBasePtr> MODULEPOOL;
        
        MODULEPOOL m_modulePool;
        
        SVLockPtr m_moduleLock;
    };
    
}//!namespace sv





#endif //SV_MODULESYS_H
