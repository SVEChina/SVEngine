//
// SVOpCreate.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li，daming Li
//

#include "SVOpCreate.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalParam.h"
#include "../core/SVSpine.h"
#include "../core/SVBMFont.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
#include "../base/SVDataSwap.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVConfig.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPickProcess.h"
#include "../event/SVEventMgr.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../node/SVCameraNode.h"
#include "../node/SVFrameOutNode.h"
#include "../node/SVNodeVisit.h"
#include "../node/SVSkyDomeNode.h"
#include "../node/SV3DBox.h"
#include "../node/SVSpriteNode.h"
#include "../node/SVBillboardNode.h"
#include "../node/SVParticlesNode.h"
#include "../node/SVFreeTypeNode.h"
#include "../node/SVBMFontNode.h"
#include "../node/SVFacePointNode.h"
#include "../node/SVSkinNode.h"
#include "../file/SVParseMain.h"
#include "../file/SVBMFontLoader.h"
#include "../file/SVLoaderGLTF.h"
#include "../file/SVParsePen.h"
#include "../module/SVModuleSys.h"
#include "../module/SVModuleBase.h"
#include "../module/SVModuleDelaySuspend.h"
#include "../module/SVDivisonFilter.h"
#include "../module/SVMark.h"
#include "../module/SVEffectPackage.h"
#include "../module/pendraw/SVPenDraw.h"
#include "../act/SVActTexAttachment.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectBase.h"
#include "../physics/SVPhysicsWorld.h"
#include "../physics/bodies/SVPhysicsBodyRigid.h"
#include "../physics/shapes/SVPhysicsShapeBox.h"
#include "../physics/shapes/SVPhysicsShapeSphere.h"
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
            if (m_pCB) {
                SVString msg = SVString::format("effectpackageloadsucceed_%s",t_moduleName.c_str());
                (*m_pCB)(msg.c_str(), mApp);
            }
            SVEffectPackagePtr t_effectPackage = DYN_TO_SHAREPTR(SVEffectPackage, t_modulePtr);
            if (t_effectPackage) {
                for (s32 i = 0; i<10; i++) {
                    SVActTexAttachmentPtr t_texAtt = t_effectPackage->getTexAttachment(i);
                    if (t_texAtt) {
                        SVActTexAttachment::TEXATTACHSPARAM t_param = t_texAtt->getParam();
                        if (m_pCB) {
                            SVString msg = SVString::format("effectpackagetexattachment_%s_channel:%d_metadata:%s",t_moduleName.c_str(), i, t_param.metadata.c_str());
                            (*m_pCB)(msg.c_str(), mApp);
                        }
                    }
                }
            }
            
        }else{
            if (m_pCB) {
                SVString msg = SVString::format("effectpackageloadfailed_%s",t_moduleName.c_str());
                (*m_pCB)(msg.c_str(), mApp);
            }
        }
    }else{
        if (m_pCB) {
            SVString msg = SVString::format("effectpackageloadfailed_%s",t_moduleName.c_str());
            (*m_pCB)(msg.c_str(), mApp);
        }
    }
}


SVOpTexAttachment::SVOpTexAttachment(SVInst *_app, cptr8 _strPath, s32 _channel, void *_data, s32 _width, s32 _height):SVOpBase(_app){
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
            SVActTexAttachmentPtr t_attachment = t_effect->getTexAttachment(m_channel);
            if (t_attachment) {
                t_attachment->setAttachmentTex(m_data, m_width, m_height);
            }
        }
    }
}


SVOpEngineResume::SVOpEngineResume(SVInst *_app) : SVOpBase(_app){
    
}

SVOpEngineResume::~SVOpEngineResume(){
    
}

void SVOpEngineResume::_process(f32 dt){
    mApp->svResume();
}

SVOpEngineSuspend::SVOpEngineSuspend(SVInst *_app) : SVOpBase(_app){
    
}

SVOpEngineSuspend::~SVOpEngineSuspend(){
    
}

void SVOpEngineSuspend::_process(f32 dt){
    mApp->svSuspend();
}

