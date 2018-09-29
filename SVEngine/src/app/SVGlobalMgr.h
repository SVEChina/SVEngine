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
        //文件部分
        SVFileMgrPtr m_pFileMgr;
        //配置部分
        SVConfigPtr m_pConfig;
        //事件部分
        SVEventMgrPtr m_pEventMgr;
        //基础系统
        SVBasicSysPtr m_pBasicSys;
        //场景管理
        SVSceneMgrPtr m_pSceneMgr;
        //相机管理
        SVCameraMgrPtr m_pCameraMgr;
        //UI部分
        SVUIMgrPtr m_pUIMgr;
        //模块(插件)部分
        SVModuleSysPtr m_pModuleSys;
        //shader部分
        SVShaderMgrPtr m_pShaderMgr;
        //纹理部分
        SVTexMgrPtr m_pTexMgr;
        //渲染部分
        SVRenderMgrPtr m_pRenderMgr;
        //识别部分
        SVDetectMgrPtr m_pDetectMgr;
        //动画部分
        SVActionSysPtr m_pActionSys;
        //静态数据
        SVStaticDataPtr m_pStaticData;
        
        //时间标志
        timeval timTagLast;
    };
    
}//!namespace sv



#endif //SV_GLOBALMGR_H
