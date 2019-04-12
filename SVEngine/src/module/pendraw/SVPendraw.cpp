//
// SVPendraw.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPendraw.h"
#include "SVPenStroke.h"
#include "../../event/SVOpEvent.h"
#include "../SVGameReady.h"
#include "../SVGameRun.h"
#include "../SVGameEnd.h"
#include "../../app/SVInst.h"
#include "../../base/SVDataSwap.h"
#include "../../basesys/SVCameraMgr.h"
#include "../../node/SVCameraNode.h"
#include "../../basesys/SVBasicSys.h"
#include "../../basesys/SVPickProcess.h"
SVPendraw::SVPendraw(SVInst *_app)
:SVGameBase(_app)
,m_curStroke(nullptr){
    
}

SVPendraw::~SVPendraw() {
    m_curStroke = nullptr;
}

void SVPendraw::init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end) {
    SVGameBase::init(_ready,_run,_end);
}

void SVPendraw::destroy() {
    SVGameBase::destroy();
}

void SVPendraw::update(f32 _dt) {
    SVGameBase::update(_dt);
    if (m_curStroke) {
        m_curStroke->update(_dt);
    }
}

void SVPendraw::open() {
    SVGameBase::open();
}

void SVPendraw::close() {
    SVGameBase::close();
}

bool SVPendraw::procEvent(SVEventPtr _event){
    if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_BEGIN){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (!m_curStroke) {
            m_curStroke = MakeSharedPtr<SVPenStroke>(mApp);
            SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
            FMat4 t_cameraMatrix = mainCamera->getViewMatObj();
            FVec4 t_plane = FVec4(t_cameraMatrix[2], t_cameraMatrix[6], t_cameraMatrix[10], 0.3);
            SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
            FVec3 t_pos;
            if(t_pickModule && t_pickModule->getCrossPointWithPlane(t_touch->x, t_touch->y,t_pos, t_plane) ){
                m_curStroke->begin(t_pos.x,t_pos.y,t_pos.z);
            }
        }
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_END){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (t_touch && m_curStroke) {
            SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
            FMat4 t_cameraMatrix = mainCamera->getViewMatObj();
            FVec4 t_plane = FVec4(t_cameraMatrix[2], t_cameraMatrix[6], t_cameraMatrix[10], 0.3);
            SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
            FVec3 t_pos;
            if(t_pickModule && t_pickModule->getCrossPointWithPlane(t_touch->x, t_touch->y,t_pos, t_plane) ){
                m_curStroke->end(t_pos.x,t_pos.y,t_pos.z);
            }
        }
//        m_curStroke = nullptr;
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_MOVE){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (t_touch && m_curStroke) {
            SVCameraNodePtr mainCamera = mApp->getCameraMgr()->getMainCamera();
            FMat4 t_cameraMatrix = mainCamera->getViewMatObj();
            FVec4 t_plane = FVec4(t_cameraMatrix[2], t_cameraMatrix[6], t_cameraMatrix[10], 0.3);
            SVPickProcessPtr t_pickModule = mApp->getBasicSys()->getPickModule();
            FVec3 t_pos;
            if(t_pickModule && t_pickModule->getCrossPointWithPlane(t_touch->x, t_touch->y,t_pos, t_plane) ){
                m_curStroke->draw(t_pos.x,t_pos.y,t_pos.z);
            }
        }
    }
    return true;
}


