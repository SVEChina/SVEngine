//
// SVInst.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_INST_H
#define SV_INST_H

#include "../base/SVIDPool.h"
#include "SVGlobalMgr.h"
#include "SVGlobalParam.h"

namespace sv {

    class SVInst : public SVObject {
    public:
        SVInst();
        
        virtual ~SVInst();
        //初始化SV
        virtual void init(s32 _sync = 0);
        //销毁SVå
        virtual void destroy();
        //开始SV
        virtual void startSVE();
        //停止SV
        virtual void stopSVE();
        //同步模式调用的借口
        void updateSVE(f32 _dt);
        //挂起SV
        void svSuspend();
        //将要挂起SV
        void svWillSuspend();
        //唤醒SV
        void svResume();
//        //同步模式
//        void svSync();
//        //异步模式
//        void syASync();
        //获取环境(上下文)
        SVContextBase* getContext();
        //设置时间状态
        void setTimeState(SV_ENG_TIMESTATE _mode);
        //获取时间状态
        SV_ENG_TIMESTATE getTimeState();
        //增加资源路径
        void addRespath(cptr8 _path);
        //获取引擎状态
        inline SV_STATE getState() { return m_svst; }
        
    public:
        //唯一ID池
        SVIDPool m_IDPool;
        //全局对象
        SVGlobalMgrPtr m_pGlobalMgr;
        //全局参数
        SVGlobalParamPtr m_pGlobalParam;
        //线程管理部分
        SVThreadPoolPtr m_pTPool;
        
    protected:
        s32 m_sync;
        SV_STATE m_svst;
        SV_ENG_TIMESTATE m_engTimeState;
        
    public:
        //获取各种模块，管理类
        SVFileMgrPtr getFileMgr();
        SVConfigPtr getConfig();
        SVEventMgrPtr getEventMgr();
        SVBasicSysPtr getBasicSys();
        SVCameraMgrPtr getCameraMgr();
        SVSceneMgrPtr getSceneMgr();
        SVUIMgrPtr getUIMgr();
        SVModuleSysPtr getModuleSys();
        SVShaderMgrPtr getShaderMgr();
        SVTexMgrPtr getTexMgr();
        SVRenderMgrPtr getRenderMgr();
        SVDetectMgrPtr getDetectMgr();
        SVDeformMgrPtr getDeformMgr();
        SVStaticDataPtr getDataMgr();
        SVActionMgrPtr getActionMgr();
        SVModelMgrPtr getModelMgr();
        SVRendererBasePtr getRenderer();
        SVRenderStatePtr getRenderState();
    };
    
}//!namespace sv



#endif //SV_INST_H