SVOpEngineDelaySuspend::SVOpEngineDelaySuspend(SVInst *_app, f32 _time) : SVOpBase(_app){
    m_time = _time;
}

SVOpEngineDelaySuspend::~SVOpEngineDelaySuspend(){
    
}

void SVOpEngineDelaySuspend::_process(f32 dt){
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule("sv_delaysuspend");
    SVModuleDelaySuspendPtr t_suspend = std::dynamic_pointer_cast<SVModuleDelaySuspend>(t_modulePtr);
    if (t_suspend) {
        t_suspend->setOpCallBack(m_pCB);
        t_suspend->setAccTime(m_time);
        t_suspend->open();
    }
}

SVOpPushFaceData::SVOpPushFaceData(SVInst *_app, void *_data)
: SVOpBase(_app){
    m_data = _data;
}

SVOpPushFaceData::~SVOpPushFaceData(){
    
}

void SVOpPushFaceData::_process(f32 dt){
    if( mApp->getDetectMgr() ) {
        SVDetectBasePtr t_detect = mApp->getDetectMgr()->getDetect();
        if( t_detect ) {
            t_detect->pushData(m_data);
        }
    }
}

//测试操作
SVOpCreateTest::SVOpCreateTest(SVInst *_app)
: SVOpBase(_app){
    
}

SVOpCreateTest::~SVOpCreateTest(){
    
}

