//
// SVBasicSys.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BASICSYS_H
#define SV_BASICSYS_H

#include "SVSysBase.h"

namespace sv{
    
    class SVBasicSys : public SVSysBase {
    public:
        SVBasicSys(SVInst* _app);
        
        ~SVBasicSys();
        
        void init();
        
        void destroy();
        //回收模块
        SVRecycleModulePtr getRecycleModule();
        //拾取模块
        SVPickModulePtr getPickModule();
#ifdef CONFIG_IS_LOAD_FREETYPE
        //字体模块
        SVFontModulePtr getFontModule();
#endif

    protected:
        SVRecycleModulePtr m_pRecycleModule;
        SVPickModulePtr m_pPickModule;
#ifdef CONFIG_IS_LOAD_FREETYPE
        SVFontModulePtr m_pFontModule;
#endif
    };
}//!namespace sv





#endif //SV_BASICSYS_H
