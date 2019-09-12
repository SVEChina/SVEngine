//
// SVSpineNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <spine/Slot.h>
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>
#include "SVSpineNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderObject.h"
#include "../core/SVSpine.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectBase.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVOpEvent.h"
#include "../mtl/SVMtlAni2D.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/SVRendererBase.h"
//
SVSpineNode::SVSpineNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVSpineNode";
    m_spine = nullptr;
    m_pRObj = nullptr;
    m_spine_callback = nullptr;
    m_p_cb_obj = nullptr;
    m_visible = false;
    m_loop = true;
    m_immediatelyPlay = true;
    m_state = tANI_STATE_WAIT;
    m_rsType = RST_SOLID_3D;
    m_cur_aniname = "animation";
    m_canSelect = true;
    m_aabbBox_scale = 1.0f;
}

SVSpineNode::~SVSpineNode() {
    if(m_pRObj){
        m_pRObj->clearMesh();
        m_pRObj = nullptr;
    }
    clearSpine();
    m_spine_callback = nullptr;
    m_p_cb_obj = nullptr;
}

void SVSpineNode::setSpine(SVSpinePtr _spine) {
    if (m_spine == _spine) {
        return;
    }
    if (m_spine) {
        clearSpine();
    }
    m_spine = _spine;
    if(!m_pRObj){
        m_pRObj = MakeSharedPtr<SVMultMeshMtlRenderObject>();
    }
    m_pRObj->clearMesh();
    //回调函数
    m_spine->m_cb_startListener = [this](s32 itrackId) -> void {
        _spine_start();
    };
    //
    m_spine->m_cb_completeListener = [this](s32 itrackId, s32 iLoopCount) -> void {
        _spine_complete();
    };
    //
    m_spine->m_cb_endListener = [this](s32 itrackId) -> void {
        _spine_stop();
    };
}

SVSpinePtr SVSpineNode::getSpine() {
    return m_spine;
}

void SVSpineNode::clearSpine() {
    m_spine = nullptr;
}

void SVSpineNode::setstate(E_ANISTATE _state){
    m_state = _state;
}

E_ANISTATE SVSpineNode::getstate(){
    return m_state;
}

cptr8 SVSpineNode::getCurAniName(){
    return m_cur_aniname.c_str();
}

void SVSpineNode::setCurAniName(cptr8 _name){
    m_cur_aniname = _name;
}

void SVSpineNode::setloop(bool _loop){
    m_loop = _loop;
}

bool SVSpineNode::getloop(){
    return m_loop;
}

bool SVSpineNode::isImmediatelyPlay(){
    return m_immediatelyPlay;
}

void SVSpineNode::setSpineCallback(sv_spine_callback _cb,void* _obj) {
    m_spine_callback = _cb;
    m_p_cb_obj = _obj;
}