void SVOpCreateTest::_process(f32 dt) {
    
//    SVScenePtr t_pScene = mApp->getSceneMgr()->getScene();
//    if (t_pScene) {
//        //创建测试盒子®
//        for(s32 i=0;i<1;i++){
//            SV3DBoxPtr t_testBox = MakeSharedPtr<SV3DBox>(mApp);
//            t_testBox->randomInit();
//            t_pScene->addNode(t_testBox);
//        }
//    }
    
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
//#ifdef SV_IOS
//        NSString *file = [[NSBundle mainBundle] pathForResource:@"sve" ofType:@"bundle"];
//        file = [file stringByAppendingPathComponent:@"svres/chineses.fnt"];
//        SVBMFontPtr m_font = SVBMFont::creatFnt([file UTF8String], mApp);
//        SVBMFontNodePtr bmNode = MakeSharedPtr<SVBMFontNode>(mApp);
//        bmNode->setFont(m_font);
//        bmNode->setText("丹分化");
//        bmNode->setSpacing(20);
//        t_pScene->addNode(bmNode);
//#endif
    
//#ifdef SV_IOS
//        NSString *t_resPath = [[NSBundle mainBundle]pathForResource:@"sve" ofType:@"bundle"];
//        t_resPath = [t_resPath stringByAppendingPathComponent:@"svres/x-wing/scene.gltf"];
//        SVGLTF glTFLoader(mApp);
//        GLTFModelPtr t_model =  glTFLoader.loadFromFile([t_resPath UTF8String]);
//        if (t_model) {
//            SVSkinNodePtr t_node3d = MakeSharedPtr<SVSkinNode>(mApp);
//            t_node3d->setPosition(0, 0, 0);
//            t_node3d->setRotation(45, 45, 0);
//            t_node3d->setScale(10.0, 10.0, 10.0);
//            t_node3d->setModel(t_model);
//            t_pScene->addNode(t_node3d);
//        }
//#endif
        
//    }
    
//    SVBillboardNodePtr billboardNode = MakeSharedPtr<SVBillboardNode>(mApp);
//    billboardNode->setPosition(0, 0, 100);
//    cptr8 file = "svres/sprite/HollowKnight.png";
//    SVTexturePtr texture = mApp->getTexMgr()->getTexture(file,true);
//    billboardNode->setTexture(texture);
//    billboardNode->setSize(500, 500);
//    t_pScene->addNode(billboardNode);
    
    
    SVScenePtr t_pScene = mApp->getSceneMgr()->getScene();
    if (t_pScene) {
        //创建3d

        SVPhysicsShapeBoxPtr t_shapebox=MakeSharedPtr<SVPhysicsShapeBox>(mApp,FVec3(100.0,1,100.0));
        t_shapebox->setMass(0.0);
        t_shapebox->setLocalInertia(FVec3(0.0,0.0,0.0));
        t_shapebox->init();
        SVPhysicsBodyRigidPtr t_body=MakeSharedPtr<SVPhysicsBodyRigid>(mApp,t_shapebox);
        t_body->setOrigin(FVec3(0.0,0.0,0));
        t_body->init();
        mApp->m_pGlobalMgr->m_pPhysics->addBody(t_body);
//
        SVPhysicsShapeBoxPtr t_shapebox1=MakeSharedPtr<SVPhysicsShapeBox>(mApp,FVec3(0.1,10.0,10));
        t_shapebox1->setMass(0.0);
        t_shapebox1->setLocalInertia(FVec3(0.0,0.0,0.0));
        t_shapebox1->init();
        SVPhysicsBodyRigidPtr t_body2=MakeSharedPtr<SVPhysicsBodyRigid>(mApp,t_shapebox1);
        t_body2->setOrigin(FVec3(-1,0,0));
        t_body2->init();
        mApp->m_pGlobalMgr->m_pPhysics->addBody(t_body2);



        SVPhysicsShapeBoxPtr t_shapebox3=MakeSharedPtr<SVPhysicsShapeBox>(mApp,FVec3(0.1,10.0,10));
        t_shapebox3->setMass(0.0);
        t_shapebox3->setLocalInertia(FVec3(0.0,0.0,0.0));
        t_shapebox3->init();
        SVPhysicsBodyRigidPtr t_body3=MakeSharedPtr<SVPhysicsBodyRigid>(mApp,t_shapebox3);
        t_body3->setOrigin(FVec3(1,0,0));
        t_body3->init();
        mApp->m_pGlobalMgr->m_pPhysics->addBody(t_body3);

        SVPhysicsShapeBoxPtr t_shapebox4=MakeSharedPtr<SVPhysicsShapeBox>(mApp,FVec3(10,10,0.01));
        t_shapebox4->setMass(0.0);
        t_shapebox4->setLocalInertia(FVec3(0.0,0.0,0.0));
        t_shapebox4->init();
        SVPhysicsBodyRigidPtr t_body4=MakeSharedPtr<SVPhysicsBodyRigid>(mApp,t_shapebox4);
        t_body4->setOrigin(FVec3(-2,-2,1.0));
        t_body4->init();
        mApp->m_pGlobalMgr->m_pPhysics->addBody(t_body4);

//
        SVPhysicsShapeBoxPtr t_shapebox5=MakeSharedPtr<SVPhysicsShapeBox>(mApp,FVec3(10.0,10.0,0.01));
        t_shapebox5->setMass(0.0);
        t_shapebox5->setLocalInertia(FVec3(0.0,0.0,0.0));
        t_shapebox5->init();
        SVPhysicsBodyRigidPtr t_body5=MakeSharedPtr<SVPhysicsBodyRigid>(mApp,t_shapebox5);
        t_body5->setOrigin(FVec3(-2,-2,-0.4));
        t_body5->init();
        mApp->m_pGlobalMgr->m_pPhysics->addBody(t_body5);


        //50.0/250.0 FVec3(50.0/250.0,50.0/250.0,50.0/250.0)
        SVPhysicsShapeBoxPtr t_shapeShere=MakeSharedPtr<SVPhysicsShapeBox>(mApp,FVec3(0.2,0.2,0.2));
        t_shapeShere->setMass(1.0);
        t_shapeShere->setLocalInertia(FVec3(0,0,0));
        t_shapeShere->init();
        for (int k = 0; k < 1; k++)
        {
            for (int i = 0; i < 1; i++)
            {
                for (int j = 0; j < 2; j++)
                {
//                    SV3DBoxPtr t_testBox = MakeSharedPtr<SV3DBox>(mApp);
//                   // t_testBox->randomInit();
//                    t_testBox->setScale(1.0,1.0,1.0);
//                    t_testBox->setPosition(0.0, 0.0, 0.0);
//                    t_testBox->setRotation(0, 0, 0);

                    SVLoaderGLTF t_load(mApp);
                    t_load.loadFromFile("svres/gltf/ShaiZi_2/ShaiZi.gltf");
                    t_load.building();
                    SVNodePtr t_testBox =t_load.getNode("default009");
                    t_testBox->setScale(0.001f,0.001f,0.001f);
                    //t_testBox->setdrawAABB(true);
                    // t_testBox->setcanSelect(true);
                    t_pScene->addNode(t_testBox);
                    SVPhysicsBodyRigidPtr t_body01=MakeSharedPtr<SVPhysicsBodyRigid>(mApp,t_shapeShere);
                    t_body01->setOrigin(FVec3(0.4 * i,5+k*0.4,0.4 * j));
                    t_body01->setNode(t_testBox);
                    t_body01->init();
                    mApp->m_pGlobalMgr->m_pPhysics->addBody(t_body01);
                    t_body01->addConstraint();
                    mApp->m_pGlobalMgr->m_pPhysics->setp();
                    t_body01->removeConstraint();
                }
            }

        }
    }

}


