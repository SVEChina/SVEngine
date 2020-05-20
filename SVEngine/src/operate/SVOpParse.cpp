//
// SVOpParse.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVOpParse.h"
#include "../work/SVThreadPool.h"
#include "../basesys/SVSceneMgr.h"
#include "../file/SVParseMain.h"
#include "../node/SVNodeVisit.h"
#include "../node/SVScene.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../app/SVInst.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../file/SVParseData.h"
#include "../file/SVLoaderPng.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/SVRenderMgr.h"
#include "../module/SVModuleBase.h"

SVOpParse::SVOpParse(SVInst *_app, s32 _resid, cptr8 _path )
: SVOpBase(_app) {
    resid = _resid;
    path = _path;
    restype = 0;
}

SVOpParse::SVOpParse(SVInst *_app, s32 _resid,  cptr8 _path,s32 _restype)
: SVOpBase(_app) {
    resid = _resid;
    path = _path;
    restype = _restype;
}


void SVOpParse::_process(f32 dt) {
//    mApp->getConfig()->loadEffectIsScreen=false;
//    //清理特效
//    SVOpFilterEftPtr t_op = MakeSharedPtr<SVOpFilterEft>(mApp, restype, resid);
//    mApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
//    //
//    SVParseMain t_parseMain(mApp);
//    t_parseMain.parseTexHeadStream(&tesHeadSet);
//    
//    if (hasHeadSet && tesHeadSet.size() > 0) {
//        t_parseMain.parseTexHeadStream(&tesHeadSet);
//    }
//    //
//    bool loadSuccess = t_parseMain.parse(path.c_str(), resid);
//    //
//    SVParseEventPtr t_event = MakeSharedPtr<SVParseEvent>();
//    t_event->isloadEffect = loadSuccess;
//    t_event->resid = resid;
//    mApp->getEventMgr()->pushEvent(t_event);
}

//
SVOperateDelEft::SVOperateDelEft(SVInst *_app, s32 _resid)
: SVOpBase(_app) {
    resid = _resid;
}

void SVOperateDelEft::_process(f32 dt) {
    SVScenePtr t_scene = mApp->getSceneMgr()->getScene();
    if(t_scene){
    }
}

//
SVOpSaveEft::SVOpSaveEft(SVInst *_app, s32 _resid, cptr8 _path)
: SVOpBase(_app) {
    resid = _resid;
    path = _path;
}

void SVOpSaveEft::_process(f32 dt) {
    SVVisitCollectResID visit(resid);
    SVScenePtr t_scene = mApp->getSceneMgr()->getScene();
}

SVOpFilterEft::SVOpFilterEft(SVInst *_app, s32 _resType, s32 _resId)
: SVOpBase(_app) {
    resType = _resType;
    resId = _resId;
}

void SVOpFilterEft::_process(f32 dt) {
//    //清理等待挂在到主线程中的任务
//    if (mApp->m_pGlobalMgr->m_pDiySys->detectResType(resType)) {
//        int t_oldresid = mApp->m_pGlobalMgr->m_pDiySys->setResType(resType, resId);
//        if (t_oldresid != -1) {
//            SVScene* t_scene = mApp->getSceneMgr()->getScene();
//            if(t_scene){
//                SVVisitCollectResID visit(resId);
//                t_scene->deep_visit(&visit);
//            }
//        }
//    }
}

//生成节点
SVOpGenerate::SVOpGenerate(SVInst *_app, SVParseDataPtr _parsedataPtr, s32 _resid,cptr8 _path)
: SVOpBase(_app) {
    mParseDataPtr = _parsedataPtr;
    mResid = _resid;
    mPath = _path;
}

void SVOpGenerate::_process(f32 dt) {
    if (mParseDataPtr) {
        //主线程生成节点
        mParseDataPtr->generate(mResid,mPath.c_str());
        mParseDataPtr->suspend();
    }
}

//加载纹理
SVOpTexLoad::SVOpTexLoad(SVInst *_app, SVTexturePtr _tex, cptr8 _fname, bool _enableMipMap)
 : SVOpBase(_app) {
    mTex = _tex;
    mFilename = _fname;
    m_enableMipMap = _enableMipMap;
}

SVOpTexLoad::~SVOpTexLoad() {
    mTex = nullptr;
}

void SVOpTexLoad::_process(f32 dt) {
    SV_LOG_INFO("load texture: %s \n",mFilename.c_str() );
    SVLoaderPng pngLoad(mApp);
    u8 *pTexData = nullptr;
    pngLoad.loadData(mFilename.c_str(), &pTexData);
    if (pTexData) {
        mTex->init(mFilename.c_str(), GL_TEXTURE_2D, pngLoad.m_iWidth, pngLoad.m_iHeight, pngLoad.mRenderFormat, pngLoad.mRenderFormat, m_enableMipMap);
        mTex->setTexData(pTexData, pngLoad.m_iDataLength);  //数据
        mApp->getRenderMgr()->pushRCmdCreate(mTex);
        free(pTexData);
    }
}