//
void SVSpineNode::update(f32 dt) {
    if (!m_visible)
        return;
    if (m_state == E_ANISTATE::tANI_STATE_STOP) {
        return;
    }
    if( m_pRObj && m_spine) {
        //spine更新
        m_spine->update(dt);
        _computeAABBBox();
        SVNode::update(dt);
        //更新模型
        m_pRObj->clearMesh();
        
        for (s32 i = 0, n = m_spine->m_pSkeleton->slotsCount; i < n; i++) {
            spSlot *t_slot = m_spine->m_pSkeleton->drawOrder[i];
            if (!t_slot->attachment) {
                continue;   //没有挂在项目
            }
            SpineMeshDataPtr pMeshData = m_spine->m_spineDataPool[i];
            SVMtlAni2DPtr t_mtl = MakeSharedPtr<SVMtlAni2D>(mApp);
            t_mtl->setModelMatrix(m_absolutMat.get());
            t_mtl->setTexture(0,pMeshData->m_pTex);
            t_mtl->setBlendEnable(true);
            t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            t_mtl->setBlendMode(SVMtlAni2D::SV_MTL_BLENDMODE_NORMAL);
            
            switch (pMeshData->m_blendMode) {
                case SP_BLEND_MODE_NORMAL:{
                    if(m_spine->m_preMultAlpha){
                        t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                    }else{
                        t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                    }
                    break;
                }
                case SP_BLEND_MODE_ADDITIVE:{
                    t_mtl->setBlendState(m_spine->m_preMultAlpha ? GL_ONE : GL_SRC_ALPHA, GL_ONE);
//                    t_mtl->setBlendState(GL_SRC_ALPHA, GL_ONE);
                    t_mtl->setBlendMode(SVMtlAni2D::SV_MTL_BLENDMODE_ADDITIVE);
                    break;
                }
                case SP_BLEND_MODE_MULTIPLY:{
                    t_mtl->setBlendState(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                    t_mtl->setBlendMode(SVMtlAni2D::SV_MTL_BLENDMODE_MULTIPLY);
                    break;
                }
                case SP_BLEND_MODE_SCREEN:{
                    t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
                    t_mtl->setBlendMode(SVMtlAni2D::SV_MTL_BLENDMODE_SCREEN);
                    break;
                }
                default:{
                    t_mtl->setBlendState(m_spine->m_preMultAlpha ? GL_ONE : GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                    break;
                }
            }
            m_pRObj->addRenderObj(pMeshData->m_pRenderMesh,t_mtl);
        }
    }
}

void SVSpineNode::render() {
    if (!m_visible)
        return;
    if (m_state == E_ANISTATE::tANI_STATE_STOP) {
        return;
    }
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (m_pRObj) {
        m_pRObj->pushCmd(t_rs, m_rsType, "SVSpineNode");
    }
    SVNode::render();
}

void SVSpineNode::play(cptr8 _actname) {
        if( m_state != E_ANISTATE::tANI_STATE_PLAY ){
        if (m_spine) {
            m_state = E_ANISTATE::tANI_STATE_PLAY;
            m_cur_aniname = _actname;
            m_spine->setToSetupPose();
            m_spine->setAnimationForTrack(0, _actname, m_loop);
            m_visible = true;
        }
    }
}

void SVSpineNode::addAni(cptr8 _actname){
    if (m_spine) {
        m_spine->addAnimationForTrack(0, _actname, m_loop,0);
    }
    m_visible = true;
}

void SVSpineNode::pause() {
    if( m_state != E_ANISTATE::tANI_STATE_PAUSE ){
        m_state = E_ANISTATE::tANI_STATE_PAUSE;
        _sendAniEvent("ani_pause");
    }
    
}

void SVSpineNode::stop() {
    if( m_state != E_ANISTATE::tANI_STATE_STOP ){
        m_state = E_ANISTATE::tANI_STATE_STOP;
        m_visible = false;
        if (m_spine) {
            m_spine->clearTrack(0);
        }
    }
}

//开始回调
void SVSpineNode::_spine_start() {
    _sendAniEvent("ani_play");
    //回调
    if( m_spine_callback ){
        (*m_spine_callback)(THIS_TO_SHAREPTR(SVSpineNode),m_p_cb_obj,1);
    }
}

//完成回调
void SVSpineNode::_spine_complete() {
    _sendAniEvent("ani_complete");
    //回调
    if( m_spine_callback ){
        (*m_spine_callback)(THIS_TO_SHAREPTR(SVSpineNode),m_p_cb_obj,2);
    }
}

//停止回调
void SVSpineNode::_spine_stop() {
    if (m_state == E_ANISTATE::tANI_STATE_STOP) {
        return;
    }
    m_visible = false;
    m_state = E_ANISTATE::tANI_STATE_STOP;
    _sendAniEvent("ani_stop");
    //回调
    if( m_spine_callback ){
        (*m_spine_callback)(THIS_TO_SHAREPTR(SVSpineNode),m_p_cb_obj,3);
    }
}

//发送事件
void SVSpineNode::_sendAniEvent(cptr8 _eventName) {
    SVString t_eventName = m_name + SVString("_") + SVString(_eventName);
    SVAnimateEventPtr t_event = MakeSharedPtr<SVAnimateEvent>();
    t_event->personID = m_personID;
    t_event->m_AnimateName = m_spine->getSpineName();
    t_event->eventName = t_eventName;
    mApp->getEventMgr()->pushEventToSecondPool(t_event);
}

void SVSpineNode::setAlpha(f32 _alpha){
    if (_alpha < 0.0f || _alpha > 1.0f) {
        return;
    }
    if(m_spine){
        m_spine->setAlpha(_alpha);
    }
}

bool SVSpineNode::getBonePosition(f32 &px, f32 &py, cptr8 bonename) {
    spBone *m_bone = m_spine->findBone(bonename);  //绑定的骨头
    if (m_bone) {
        px = m_bone->worldX*m_scale.x;
        py = m_bone->worldY*m_scale.y;
        //逐层找父节点，把本地坐标和世界坐标加上
        SVNodePtr t_curNode = THIS_TO_SHAREPTR(SVSpineNode);
        while (t_curNode) {
            px = t_curNode->getPosition().x + px;
            py = t_curNode->getPosition().y + py;
            
            if (t_curNode->getParent()) {
                t_curNode = t_curNode->getParent();
            } else {
                return true;
            }
        }
    }
    return false;
}

bool SVSpineNode::getBoneScale(f32 &sx, f32 &sy, cptr8 bonename){
    spBone *m_bone = m_spine->findBone(bonename);//spSkeleton_findBone(,bonename);           //绑定的骨头
    if (m_bone) {
        sx = m_bone->scaleX;
        sy = m_bone->scaleY;
        SVNodePtr t_curNode = THIS_TO_SHAREPTR(SVSpineNode);
        while (t_curNode) {
            sx = sx * t_curNode->getScale().x;
            sy = sy * t_curNode->getScale().y;
            
            if (t_curNode->getParent()) {
                t_curNode = t_curNode->getParent();
            } else {
                return true;
            }
        }
    }
    return false;
}

bool SVSpineNode::getBoneRotation(f32 &rotation, cptr8 bonename){
    spBone *m_bone = m_spine->findBone(bonename);//spSkeleton_findBone(,bonename);           //绑定的骨头
    if (m_bone) {
        rotation = m_bone->rotation;
        SVNodePtr t_curNode = THIS_TO_SHAREPTR(SVSpineNode);
        while (t_curNode) {
            rotation = rotation * t_curNode->getRotation().z;
            if (t_curNode->getParent()) {
                t_curNode = t_curNode->getParent();
            } else {
                return true;
            }
        }
    }
    return false;
}

f32 SVSpineNode::getSlotAlpha(cptr8 bonename) {
    spSlot *t_slot = m_spine->findSlot(bonename);
    f32 fAlpha = 1.0f;
    if (t_slot == NULL) {
        return fAlpha;
    }
    fAlpha = t_slot->color.a;
    return fAlpha;
}

void SVSpineNode::setAABBBoxScale(f32 _scale){
    m_aabbBox_scale = _scale;
}

void SVSpineNode::_computeAABBBox(){
    if (m_spine) {
        SVBoundBox t_boundingBox = m_spine->m_aabbBox;
        FVec3 t_min = t_boundingBox.getMin();
        FVec3 t_max = t_boundingBox.getMax();
        m_aabbBox = SVBoundBox(FVec3(t_min.x*m_aabbBox_scale, t_min.y*m_aabbBox_scale, 1.0), FVec3(t_max.x*m_aabbBox_scale, t_max.y*m_aabbBox_scale, 1.0));
    }
}

//序列化
void SVSpineNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    //
    locationObj.AddMember("aniname", RAPIDJSON_NAMESPACE::StringRef(m_cur_aniname.c_str()), _allocator);
    bool m_hasSpine = false;
    if(m_spine){
        //有spine
        m_hasSpine = true;
        locationObj.AddMember("ske_atlas", RAPIDJSON_NAMESPACE::StringRef(m_spine->m_spine_atlas.c_str()), _allocator);
        locationObj.AddMember("ske_json", RAPIDJSON_NAMESPACE::StringRef(m_spine->m_spine_json.c_str()), _allocator);
    }
    locationObj.AddMember("loop", m_loop, _allocator);
    locationObj.AddMember("play", m_immediatelyPlay, _allocator);
    locationObj.AddMember("spine", m_hasSpine, _allocator);
    _objValue.AddMember("SVSpineNode", locationObj, _allocator);
}

void SVSpineNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    if (item.HasMember("aniname") && item["aniname"].IsString()) {
        m_cur_aniname = item["aniname"].GetString();
    }
    
    if (item.HasMember("play") && item["play"].IsBool()) {
        m_immediatelyPlay = item["play"].GetBool();
    }
    
    if (item.HasMember("loop") && item["loop"].IsBool()) {
        m_loop = item["loop"].GetBool();
    }
    bool m_hasSpine = false;
    if (item.HasMember("spine") && item["spine"].IsBool()) {
        m_hasSpine = item["spine"].GetBool();
    }
    if(m_hasSpine){
        //有spine
        SVString t_atlas;
        SVString t_json;
        if (item.HasMember("ske_atlas") && item["ske_atlas"].IsString()) {
            t_atlas = item["ske_atlas"].GetString();
        }
        if (item.HasMember("ske_json") && item["ske_json"].IsString()) {
            t_json = item["ske_json"].GetString();
        }
        SVSpinePtr t_spine = SVSpine::createSpine(mApp, m_rootPath + t_json, m_rootPath + t_atlas, 1.0f, m_enableMipMap);
        if ( t_spine ) {
            s32 len = t_atlas.size();
            s32 pos = t_atlas.rfind('.');
            SVString t_spineName = SVString::substr(t_atlas.c_str(), 0, pos);
            t_spine->setSpineName(t_spineName.c_str());
            setSpine(t_spine);
        }
        //从特效包里加载的spine动画，写死了走这个渲染流！！
        m_rsType = RST_SOLID_3D;
    }
}