//测试操作
SVOpCreateDebugFacePt::SVOpCreateDebugFacePt(SVInst *_app)
: SVOpBase(_app){

}

SVOpCreateDebugFacePt::~SVOpCreateDebugFacePt(){
    
}

void SVOpCreateDebugFacePt::_process(f32 dt) {
    //创建逻辑场景
    SVScenePtr t_pScene = mApp->getSceneMgr()->getScene();
    if (t_pScene) {
        SVFacePointNodePtr t_facePtNode = MakeSharedPtr<SVFacePointNode>(mApp);
        t_pScene->addNode(t_facePtNode);
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
//about mark
SVOpCreateMark::SVOpCreateMark(SVInst *_app) :  SVOpBase(_app){
    
}

SVOpCreateMark::~SVOpCreateMark(){
    
}

void SVOpCreateMark::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr == nullptr) {
        t_modulePtr = MakeSharedPtr<SVMark>(mApp);
        t_modulePtr->init();
        t_modulePtr->open();
        mApp->getModuleSys()->regist(t_modulePtr, t_name.c_str());
    }
}

SVOpMarkContent::SVOpMarkContent(SVInst *_app, SVString _content) :  SVOpBase(_app){
    m_content = _content;
}

SVOpMarkContent::~SVOpMarkContent(){
    
}

void SVOpMarkContent::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVMarkPtr t_mark = DYN_TO_SHAREPTR(SVMark, t_modulePtr);
    if (t_mark) {
        t_mark->setContent(m_content);
    }
}

SVOpMarkAlpha::SVOpMarkAlpha(SVInst *_app, f32 _alpha) :  SVOpBase(_app){
    m_alpha = _alpha;
}

SVOpMarkAlpha::~SVOpMarkAlpha(){
    
}

void SVOpMarkAlpha::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVMarkPtr t_mark = DYN_TO_SHAREPTR(SVMark, t_modulePtr);
    if (t_mark) {
        t_mark->setAlpha(m_alpha);
    }
}

SVOpMarkScale::SVOpMarkScale(SVInst *_app, f32 _scale) :  SVOpBase(_app){
    m_scale = _scale;
}

SVOpMarkScale::~SVOpMarkScale(){
    
}

void SVOpMarkScale::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVMarkPtr t_mark = DYN_TO_SHAREPTR(SVMark, t_modulePtr);
    if (t_mark) {
        t_mark->setScale(m_scale);
    }
}

SVOpMarkRotation::SVOpMarkRotation(SVInst *_app, f32 _roation) :  SVOpBase(_app){
    m_rotation = _roation;
}

SVOpMarkRotation::~SVOpMarkRotation(){
    
}

void SVOpMarkRotation::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVMarkPtr t_mark = DYN_TO_SHAREPTR(SVMark, t_modulePtr);
    if (t_mark) {
        t_mark->setRotation(m_rotation);
    }
}

SVOpMarkAlphaTime::SVOpMarkAlphaTime(SVInst *_app, f32 _alphaTime) :  SVOpBase(_app){
    m_alphaTime = _alphaTime;
}

SVOpMarkAlphaTime::~SVOpMarkAlphaTime(){
    
}

