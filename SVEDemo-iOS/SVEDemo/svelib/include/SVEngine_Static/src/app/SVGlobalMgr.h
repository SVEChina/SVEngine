//
// SVGlobalMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GLOBALMGR_H
#define SV_GLOBALMGR_H

#include "../base/SVGBase.h"
#include "../base/SVPreDeclare.h"
#include "../act/SVActDeclare.h"

namespace sv {
    
    class SVGlobalMgr : public SVGBase {
    public:
        SVGlobalMgr(SVInst *_app);
        
        ~SVGlobalMgr();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 dt);
        
        void timeTag(bool _clear,cptr8 _tag);
        
    public:
        //基础模块
        SVFileMgrPtr m_pFileMgr;
        SVConfigPtr m_pConfig;
        SVEventMgrPtr m_pEventMgr;
        SVBasicSysPtr m_pBasicSys;
        SVSceneMgrPtr m_pSceneMgr;
        SVUIMgrPtr m_pUIMgr;
        SVModuleSysPtr m_pModuleSys;
        SVLogicMgrPtr m_pLogicMgr;
        SVShaderMgrPtr m_pShaderMgr;
        SVTexMgrPtr m_pTexMgr;
        SVRenderMgrPtr m_pRenderMgr;
        SVDetectMgrPtr m_pDetectMgr;
        SVStaticDataPtr m_pStaticData;
        SVActionSysPtr m_pActionSys;
        //
        timeval timTagLast;
    };
    
}//!namespace sv



#endif //SV_GLOBALMGR_H
