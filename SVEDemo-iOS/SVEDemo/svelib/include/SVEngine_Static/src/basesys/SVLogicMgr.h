//
// SVLogicMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_LOGICMGR_H
#define SV_LOGICMGR_H

#include "../basesys/SVSysBase.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    //这里都是要走逻辑功能的接口
    
    class SVLogicMgr : public SVSysBase {
    public:
        SVLogicMgr(SVInst *_app);
        
        ~SVLogicMgr();
        
        void init();
        
        void update(f32 dt);
        
        void destroy();
        
        SVStreamInPtr getStreamIn();
        
        SVStreamOutPtr getStreamOut();
        
        SVPictureProcessPtr getPicProc();
        
    protected:
        
        SVStreamInPtr m_pStreamIn;
        
        SVStreamOutPtr m_pStreamOut;
        
        SVPictureProcessPtr m_pPicProc;
    };
    
}//!namespace



#endif //SV_LOGICMGR_H
