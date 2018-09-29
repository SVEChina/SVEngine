//
// SVOpCreate.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li，daming Li
//

#include "SVOpCreate.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalParam.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVConfig.h"
#include "../basesys/SVBasicSys.h"
#include "../event/SVEventMgr.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../node/SVCameraNode.h"
#include "../node/SVFrameOutNode.h"
#include "../node/SVNodeVisit.h"
#include "../node/SVSkyDomeNode.h"
#include "../node/SV3DBox.h"
#include "../node/SVSpriteNode.h"
#include "../node/SVParticlesNode.h"
#include "../node/SVFreeTypeNode.h"
#include "../core/SVSpine.h"
#include "../base/SVDataSwap.h"
#include "../file/SVParseMain.h"
#include "../module/SVModuleSys.h"
#include "../module/SVModuleBase.h"
#include "../module/SVRedPacket.h"
#include "../module/SVRedPacketRun.h"
#include "../module/SVRedPacketReady.h"
#include "../module/SVRedPacketEnd.h"
#include "../basesys/SVPickProcess.h"
#include "../module/SVModuleDelaySuspend.h"
#include "../module/SVDivisonFilter.h"
//heart
#include "../module/SVHeartFlutter.h"
#include "../module/SVHeartFlutterReady.h"
#include "../module/SVHeartFlutterRun.h"
#include "../module/SVHeartFlutterEnd.h"
//创建场景OP
SVOpCreateScene::SVOpCreateScene(SVInst *_app,cptr8 name)
: SVOpBase(_app) {
    m_scenename = name;
}

void SVOpCreateScene::_process(f32 dt) {
    //创建逻辑场景
    SVScenePtr t_pScene = MakeSharedPtr<SVScene>(mApp,m_scenename.c_str());
    if (t_pScene) {
        t_pScene->create(); //创建场景树
        mApp->getSceneMgr()->setScene(t_pScene);//设置主场景
    }
}


//删除逻辑场景OP
SVOpDestroyScene::SVOpDestroyScene(SVInst *_app,cptr8 name)
: SVOpBase(_app) {
    m_scenename = name;
}

void SVOpDestroyScene::_process(f32 dt) {
}

//
SVOpCameraControl::SVOpCameraControl(SVInst *_app, bool bOpen, s32 iResId)
: SVOpBase(_app)
, m_bOpen(bOpen)
, m_iResId(iResId) {
}

SVOpCameraControl::~SVOpCameraControl() {
}

void SVOpCameraControl::_process(f32 dt) {
    if (m_bOpen == true) {
//        SVNode *pCam = mApp->m_pGlobalMgr->m_pNodeMgr->getNode(CAMERACONTROL);
//        if (pCam != NULL)
//            return;
//        pCam = mApp->m_pGlobalMgr->m_pNodeMgr->pCreator->createNode(E_NT_CAMERACONTROLFP, "cameraControl");
//        pCam->setname(CAMERACONTROL);
//        mApp->m_pGlobalMgr->m_pNodeMgr->addNode(pCam);
    }
}
//创建天空
SVOpCreateSkyDome::SVOpCreateSkyDome(SVInst *_app,cptr8 pStrPath,s32 resId)
: SVOpBase(_app)
, m_strPath(pStrPath)
,m_iResId(resId) {
}

SVOpCreateSkyDome::~SVOpCreateSkyDome() {
}

void SVOpCreateSkyDome::_process(f32 dt) {
//    SVSkyDomeNode *tSkyNode = (SVSkyDomeNode *) mApp->m_pGlobalMgr->m_pNodeMgr->pCreator->createNode(
//            "SVSkyDomeNode", "skyDome");
//    SVTexture *pTex = mApp->getTexMgr()->getTexture(m_strPath.c_str(), true);
//    tSkyNode->generateSkyDome(pTex, 50, 15, 1.0, 1.0, 2000);
//    SVScenePtr pScene = mApp->getSceneMgr()->getScene();
//    if (pScene) {
//        pScene->addChild(tSkyNode);
//    }
}
//创建粒子节点
SVOpCreateParticle::SVOpCreateParticle(SVInst *_app,cptr8 pStrPath)
: SVOpBase(_app)
, m_strPath(pStrPath) {
}

