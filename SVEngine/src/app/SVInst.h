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
    
    /*
        不用单例的原因，是有些需求，需要启动两个引擎
     */

    class SVInst : public SVObject {
    public:
        //
        static SVInstPtr makeCreate();
        //
        SVInst();
        //
        virtual ~SVInst();
        //
        //初始化SV
        virtual void init();
        //销毁SVå
        virtual void destroy();
        //增加资源路径
        void addRespath(cptr8 _path);
        //开始SV
        virtual void startSVE();
        //停止SV
        virtual void stopSVE();
        //更新引擎
        void updateSVE(f32 _dt);
        //渲染引擎
        void renderSVE();
        //清楚缓存
        void clearCache();
        //挂起SV
        void svSuspend();
        //唤醒SV
        void svResume();
        //设置时间状态
        void setTimeState(SV_ENG_TIMESTATE _mode);
        //获取时间状态
        SV_ENG_TIMESTATE getTimeState();
        //获取引擎状态
        inline SV_STATE getState() { return m_svst; }
        
    public:
        //唯一ID池
        SVIDPool m_IDPool;
        //全局对象
        SVGlobalMgrPtr m_pGlobalMgr;
        //全局参数
        SVGlobalParamPtr m_pGlobalParam;
        //
        SV_STATE m_svst;
        //
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
        SVLightSysPtr getLightSys();
        SVModuleSysPtr getModuleSys();
        SVShaderMgrPtr getShaderMgr();
        SVTexMgrPtr getTexMgr();
        SVRenderMgrPtr getRenderMgr();
        SVDetectMgrPtr getDetectMgr();
        SVDeformMgrPtr getDeformMgr();
        SVStaticDataPtr getDataMgr();
        SVActionMgrPtr getActionMgr();
        SVModelMgrPtr getModelMgr();
        SVPhysicsWorldMgrPtr getPhysicsWorldMgr();
        SVRendererPtr getRenderer();
        SVRenderStatePtr getRenderState();
    };
    
}//!namespace sv



#endif //SV_INST_H