void SVOpMarkAlphaTime::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVMarkPtr t_mark = DYN_TO_SHAREPTR(SVMark, t_modulePtr);
    if (t_mark) {
        t_mark->setAlphaTime(m_alphaTime);
    }
}

SVOpMarkAlphaRange::SVOpMarkAlphaRange(SVInst *_app, f32 _srcAlpha, f32 _tarAlpha) :  SVOpBase(_app){
    m_srcAlpha = _srcAlpha;
    m_tarAlpha = _tarAlpha;
}

SVOpMarkAlphaRange::~SVOpMarkAlphaRange(){
    
}

void SVOpMarkAlphaRange::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVMarkPtr t_mark = DYN_TO_SHAREPTR(SVMark, t_modulePtr);
    if (t_mark) {
        t_mark->setAlphaRange(m_srcAlpha, m_tarAlpha);
    }
}

SVOpMarkAppearTime::SVOpMarkAppearTime(SVInst *_app, f32 _appearTime) :  SVOpBase(_app){
    m_appearTime = _appearTime;
}

SVOpMarkAppearTime::~SVOpMarkAppearTime(){
    
}

void SVOpMarkAppearTime::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVMarkPtr t_mark = DYN_TO_SHAREPTR(SVMark, t_modulePtr);
    if (t_mark) {
        t_mark->setAppearRate(m_appearTime);
    }
}

SVOpMarkPosition::SVOpMarkPosition(SVInst *_app, FVec3 _pos) :  SVOpBase(_app){
    m_pos = _pos;
}

SVOpMarkPosition::~SVOpMarkPosition(){
    
}

void SVOpMarkPosition::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVMarkPtr t_mark = DYN_TO_SHAREPTR(SVMark, t_modulePtr);
    if (t_mark) {
        f32 t_x = m_pos.x;
        f32 t_y = m_pos.y;
        s32 t_camera_w = mApp->m_pGlobalParam->m_inner_width;
        s32 t_camera_h = mApp->m_pGlobalParam->m_inner_height;
        f32 t_mod_x = t_x;
        f32 t_mod_y = t_camera_h - t_y;
        f32 t_n_targetX = t_mod_x - t_camera_w*0.5;
        f32 t_n_targetY =  t_camera_h*0.5 - t_mod_y;
        t_mark->setPosition(t_n_targetX, t_n_targetY, 0.0f);
    }
}

SVOpMarkEnableRandom::SVOpMarkEnableRandom(SVInst *_app, bool _enable) :  SVOpBase(_app){
    m_enable = _enable;
}

SVOpMarkEnableRandom::~SVOpMarkEnableRandom(){
    
}

void SVOpMarkEnableRandom::_process(f32 dt) {
    SVString t_name = "sv_mark_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    SVMarkPtr t_mark = DYN_TO_SHAREPTR(SVMark, t_modulePtr);
    if (t_mark) {
        t_mark->setEnableRandomPosition(m_enable);
    }
}

//about pen
SVOpOpenPen::SVOpOpenPen(SVInst *_app) :  SVOpBase(_app){
    
}

SVOpOpenPen::~SVOpOpenPen(){
    
}

void SVOpOpenPen::_process(f32 dt) {
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr == nullptr) {
        t_modulePtr = MakeSharedPtr<SVPenDraw>(mApp);
        SVGameBasePtr gameBasePtr = DYN_TO_SHAREPTR(SVGameBase, t_modulePtr);
        if (gameBasePtr) {
            gameBasePtr->init(nullptr, nullptr, nullptr);
            gameBasePtr->open();
            mApp->getModuleSys()->regist(gameBasePtr, t_name.c_str());
        }

    }
}

SVOpSetPenEffcet::SVOpSetPenEffcet(SVInst *_app,cptr8 pStrPath)
: SVOpBase(_app)
, m_strPath(pStrPath) {
}

SVOpSetPenEffcet::~SVOpSetPenEffcet() {
}

void SVOpSetPenEffcet::_process(f32 dt) {
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr) {
        SVParsePen t_parssPen(mApp);
        t_parssPen.parse(m_strPath.c_str(),123);
    }
}