SVOpCreateParticle::~SVOpCreateParticle() {
}

void SVOpCreateParticle::_process(f32 dt) {
}

//创建特效
SVOpCreateEffcet::SVOpCreateEffcet(SVInst *_app,cptr8 pStrPath)
: SVOpBase(_app)
, m_strPath(pStrPath) {
}

SVOpCreateEffcet::~SVOpCreateEffcet() {
}

void SVOpCreateEffcet::_process(f32 dt) {
    s32 t_pos = m_strPath.rfind('.');
    s32 t_len = m_strPath.size();
    SVString t_ext = SVString::substr(m_strPath, 0, t_pos);
    t_pos = t_ext.rfind('/');
    t_len = t_ext.size();
    SVString t_name = SVString::substr(t_ext, t_pos+1, t_len - t_pos - 1);
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name);
    if (t_modulePtr == nullptr) {
        SVParseMain t_parssMain(mApp);
        t_modulePtr = t_parssMain.parse(m_strPath.c_str(),123);
        if (t_modulePtr) {
            t_modulePtr->setOpCallBack(m_pCB);
            t_modulePtr->open();
            mApp->getModuleSys()->regist(t_modulePtr, t_name.c_str());
        }
    }
}

//
SVOpLoadRedPacket::SVOpLoadRedPacket(SVInst *_app, cptr8 pStrPath)
:SVOpBase(_app){
    m_strPath = pStrPath;
}

SVOpLoadRedPacket::~SVOpLoadRedPacket(){
    
}

void SVOpLoadRedPacket::_process(f32 dt){
    SVString t_name = "sv_game_redpacket";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr == nullptr) {
        SVParseMain t_parssMain(mApp);
        SVString t_path = m_strPath;
        t_modulePtr = t_parssMain.parse(t_path.c_str(),111);
        SVRedPacketPtr t_redPacket = std::dynamic_pointer_cast<SVRedPacket>(t_modulePtr);
        if (t_redPacket) {
            mApp->getModuleSys()->regist(t_modulePtr, t_name.c_str());
        }
    }
}

//创建红包
SVOpCreateRedPacket::SVOpCreateRedPacket(SVInst *_app,cptr8 pStrPath, s32 _totalPacketNum, s32 _mode)
: SVOpBase(_app)
, m_strPath(pStrPath) {
    m_totalPacketNum = _totalPacketNum;
    m_mode = _mode;
}

SVOpCreateRedPacket::~SVOpCreateRedPacket() {
    
}

void SVOpCreateRedPacket::_process(f32 dt) {
    SVString t_name = "sv_game_redpacket";
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_name.c_str());
    SVRedPacketPtr t_redPacket = std::dynamic_pointer_cast<SVRedPacket>(t_module);
    if (t_redPacket) {
        t_redPacket->setOpCallBack(m_pCB);
        //设置红包总数
        t_redPacket->m_packetRun->getPacketUnitMgr()->setWillShowPacketNum(m_totalPacketNum);
        //设置准备动画倒计时模式
        t_redPacket->m_packetReady->setReadyMode((READYMODE)m_mode);
        //进入动画
        t_redPacket->open();
        //开启拾取
        SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
        if (t_pickModule) {
            t_pickModule->enablePick();
        }
    }
 
}

SVOpOpenRedPacketMoney::SVOpOpenRedPacketMoney(SVInst *_app, s32 _value):SVOpBase(_app){
    m_value = _value;
}

SVOpOpenRedPacketMoney::~SVOpOpenRedPacketMoney(){
    
}

void SVOpOpenRedPacketMoney::_process(f32 dt){
    SVString t_name = "sv_game_redpacket";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVRedPacketPtr t_redPacket = std::dynamic_pointer_cast<SVRedPacket>(t_modulePtr);
    if (t_redPacket) {
        t_redPacket->m_packetRun->showMoney(m_value);
    }
}

