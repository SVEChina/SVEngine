//
// SVGlobalMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVGlobalMgr.h"
#include "SVInst.h"
#include "../basesys/SVConfig.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVFontProcess.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVCameraMgr.h"
#include "../basesys/SVUIMgr.h"
#include "../basesys/SVStaticData.h"
#include "../module/SVModuleSys.h"
#include "../file/SVFileMgr.h"
#include "../event/SVEventMgr.h"
#include "../mtl/SVTexMgr.h"
#include "../detect/SVDetectMgr.h"
#include "../mtl/SVShaderMgr.h"
#include "../rendercore/SVRenderMgr.h"
#include "../act/SVActionSys.h"
#include <sys/time.h>
#include "../base/svstr.h"

SVGlobalMgr::SVGlobalMgr(SVInst *_app)
:SVGBase(_app) {
    //引擎系统和操作指令系统要先建立起来
    m_pFileMgr = MakeSharedPtr<SVFileMgr>(mApp);
    m_pConfig = nullptr;
    m_pEventMgr = nullptr;
    m_pBasicSys = nullptr;
    m_pSceneMgr = nullptr;
    m_pCameraMgr = nullptr;
    m_pUIMgr = nullptr;
    m_pShaderMgr = nullptr;
    m_pTexMgr = nullptr;
    m_pRenderMgr = nullptr;
    m_pDetectMgr = nullptr;
    m_pStaticData = nullptr;
    m_pActionSys = nullptr;
}

SVGlobalMgr::~SVGlobalMgr() {
    m_pFileMgr = nullptr;
}

void SVGlobalMgr::init() {
    //推送默认空路径
    if (!m_pFileMgr) {
        m_pFileMgr = MakeSharedPtr<SVFileMgr>(mApp);
    }
    m_pFileMgr->addRespath("");
    //配置系统
    if (!m_pConfig) {
        m_pConfig = MakeSharedPtr<SVConfig>(mApp);
        m_pConfig->init();
    }
    //加载配置
    m_pConfig->loadConfig();
    //消息系统建立起来
    m_pEventMgr = MakeSharedPtr<SVEventMgr>(mApp);
    m_pEventMgr->init();
    //基础系统
    m_pBasicSys = MakeSharedPtr<SVBasicSys>(mApp);
    m_pBasicSys->init();
    //渲染器初始化
    m_pRenderMgr = MakeSharedPtr<SVRenderMgr>(mApp);
    m_pRenderMgr->init();
    //相机系统
    m_pCameraMgr = MakeSharedPtr<SVCameraMgr>(mApp);
    m_pCameraMgr->init();
    //场景系统
    m_pSceneMgr = MakeSharedPtr<SVSceneMgr>(mApp);
    m_pSceneMgr->init();
    //UI系统
    m_pUIMgr = MakeSharedPtr<SVUIMgr>(mApp);
    m_pUIMgr->init();
    //组件系统
    m_pModuleSys = MakeSharedPtr<SVModuleSys>(mApp);
    m_pModuleSys->init();
    //shader程序初始化
    m_pShaderMgr = MakeSharedPtr<SVShaderMgr>(mApp);
    m_pShaderMgr->init();
    //动画运动管理系统
    m_pActionSys = MakeSharedPtr<SVActionSys>(mApp);
    m_pActionSys->init();
    //纹理管理器初始化
    m_pTexMgr = MakeSharedPtr<SVTexMgr>(mApp);
    m_pTexMgr->init();
    //创建识别对象成功
    m_pDetectMgr = MakeSharedPtr<SVDetectMgr>(mApp);
    m_pDetectMgr->init(DETECT_T_ST);
    //最后构建数据
    m_pStaticData = MakeSharedPtr<SVStaticData>(mApp);
}

