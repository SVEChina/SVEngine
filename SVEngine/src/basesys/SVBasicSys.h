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
        
        void update(f32 dt);
        
        void output();
        
        //回收模块
        SVRecycleProcessPtr getRecycleModule();
        //拾取模块
        SVPickProcessPtr getPickModule();
        //字体模块
        SVFontProcessPtr getFontModule();
        //流入
        SVStreamInPtr getStreamIn();
        //流出
        SVStreamOutPtr getStreamOut();
        //图片处理
        SVPictureProcessPtr getPicProc();
        
    protected:
        
        SVStreamInPtr m_pStreamIn;
        
        SVStreamOutPtr m_pStreamOut;
        
        SVPictureProcessPtr m_pPicProc;

        SVRecycleProcessPtr m_pRecycleModule;
        
        SVPickProcessPtr m_pPickModule;

        SVFontProcessPtr m_pFontModule;

    };
}//!namespace sv





#endif //SV_BASICSYS_H