SVOpWalletPos::SVOpWalletPos(SVInst *_app, f32 _x, f32 _y) : SVOpBase(_app){
    m_x = _x;
    m_y = _y;
}

SVOpWalletPos::~SVOpWalletPos(){
    
}

void SVOpWalletPos::_process(f32 dt){
    SVString t_name = "sv_game_redpacket";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVRedPacketPtr t_redPacket = std::dynamic_pointer_cast<SVRedPacket>(t_modulePtr);
    if (t_redPacket) {
        t_redPacket->m_packetRun->setTarPos(FVec3(m_x, m_y, 0.0));
        t_redPacket->m_packetEnd->setWalletPos(FVec3(m_x, m_y, 0.0));
    }
}

SVOpTotalMoney::SVOpTotalMoney(SVInst *_app, cptr8 _text) : SVOpBase(_app){
    m_text = _text;
}

SVOpTotalMoney::~SVOpTotalMoney(){
    
}

void SVOpTotalMoney::_process(f32 dt){
    SVString t_name = "sv_game_redpacket";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVRedPacketPtr t_redPacket = std::dynamic_pointer_cast<SVRedPacket>(t_modulePtr);
    if (t_redPacket) {
        t_redPacket->m_packetEnd->setText(m_text.c_str());
    }
}

SVOpRemoveOnePacket::SVOpRemoveOnePacket(SVInst *_app, s32 _id): SVOpBase(_app){
    m_id = _id;
}

SVOpRemoveOnePacket::~SVOpRemoveOnePacket(){

    
}

void SVOpRemoveOnePacket::_process(f32 dt){
    SVString t_name = "sv_game_redpacket";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVRedPacketPtr t_redPacket = std::dynamic_pointer_cast<SVRedPacket>(t_modulePtr);
    if (t_redPacket) {
        t_redPacket->m_packetRun->getPacketUnitMgr()->disappearOnePacketWithID(m_id);
    }
}

SVOpOpenDelaySuspend::SVOpOpenDelaySuspend(SVInst *_app, f32 _time) : SVOpBase(_app){
    m_time = _time;
}

SVOpOpenDelaySuspend::~SVOpOpenDelaySuspend(){
    
}

void SVOpOpenDelaySuspend::_process(f32 dt){
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule("sv_delaysuspend");
    SVModuleDelaySuspendPtr t_suspend = std::dynamic_pointer_cast<SVModuleDelaySuspend>(t_modulePtr);
    if (t_suspend) {
        t_suspend->setOpCallBack(m_pCB);
        t_suspend->setAccTime(m_time);
        t_suspend->open();
    }
}

//测试操作
SVOpCreateTest::SVOpCreateTest(SVInst *_app)
: SVOpBase(_app){
    
}

SVOpCreateTest::~SVOpCreateTest(){
    
}

void SVOpCreateTest::_process(f32 dt) {
    //return;
    //创建逻辑场景
    SVScenePtr t_pScene = mApp->getSceneMgr()->getScene();
    if (t_pScene) {
        //创建测试盒子®
//        for(s32 i=0;i<30;i++){
//            SV3DBoxPtr t_testBox = MakeSharedPtr<SV3DBox>(mApp);
//            t_testBox->randomInit();
//            t_pScene->addNode(t_testBox);
//        }
//        //
        SVParticlesNodePtr t_p_node = MakeSharedPtr<SVParticlesNode>(mApp);
        t_p_node->testInit();
        t_pScene->addNode(t_p_node);
//        SVFreetypeNodeParam t_ftype_param;
//        t_ftype_param.m_text = "付一舟Ab1.";
//        t_ftype_param.m_canSelect = true;
//        SVNodePtr t_testFreetype = t_ftype_param.genNode(mApp);
//        t_pScene->addNode(t_testFreetype);
    }
}

//测试操作
SVOpCreateTestSpine::SVOpCreateTestSpine(SVInst *_app, cptr8 _path)
: SVOpBase(_app){
    m_path = _path;
}

