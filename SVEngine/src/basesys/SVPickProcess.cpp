//
// SVPickProcess.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPickProcess.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../event/SVEventMgr.h"
#include "../event/SVOpEvent.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVCameraMgr.h"
#include "../node/SVScene.h"
#include "../node/SVCameraNode.h"
#include "../ui/SVUIMgr.h"

//获取射线求教节点
SVVisitRayPick::SVVisitRayPick(FVec3& _rayStart,FVec3& _rayEnd)
:m_rayStart(_rayStart)
,m_rayEnd(_rayEnd){
}

SVVisitRayPick::~SVVisitRayPick() {
    m_nodearray.destroy();
}

bool SVVisitRayPick::visit(SVNodePtr _node) {
    if(_node->getvisible() && _node->getcanSelect() ){
        SVBoundBox t_box_sw = _node->getAABBSW();
        if( t_box_sw.getIntersectionValid(m_rayStart, m_rayEnd) ){
            m_nodearray.append(_node);
        }
    }
    return true;
}

SVNodePtr SVVisitRayPick::getCrossNode(FVec3& _campos){
    SVNodePtr t_node = nullptr;
    f32 t_dis = 1000000;
    for(s32 i=0;i<m_nodearray.size();i++){
        if (m_nodearray[i]->getcanSelect() == false) {
            continue;
        }
        FVec3 tt = _campos - m_nodearray[i]->getPosition();
        f32 t_cam_dis = tt.length();
        if( t_cam_dis<t_dis){
            t_dis = t_cam_dis;
            t_node = m_nodearray[i];
        }
    }
    return t_node;
}

//
SVPickProcess::SVPickProcess(SVInst *_app)
:SVProcess(_app) {
    m_enablePick = false;
    m_curPickNode = nullptr;
}

SVPickProcess::~SVPickProcess() {
    m_curPickNode = nullptr;
}

void SVPickProcess::enablePick() {
    m_enablePick = true;
}

void SVPickProcess::disablePick() {
    clear();
    m_enablePick = false;
}

void SVPickProcess::clear() {
    m_curPickNode = nullptr;
}

//
SVNodePtr SVPickProcess::getPickNode(){
    return m_curPickNode;
}

//获取射线
bool SVPickProcess::_getRay(SVCameraNodePtr _cam,s32 _sx,s32 _sy,FVec3& _rayStart,FVec3& _rayEnd){
    if( _cam){
        f32 m_screenW = mApp->m_pGlobalParam->m_inner_width;
        f32 m_screenH = mApp->m_pGlobalParam->m_inner_height;
        //投影坐标x(-1,1.0),y(-1,1.0)
        FVec4 t_proj_v_begin = FVec4((_sx/m_screenW - 0.5f)*2.0f,(_sy/m_screenH - 0.5f)*2.0f,-1.0f,1.0f);
        FVec4 t_proj_v_end = FVec4((_sx/m_screenW - 0.5f)*2.0f,(_sy/m_screenH - 0.5f)*2.0f,1.0f,1.0f);
        FMat4 t_vp_inv = inverse(_cam->getVPMatObj());
        FVec4 t_world_begin = t_vp_inv*t_proj_v_begin;
        FVec4 t_world_end = t_vp_inv*t_proj_v_end;
        t_world_begin = t_world_begin/t_world_begin.w;
        t_world_end = t_world_end/t_world_end.w;
        //射线(确认过的 没错了)
        _rayStart.x = t_world_begin.x;
        _rayStart.y = t_world_begin.y;
        _rayStart.z = t_world_begin.z;
        _rayEnd.x = t_world_end.x;
        _rayEnd.y = t_world_end.y;
        _rayEnd.z = t_world_end.z;
        return true;
    }
    return false;
}

