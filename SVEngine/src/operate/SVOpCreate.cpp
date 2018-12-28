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
#include "../basesys/SVPickProcess.h"
#include "../module/SVModuleDelaySuspend.h"
#include "../module/SVDivisonFilter.h"
//
#include "../file/SVBMFontLoader.h"
#include "../node/SVBMFontNode.h"
#include "../core/SVBMFont.h"
#include "../core/SVglTF.h"
#include "../node/SVGLTFModelNode.h"
#include "../module/SVEffectPackage.h"
#include "../act/SVTexAttachment.h"
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
    s32 len = m_strPath.size();
    s32 pos = m_strPath.rfind('/');
    SVString t_moduleName = SVString::substr(m_strPath.c_str(), pos+1, len - pos - 1);
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_moduleName.c_str());
    if (t_modulePtr == nullptr) {
        SVParseMain t_parssMain(mApp);
        t_modulePtr = t_parssMain.parse(m_strPath.c_str(),123);
        if (t_modulePtr) {
            t_modulePtr->setOpCallBack(m_pCB);
            t_modulePtr->setModuleName(t_moduleName.c_str());
            t_modulePtr->open();
            mApp->getModuleSys()->regist(t_modulePtr, t_moduleName.c_str());
        }
    }
}


SVOpTexAttachment::SVOpTexAttachment(SVInst *_app, cptr8 _strPath, s32 _channel, cptr8 _data, s32 _width, s32 _height):SVOpBase(_app){
    m_strPath = _strPath;
    m_data = _data;
    m_width = _width;
    m_height = _height;
    m_channel = _channel;
}

SVOpTexAttachment::~SVOpTexAttachment(){
    
}

void SVOpTexAttachment::_process(f32 dt) {
    s32 len = m_strPath.size();
    s32 pos = m_strPath.rfind('/');
    SVString t_moduleName = SVString::substr(m_strPath.c_str(), pos+1, len - pos - 1);
    SVModuleBasePtr t_module = mApp->getModuleSys()->getModule(t_moduleName.c_str());
    if (t_module) {
        SVEffectPackagePtr t_effect = std::dynamic_pointer_cast<SVEffectPackage>(t_module);
        if (t_effect) {
            SVTexAttachmentPtr t_attachment = t_effect->getTexAttachment(m_channel);
            if (t_attachment) {
                t_attachment->setAttachmentTex(m_channel, m_data, m_width, m_height);
            }
        }
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
//    if (t_pScene) {
//        //创建测试盒子®
//        for(s32 i=0;i<50;i++){
//            SV3DBoxPtr t_testBox = MakeSharedPtr<SV3DBox>(mApp);
//            t_testBox->randomInit();
//            t_pScene->addNode(t_testBox);
//        }
//#ifdef SV_IOS
//        SVSpriteNodePtr spriteNode = MakeSharedPtr<SVSpriteNode>(mApp);
//        spriteNode->setPosition(0, 0, 1000);
//        NSString *file = [[NSBundle mainBundle] pathForResource:@"sve" ofType:@"bundle"];
//        file = [file stringByAppendingPathComponent:@"svres/HollowKnight.png"];
//        spriteNode->setTexture([file UTF8String]);
//        spriteNode->setSpriteSize(500, 500);
//        t_pScene->addNode(spriteNode);
//#endif
//        //
//        SVParticlesNodePtr t_p_node = MakeSharedPtr<SVParticlesNode>(mApp);
//        t_p_node->testInit();
//        t_pScene->addNode(t_p_node);
//        SVFreetypeNodeParam t_ftype_param;
//        t_ftype_param.m_text = "付一舟Ab1.";
//        t_ftype_param.m_canSelect = true;
//        SVNodePtr t_testFreetype = t_ftype_param.genNode(mApp);
//        t_pScene->addNode(t_testFreetype);
        
        //
#ifdef SV_IOS
        NSString *file = [[NSBundle mainBundle] pathForResource:@"sve" ofType:@"bundle"];
        file = [file stringByAppendingPathComponent:@"svres/chineses.fnt"];
        SVBMFontPtr m_font = SVBMFont::creatFnt([file UTF8String], mApp);
        SVBMFontNodePtr bmNode = MakeSharedPtr<SVBMFontNode>(mApp);
        bmNode->setFont(m_font);
        bmNode->setText("丹分化");
        bmNode->setSpacing(20);
        t_pScene->addNode(bmNode);
#endif
    
//#ifdef SV_IOS
//        NSString *t_resPath = [[NSBundle mainBundle]pathForResource:@"sve" ofType:@"bundle"];
//        t_resPath = [t_resPath stringByAppendingPathComponent:@"svres/x-wing/scene.gltf"];
//        SVGLTF glTFLoader(mApp);
//        GLTFModelPtr t_model =  glTFLoader.loadFromFile([t_resPath UTF8String]);
//        if (t_model) {
//            SVGLTFModelNodePtr t_node3d = MakeSharedPtr<SVGLTFModelNode>(mApp);
//            t_node3d->setPosition(0, 0, 0);
//            t_node3d->setRotation(45, 45, 0);
//            t_node3d->setScale(10.0, 10.0, 10.0);
//            t_node3d->setModel(t_model);
//            t_pScene->addNode(t_node3d);
//        }
//#endif
        
//    }
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