SVOpCreateTestSpine::~SVOpCreateTestSpine(){
    
}

void SVOpCreateTestSpine::_process(f32 dt) {
    
    SVSpineNodePtr t_spineNode = MakeSharedPtr<SVSpineNode>(mApp);
    SVString t_spine_json = m_path + "/" + "jiesuan.json";
    SVString t_spine_atlas = m_path + "/" + "jiesuan.atlas";
    SVSpinePtr t_spine = SVSpine::createSpine(mApp, t_spine_json.c_str(), t_spine_atlas.c_str(), 1.0f);
    if ( t_spine ) {
        t_spine->setSpineName(t_spine_atlas.c_str());
        t_spineNode->setSpine(t_spine);
    }
    //创建逻辑场景
    SVScenePtr t_pScene = mApp->getSceneMgr()->getScene();
    if (t_pScene) {
        t_pScene->addNode(t_spineNode);
        t_spineNode->setloop(true);
        t_spineNode->play("jiesuan");
        t_spineNode->setScale(2.0, 2.0, 0.0);
    }
}

SVOpCreateDivision::SVOpCreateDivision(SVInst *_app) :  SVOpBase(_app){
    
}

SVOpCreateDivision::~SVOpCreateDivision(){
    
}

void SVOpCreateDivision::_process(f32 dt) {
    SVString t_name = "sv_filter_division";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr == nullptr) {
        t_modulePtr = MakeSharedPtr<SVDivisonFilter>(mApp);
        t_modulePtr->init();
        t_modulePtr->open();
        mApp->getModuleSys()->regist(t_modulePtr, t_name.c_str());
    }
}


//
SVOpLoadHeartFlutter::SVOpLoadHeartFlutter(SVInst *_app, cptr8 pStrPath)
:SVOpBase(_app){
    m_strPath = pStrPath;
}

SVOpLoadHeartFlutter::~SVOpLoadHeartFlutter(){
    
}

void SVOpLoadHeartFlutter::_process(f32 dt){
    SVString t_name = "sv_game_heartflutter";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr == nullptr) {
        SVParseMain t_parssMain(mApp);
        SVString t_path = m_strPath;
        t_modulePtr = t_parssMain.parse(t_path.c_str(),111);
        SVHeartFlutterPtr t_heartFlutter = std::dynamic_pointer_cast<SVHeartFlutter>(t_modulePtr);
        if (t_heartFlutter) {
            mApp->getModuleSys()->regist(t_modulePtr, t_name.c_str());
        }
    }
}

//创建红包
SVOpBeginHeartFlutter::SVOpBeginHeartFlutter(SVInst *_app,cptr8 pStrPath)
: SVOpBase(_app)
, m_strPath(pStrPath) {
}

SVOpBeginHeartFlutter::~SVOpBeginHeartFlutter() {
    
}

void SVOpBeginHeartFlutter::_process(f32 dt) {
    SVString t_name = "sv_game_heartflutter";
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_name.c_str());
    SVHeartFlutterPtr t_heartFlutter = std::dynamic_pointer_cast<SVHeartFlutter>(t_module);
    if (t_heartFlutter) {
        t_heartFlutter->setOpCallBack(m_pCB);
        //进入动画
        t_heartFlutter->open();
        //开启拾取
//        SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
//        if (t_pickModule) {
//            t_pickModule->enablePick();
//        }
    }
    
}

SVOpDisappearHeartFlutter::SVOpDisappearHeartFlutter(SVInst *_app)
: SVOpBase(_app) {
}

SVOpDisappearHeartFlutter::~SVOpDisappearHeartFlutter() {
    
}

void SVOpDisappearHeartFlutter::_process(f32 dt) {
    SVString t_name = "sv_game_heartflutter";
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_name.c_str());
    SVHeartFlutterPtr t_heartFlutter = std::dynamic_pointer_cast<SVHeartFlutter>(t_module);
    if (t_heartFlutter) {
        t_heartFlutter->m_heartFlutterRun->getHeartUnitMgr()->disappearAllHearUnit();
    }
    
}