bool SVPickProcess::_getRayMat(SVCameraNodePtr _cam,FMat4 _vpMat,s32 _sx,s32 _sy,FVec3& _rayStart,FVec3& _rayEnd){
    if( _cam){
        f32 m_screenW = mApp->m_pGlobalParam->m_inner_width;
        f32 m_screenH = mApp->m_pGlobalParam->m_inner_height;
        //投影坐标x(-1,1.0),y(-1,1.0)
        FVec4 t_proj_v_begin = FVec4((_sx/m_screenW - 0.5f)*2.0f,(_sy/m_screenH - 0.5f)*2.0f,-1.0f,1.0f);
        FVec4 t_proj_v_end = FVec4((_sx/m_screenW - 0.5f)*2.0f,(_sy/m_screenH - 0.5f)*2.0f,1.0f,1.0f);
        FMat4 t_vp_inv = inverse(_vpMat);
        FVec4 t_world_begin = t_vp_inv*t_proj_v_begin;
        FVec4 t_world_end = t_vp_inv*t_proj_v_end;
        t_world_begin = t_world_begin/t_world_begin.w;
        t_world_end = t_world_end/t_world_end.w;
        //射线(确认过的 没错了)
        _rayStart.x = t_world_begin.x;
        _rayStart.y = t_world_begin.y;
        _rayStart.z = t_world_begin.z;
        _rayEnd.x = t_world_end.x;
        _rayEnd.y = t_world_end.y;
        _rayEnd.z = t_world_end.z;
        return true;
    }
    return false;
}

//屏幕坐标(左上角为 0，0),(720，1280)
bool SVPickProcess::pickScene(SVCameraNodePtr _cam,s32 _sx,s32 _sy){
    SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
    if (_cam && t_sc) {
        FVec3 t_start,t_end;
        if( _getRayMat(_cam,_cam->getVPMatObj(),_sx,_sy,t_start,t_end) ){
            //射线求交
            SVVisitRayPickPtr t_visit = MakeSharedPtr<SVVisitRayPick>(t_start,t_end);
            t_sc->visit(t_visit);
            FVec3 t_pos = _cam->getPosition();
            SVNodePtr t_node = t_visit->getCrossNode(t_pos);
            if(t_node){
                _pick(t_node);
                return true;
            }else{
                SVPickGetNothingEventPtr t_event = MakeSharedPtr<SVPickGetNothingEvent>();
                t_event->m_px = _sx;
                t_event->m_py = _sy;
                mApp->getEventMgr()->pushEvent(t_event);
            }
        }
    }
    return false;
}

//相机和屏幕坐标(都是以左下角为（0，0）)
bool SVPickProcess::pickUI(s32 _sx,s32 _sy){
    SVCameraNodePtr t_ui_cam = mApp->getCameraMgr()->getUICamera();
    FVec3 t_start,t_end;
    t_start.set(_sx, _sy, -10000.0f);
    t_end.set(_sx, _sy, 10000.0f);
    //构建射线
    SVVisitRayPickPtr t_visit = MakeSharedPtr<SVVisitRayPick>(t_start,t_end);
    mApp->getUIMgr()->visit(t_visit);
//    FVec3 t_postion(0.0,0.0,0.0);
//    SVNodePtr t_node = t_visit->getCrossNode(t_postion);
//    if(t_node){
//        if(t_node->getRSType() == RST_UI){
//            _pick(t_node);
//            return true;
//        }
//    }else{
//        SVPickGetNothingEventPtr t_event = MakeSharedPtr<SVPickGetNothingEvent>();
//        t_event->m_px = _sx;
//        t_event->m_py = _sy;
//        mApp->getEventMgr()->pushEvent(t_event);
//    }
    return false;
}

//移动节点
void SVPickProcess::moveNode(SVCameraNodePtr _cam,s32 _sx,s32 _sy){
    if (_cam) {
        FVec3 t_start,t_end;
        if( m_curPickNode && _getRay(_cam,_sx,_sy,t_start,t_end) ){
//            //构建移动平面(这个平面可以绘制出来)
//            FVec3 t_pos = m_curPickNode->getPosition();
//            FVec3 t_nor = _cam->getDirection();
            //        plane3df t_plane(t_pos,t_nor);
            //        //获取与平面的交点
            //        FVec3 t_out_cross_pt;
            //        if( t_plane.getIntersectionWithLine(t_ray.start, t_ray.getVector().normalize(), t_out_cross_pt) ){
            //            m_curPickNode->setPosition(t_out_cross_pt);
            //        }
        }
    }
}

bool SVPickProcess::getCrossPoint(SVCameraNodePtr _cam,s32 _sx,s32 _sy,FVec3& _crosspt){
    FVec3 t_start,t_end;
    if( _getRay(_cam,_sx,_sy,t_start,t_end) ){
        //构建移动平面(这个平面可以绘制出来)
        FVec3 t_pos = t_start;
        FVec3 t_dir = t_end - t_start;
        FVec4 t_plane = FVec4(0.0f,0.0f,1.0f,0.0f);
        //
        s32 t_ret = rayPlaneIntersection(_crosspt,t_pos,t_dir,t_plane);
        if(t_ret>0)
            return true;
    }
    return false;
}