void SVGlobalMgr::destroy() {
    //第一销毁数据
    m_pStaticData = nullptr;
    //
    if (m_pDetectMgr) {
        //识别模块
        m_pDetectMgr->destroy();
        m_pDetectMgr = nullptr;
        SV_LOG_ERROR("SVDetectMgr:destroy sucess");
    }
    //UI析构
    if( m_pUIMgr ) {
        m_pUIMgr->destroy();
        m_pUIMgr = nullptr;
        SV_LOG_ERROR("m_pUIMgr:destroy sucess");
    }
    //相机析构
    if (m_pCameraMgr) {
        //要先析构相机上的节点，才能析构场景
        m_pCameraMgr->destroy();
        m_pCameraMgr = nullptr;
        SV_LOG_ERROR("SVCameraMgr:destroy sucess");
    }
    //场景析构
    if (m_pSceneMgr) {
        //场景析够(场景虽然也是节点 但是需要单独管理,因为节点需要挂在场景上,所以在节点没析够前,场景不能析构掉)
        m_pSceneMgr->destroy();
        m_pSceneMgr = nullptr;
        SV_LOG_ERROR("SVSceneMgr:destroy sucess");
    }

    //纹理析够 析构都要用到渲染模块
    if (m_pTexMgr) {
        m_pTexMgr->destroy();
        m_pTexMgr = nullptr;
        SV_LOG_ERROR("SVTexMgr:destroy sucess");
    }
    //shader析构
    if (m_pShaderMgr) {
        //shader 析构都要用到渲染模块
        m_pShaderMgr->destroy();
        m_pShaderMgr = nullptr;
        SV_LOG_ERROR("SVShaderMgr:destroy sucess");
    }
    //动画运动析构
    if (m_pActionSys) {
        m_pActionSys->destroy();
        m_pActionSys = nullptr;
        SV_LOG_ERROR("SVAnimateSysPtr:destroy sucess");
    }
    //组件系统析构
    if(m_pModuleSys) {
        m_pModuleSys->destroy();
        m_pModuleSys = nullptr;
        SV_LOG_ERROR("m_pModuleSys:destroy sucess");
    }
    if (m_pRenderMgr) {
        //渲染析够
        m_pRenderMgr->destroy();
        m_pRenderMgr = nullptr;
        SV_LOG_ERROR("SVRenderMgr:destroy sucess");
    }
    if(m_pBasicSys){
        //基础系统
        m_pBasicSys->destroy();
        m_pBasicSys = nullptr;
        SV_LOG_ERROR("SVBasicSys:destroy sucess");
    }
    if (m_pEventMgr) {
        //事件系统最后析够,因为很多其他模块 会注册监听事件
        m_pEventMgr->destroy();
        m_pEventMgr = nullptr;
        SV_LOG_ERROR("SVEventMgr:destroy sucess");
    }
    if (m_pConfig) {
        //配置系统析够
        m_pConfig->destroy();
        m_pConfig = nullptr;
        SV_LOG_ERROR("SVConfig:destroy sucess");
    }
}

void SVGlobalMgr::update(f32 dt) {
    m_pModuleSys->update(dt);           //组件系统更新
    timeTag(false,"module cost");
    m_pBasicSys->update(dt);            //基础系统更新
    timeTag(false,"basesys cost");
    m_pEventMgr->update(dt);            //事件处理系统更新
    timeTag(false,"event cost");
    m_pActionSys->update(dt);
    timeTag(false,"action cost");
    m_pCameraMgr->update(dt);            //相机更新(节点系统)
    timeTag(false,"camera cost");
    m_pSceneMgr->update(dt);            //场景更新(节点系统)
    timeTag(false,"scene cost");
    m_pUIMgr->update(dt);               //UI更新
    timeTag(false,"ui cost");
    m_pTexMgr->update(dt);              //删除不用的纹理
    timeTag(false,"texmgr cost");
    m_pDetectMgr->update(dt);           //识别更新
}

void SVGlobalMgr::timeTag(bool _clear,cptr8 _tag){
    if(_clear){
        gettimeofday(&timTagLast, nullptr);
        SV_LOG_INFO("timeTag reset %s \n",_tag);
    }else{
        timeval timTagCur;
        gettimeofday(&timTagCur, nullptr);
        s32 dt = s32((timTagCur.tv_sec - timTagLast.tv_sec) * 1000.0f + (timTagCur.tv_usec - timTagLast.tv_usec) * 0.001f);
        timTagLast = timTagCur;
        SV_LOG_INFO("timeTag %d %s\n",dt,_tag);
    }
}