SVOpSavePenEffcet::SVOpSavePenEffcet(SVInst *_app,cptr8 pStrPath)
: SVOpBase(_app)
, m_strPath(pStrPath) {
}

SVOpSavePenEffcet::~SVOpSavePenEffcet() {
}

void SVOpSavePenEffcet::_process(f32 dt) {
    SVString result = "false";
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr) {
        SVPenDrawPtr t_penDraw = DYN_TO_SHAREPTR(SVPenDraw, t_modulePtr);
        if (t_penDraw) {
            if (t_penDraw->save(m_strPath)) {
                result = "true";
            }
        }
    }
    if (m_pCB) {
        (*m_pCB)(result, mApp);
    }
}

SVOpClearPen::SVOpClearPen(SVInst *_app) :  SVOpBase(_app){
    
}

SVOpClearPen::~SVOpClearPen(){
    
}

void SVOpClearPen::_process(f32 dt) {
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr) {
        SVPenDrawPtr t_penDraw = DYN_TO_SHAREPTR(SVPenDraw, t_modulePtr);
        if (t_penDraw) {
            t_penDraw->clear();
        }
    }
}

SVOpPenUndo::SVOpPenUndo(SVInst *_app) :  SVOpBase(_app){
    
}

SVOpPenUndo::~SVOpPenUndo(){
    
}

void SVOpPenUndo::_process(f32 dt) {
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr) {
        SVPenDrawPtr t_penDraw = DYN_TO_SHAREPTR(SVPenDraw, t_modulePtr);
        if (t_penDraw) {
            t_penDraw->undo();
        }
    }
}

SVOpPenRedo::SVOpPenRedo(SVInst *_app) :  SVOpBase(_app){
    
}

SVOpPenRedo::~SVOpPenRedo(){
    
}

void SVOpPenRedo::_process(f32 dt) {
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr) {
        SVPenDrawPtr t_penDraw = DYN_TO_SHAREPTR(SVPenDraw, t_modulePtr);
        if (t_penDraw) {
            t_penDraw->redo();
        }
    }
}

SVOpPenUndoIsEnable::SVOpPenUndoIsEnable(SVInst *_app) :  SVOpBase(_app){
    
}

SVOpPenUndoIsEnable::~SVOpPenUndoIsEnable(){
    
}

void SVOpPenUndoIsEnable::_process(f32 dt) {
    SVString result = "false";
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr) {
        SVPenDrawPtr t_penDraw = DYN_TO_SHAREPTR(SVPenDraw, t_modulePtr);
        if (t_penDraw) {
            if (t_penDraw->isUndoEnable()) {
                result = "true";
            }
        }
    }
    if (m_pCB) {
        (*m_pCB)(result, mApp);
    }
}

SVOpPenRedoIsEnable::SVOpPenRedoIsEnable(SVInst *_app) :  SVOpBase(_app){
    
}

SVOpPenRedoIsEnable::~SVOpPenRedoIsEnable(){
    
}

void SVOpPenRedoIsEnable::_process(f32 dt) {
    SVString result = "false";
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr) {
        SVPenDrawPtr t_penDraw = DYN_TO_SHAREPTR(SVPenDraw, t_modulePtr);
        if (t_penDraw) {
            if (t_penDraw->isRedoEnable()) {
                result = "true";
            }
        }
    }
    if (m_pCB) {
        (*m_pCB)(result, mApp);
    }
}

SVOpPenMode::SVOpPenMode(SVInst *_app, s32 _mode) :  SVOpBase(_app){
    m_mode = _mode;
}

SVOpPenMode::~SVOpPenMode(){
    
}

void SVOpPenMode::_process(f32 dt) {
    SVString result = "false";
    SVString t_name = "sv_pen_module";
    SVModuleBasePtr t_modulePtr = mApp->getModuleSys()->getModule(t_name.c_str());
    if (t_modulePtr) {
        SVPenDrawPtr t_penDraw = DYN_TO_SHAREPTR(SVPenDraw, t_modulePtr);
        if (t_penDraw) {
            t_penDraw->setPenMode(SVPENMODE(m_mode));
        }
    }
}