bool SVPickProcess::getCrossPointWithPlane(SVCameraNodePtr _cam,s32 _sx,s32 _sy,FVec3& _crosspt, FVec4& _plane){
    FVec3 t_start,t_end;
    if( _getRay(_cam,_sx,_sy,t_start,t_end) ){
        //构建移动平面(这个平面可以绘制出来)
        FVec3 t_pos = t_start;
        FVec3 t_dir = t_end - t_start;
        //
        s32 t_ret = rayPlaneIntersection(_crosspt,t_pos,t_dir,_plane);
        if(t_ret>0)
            return true;
    }
    return false;
}

bool SVPickProcess::getCrossPointUI(SVCameraNodePtr _cam,s32 _sx,s32 _sy,FVec3& _crosspt){
    FVec3 t_start,t_end;
    //error fyz
//    if(_cam){
//        if(_getRayMat(_cam,mApp->getCameraMgr()->getMainCamera()->getVPMatObjUI(),_sx,_sy,t_start,t_end)){
//            //构建移动平面(这个平面可以绘制出来)
//            FVec3 t_pos = t_start;
//            FVec3 t_dir = t_end - t_start;
//            FVec4 t_plane = FVec4(0.0f,0.0f,1.0f,0.0f);
//            //
//            s32 t_ret = rayPlaneIntersection(_crosspt,t_pos,t_dir,t_plane);
//            if(t_ret>0)
//                return true;
//        }
//    }
    return false;
}
                                                     

//
void SVPickProcess::_pick(SVNodePtr _node){
    if (_node) {
        if (m_curPickNode != _node) {
            if(m_curPickNode){
                m_curPickNode->setbeSelect(false);
                SVPickLoseEventPtr t_event = MakeSharedPtr<SVPickLoseEvent>(m_curPickNode);
                mApp->getEventMgr()->pushEvent(t_event);
            }
            m_curPickNode = _node;
            SVPickChangeEventPtr t_event = MakeSharedPtr<SVPickChangeEvent>(m_curPickNode,_node);
            mApp->getEventMgr()->pushEvent(t_event);
        }
        
        _node->setbeSelect(true);
        SVPickGetEventPtr t_event = MakeSharedPtr<SVPickGetEvent>(_node);
        mApp->getEventMgr()->pushEvent(t_event);
    }
}

bool SVPickProcess::procEvent(SVEventPtr _event){
    SVCameraNodePtr t_camera = mApp->m_pGlobalMgr->m_pCameraMgr->getMainCamera();
    if(_event->eventType == EVN_T_TOUCH_BEGIN){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        f32 t_mod_x = t_touch->x;
        f32 t_mod_y = t_touch->y;
        pickScene(t_camera,t_mod_x,t_mod_y);
    }else if(_event->eventType == EVN_T_TOUCH_END){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        f32 t_mod_x = t_touch->x;
        f32 t_mod_y = t_touch->y;
    }else if(_event->eventType == EVN_T_TOUCH_MOVE){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        f32 t_mod_x = t_touch->x;
        f32 t_mod_y = t_touch->y;
        moveNode(t_camera,t_mod_x,t_mod_y);
    }
    return true;
}

void SVPickProcess::transScreenPtToWorld(FVec2 &_screenPt, FVec3 &_worldPt){
    SVCameraNodePtr t_camera = mApp->getCameraMgr()->getMainCamera();
    FMat4 t_cameraMatrix = t_camera->getViewMatObj();
    FVec3 t_cameraEye = t_camera->getPosition();
    //构建虚拟平面
    FVec3 t_cameraDir = FVec3(-t_cameraMatrix[2],
                              -t_cameraMatrix[6],
                              -t_cameraMatrix[10]);
    t_cameraDir.normalize();
    FVec3 t_targetPos = t_cameraEye + t_cameraDir*0.0001f;
    t_targetPos.normalize();
    f32 t_dis = dot(t_targetPos,t_cameraDir);
    FVec4 t_plane = FVec4(-t_cameraDir,t_dis);
    //求交点
    FVec3 t_pos;
    f32 t_pt_x = _screenPt.x;
    f32 t_pt_y = _screenPt.y;
    f32 t_pt_z = 0.0f;
    if(getCrossPointWithPlane(t_camera,t_pt_x, t_pt_y,t_pos, t_plane) ){
        t_pt_x = t_pos.x;
        t_pt_y = t_pos.y;
        t_pt_z = t_pos.z;
    }
    _worldPt.set(t_pt_x, t_pt_y, t_pt_z);
}
