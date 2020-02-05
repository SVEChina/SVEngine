//
// SVARAnchor.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVARAnchor.h"
#include "../act/SVCameraCtrl.h"
#include "../base/SVPreDeclare.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPickProcess.h"
#include "../basesys/SVSensorProcess.h"
#include "../basesys/SVStaticData.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRendererBase.h"
#include "../node/SVCameraNode.h"
#include "../node/SVScene.h"
#include "../node/SVNode.h"
#include "../node/SV3DBox.h"
#include "../event/SVOpEvent.h"

SVARAnchor::SVARAnchor(SVInst* _app)
:SVModuleBase(_app){
    m_plane_dis = 0.3f;
    m_fbo = nullptr;
    m_pTex = nullptr;
    m_pRenderObj = nullptr;
    m_mtl = nullptr;
    m_pMesh = nullptr;
    t_testNode = nullptr;
    m_cb = nullptr;
}

SVARAnchor::~SVARAnchor(){
    m_fbo = nullptr;
    m_pTex = nullptr;
    m_pRenderObj = nullptr;
    m_mtl = nullptr;
    m_pMesh = nullptr;
    t_testNode = nullptr;
    m_cb = nullptr;
}

void SVARAnchor::init(){
    SVModuleBase::init();
    startListen();
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if (t_renderer) {
        SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_MAIN);
        s32 t_w = t_tex->getwidth();
        s32 t_h = t_tex->getheight();
        if (t_renderer->hasSVTex(E_TEX_HELP7)) {
            m_pTex = t_renderer->getSVTex(E_TEX_HELP7);
        }else{
            m_pTex = t_renderer->createSVTex(E_TEX_HELP7, t_w, t_h, GL_RGBA);
        }
    }
    m_fbo = MakeSharedPtr<SVRenderTexture>(mApp,m_pTex,true,true);
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo);
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_mtl = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl->setTexcoordFlip(1.0f, 1.0f);
    m_mtl->setTexture(0, E_TEX_HELP7);
    m_mtl->setBlendEnable(true);
    m_mtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
    m_pMesh = mApp->getDataMgr()->m_screenMesh;
}

void SVARAnchor::destroy(){
    SVModuleBase::destroy();
    stopListen();
}

void SVARAnchor::open(){
    SVModuleBase::open();
    
}

void SVARAnchor::close() {
    SVModuleBase::close();
    
}

bool SVARAnchor::isOpen(){
    return SVModuleBase::isOpen();
}

void SVARAnchor::update(f32 _dt) {
    SVModuleBase::update(_dt);
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (t_rs && t_renderer && m_fbo) {
        SVRenderCmdFboBindPtr t_fbo_bind = MakeSharedPtr<SVRenderCmdFboBind>(m_fbo);
        t_fbo_bind->mTag = "draw_aranchor_bind";
        t_rs->pushRenderCmd(RST_AR, t_fbo_bind);

        SVRenderCmdClearPtr t_clear = MakeSharedPtr<SVRenderCmdClear>();
        t_clear->mTag = "draw_aranchor_clear";
        t_clear->setRenderer(t_renderer);
        t_clear->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        t_rs->pushRenderCmd(RST_AR, t_clear);
        
        if (t_testNode) {
            t_testNode->update(_dt);
            t_testNode->render();
        }
        
        SVRenderCmdFboUnbindPtr t_fbo_unbind = MakeSharedPtr<SVRenderCmdFboUnbind>(m_fbo);
        t_fbo_unbind->mTag = "draw_aranchor_unbind";
        t_rs->pushRenderCmd(RST_AR, t_fbo_unbind);
        
//      再画回主纹理
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_mtl && m_pMesh && m_pRenderObj) {
            m_pRenderObj->setMtl(m_mtl);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->pushCmd(t_rs, RST_AR_END, "ARAnchorDrawReback");
        }
    }
    f32 t_distance = _getAnchorDis();
    if (*m_cb) {
        SVString t_distanceStr = SVString::format("%f",t_distance);
        (*m_cb)(t_distanceStr.c_str(), m_obj);
    }
}

void SVARAnchor::_addAnchor(f32 _x, f32 _y){
    FVec2 t_pt = FVec2(_x, _y);
    SVAnchorPoint t_worldPt;
    _screenPointToWorldAnchorPoint(t_pt, t_worldPt);
    _generateBox(t_worldPt);
}

void SVARAnchor::_generateBox(SVAnchorPoint &_worldPoint){
    SVScenePtr t_pScene = mApp->getSceneMgr()->getScene();
    if (t_pScene) {
        t_testNode = nullptr;
        t_testNode = MakeSharedPtr<SV3DBox>(mApp);
        t_testNode->setRSType(RST_AR);
        t_testNode->setdrawAABB(true);
        t_testNode->setScale(0.001, 0.001, 0.001);
        t_testNode->setPosition(_worldPoint.point.x, _worldPoint.point.y, _worldPoint.point.z);
    }
}

f32 SVARAnchor::_getAnchorDis(){
    SVSensorProcessPtr t_sensor = mApp->getBasicSys()->getSensorModule();
    SVCameraNodePtr t_arCam = t_sensor->getARCamera();
    if(!t_arCam)
        return 0.0f;
    
    if (!t_testNode) {
        return 0.0f;
    }
    FVec3 t_cameraPos = t_arCam->getPosition();
    FVec3 t_anchorPos =  t_testNode->getPosition();
    f32 distance = (t_cameraPos - t_anchorPos).length();
    return distance;
}

void SVARAnchor::_screenPointToWorldAnchorPoint(FVec2 &_point, SVAnchorPoint &_worldPoint){
    SVSensorProcessPtr t_sensor = mApp->getBasicSys()->getSensorModule();
    SVCameraNodePtr t_arCam = t_sensor->getARCamera();
    if(!t_arCam)
        return;
    FMat4 t_cameraMatrix = t_arCam->getViewMatObj();
    FVec3 t_cameraEye = t_arCam->getPosition();
    //构建虚拟平面
    FVec3 t_cameraDir = FVec3(-t_cameraMatrix[2],
                              -t_cameraMatrix[6],
                              -t_cameraMatrix[10]);
    t_cameraDir.normalize();
    FVec3 t_targetPos = t_cameraEye + t_cameraDir*m_plane_dis;
    f32 t_dis = dot(t_targetPos,t_cameraDir);
    FVec4 t_plane = FVec4(-t_cameraDir,t_dis);
    SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
    //求交点
    FVec3 t_pos;
    f32 t_pt_x = _point.x;
    f32 t_pt_y = _point.y;
    f32 t_pt_z = 0.0f;
    if(t_pickModule && t_pickModule->getCrossPointWithPlane(t_arCam,t_pt_x, t_pt_y,t_pos, t_plane) ){
        t_pt_x = t_pos.x;
        t_pt_y = t_pos.y;
        t_pt_z = t_pos.z;
    }
    //保存交点的位置和法线方向
    _worldPoint.point = FVec3(t_pt_x,t_pt_y,t_pt_z);
    _worldPoint.normal = -t_cameraDir;
    _worldPoint.ext0 = FVec3(0.0f,0.0f,0.0f);
    _worldPoint.ext1 = FVec3(0.0f,0.0f,0.0f);
}

bool SVARAnchor::procEvent(SVEventPtr _event) {
    if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_BEGIN){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        _addAnchor(t_touch->x, t_touch->y);
    }
    return true;
}


