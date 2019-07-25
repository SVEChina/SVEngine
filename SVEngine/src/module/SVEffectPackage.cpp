//
// SVEffectPackage.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVEffectPackage.h"
#include "../app/SVGlobalMgr.h"
#include "../core/SVSpine.h"
#include "../core/SVDeformImageMove.h"
#include "../base/SVLock.h"
#include "../base/SVPreDeclare.h"
#include "../node/SVNode.h"
#include "../node/SVScene.h"
#include "../node/SVSpineNode.h"
#include "../node/SVSpriteNode.h"
#include "../node/SVBitFontNode.h"
#include "../act/SVActTexAttachment.h"
#include "../act/SVActFollow.h"
#include "../act/SVActionMgr.h"
#include "../act/SVActionUnit.h"
#include "../event/SVEvent.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/filter/SVParseLUTFilter.h"
#include "../basesys/SVBasicSys.h"
#include "../basesys/SVPictureProcess.h"
#include "../basesys/filter/SVFilterLUT.h"
#include "../basesys/SVDeformMgr.h"
#include "SVEffectMusic.h"
void spinenode_callback(SVSpineNodePtr _node,void* _obj,s32 _status) {
    SVEffectUnit *t_unit = (SVEffectUnit*)(_obj);
    if(_status == 2) {
        t_unit->setEnd(true);
    }else if(_status == 3) {
        t_unit->setEnd(true);
    }
}

SVEffectUnit::SVEffectUnit(SVInst* _app):SVGBase(_app){
    m_end = false;
    m_personAct = nullptr;
}

SVEffectUnit::~SVEffectUnit(){
    m_node = nullptr;
    m_personAct = nullptr;
}

void SVEffectUnit::init(SVNodePtr _node){
    SVScenePtr t_scene = mApp->getSceneMgr()->getScene();
    if(_node && t_scene){
        m_end = true;
        m_node = _node;
        t_scene->addNode(m_node);
        //需要挂的人身上
        if (_node->getBindIndex() >= 0 && !m_personAct) {
            _attachToPeople(_node);
        }
        SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, m_node);
        if (t_spineNode) {
            cptr8 t_defAniName = t_spineNode->getCurAniName();
            t_spineNode->setSpineCallback(spinenode_callback, this);
            t_spineNode->play(t_defAniName);
            m_end = false;
        }
    }
}

void SVEffectUnit::updateAniEvent(SVAnimateEventPtr _event){
    SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, m_node);
    
}

void SVEffectUnit::_attachToPeople(SVNodePtr _node){
    //跟随人脸
    SVActFollowPersonPtr t_fllowPerson = MakeSharedPtr<SVActFollowPerson>(mApp, _node->getPersonID());
    t_fllowPerson->setFllowIndex(_node->getBindIndex());
    t_fllowPerson->setBindOffset(_node->getBindOffset().x, _node->getBindOffset().y, _node->getBindOffset().z);
    t_fllowPerson->setScale(_node->getScale().x, _node->getScale().y, _node->getScale().z);
    m_personAct = mApp->getActionMgr()->addAction(t_fllowPerson, _node);
    m_personAct->play();
}

void SVEffectUnit::destroy(){
    if (m_node) {
        m_node->removeFromParent();
    }
    if (m_personAct) {
        m_personAct->stop();
        m_personAct->removeFromActionMgr();
    }
}

SVNodePtr SVEffectUnit::getNode(){
    return m_node;
}

void SVEffectUnit::setEnd(bool _end){
    m_end = _end;
}

bool SVEffectUnit::isEnd(){
    return m_end;
}

SVEffectPackage::SVEffectPackage(SVInst* _app)
:SVModuleBase(_app){
    m_lock = MakeSharedPtr<SVLock>();
    m_aniState = EFFECT_ANI_WAIT;
}

SVEffectPackage::~SVEffectPackage(){
    m_lock = nullptr;
    m_music = nullptr;
}

void SVEffectPackage::init(){
    SVModuleBase::init();
    startListen();
}

void SVEffectPackage::destroy(){
    m_lock->lock();
    stopListen();
    for (s32 i = 0; i<m_attachmentPool.size(); i++) {
        SVActTexAttachmentPtr t_attachment = m_attachmentPool[i];
        t_attachment->destroy();
    }
    m_attachmentPool.destroy();
    for (s32 i = 0; i<m_effectUnitPool.size(); i++) {
        SVEffectUnitPtr t_unit = m_effectUnitPool[i];
        t_unit->destroy();
    }
    m_effectUnitPool.destroy();
    for (s32 i = 0; i<m_filterBasePool.size(); i++) {
        SVFilterBasePtr t_filterBase = m_filterBasePool[i];
        SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
        if( t_picproc) {
            t_picproc->clearFilter(t_filterBase);
        }
    }
    
    for (s32 i = 0; i<m_deformPool.size(); i++) {
        SVDeformImageMovePtr t_deform = m_deformPool[i];
        SVDeformMgrPtr t_deformMgr = mApp->getDeformMgr();
        t_deform->m_rule=1;
    }
    
    m_filterBasePool.destroy();
    m_deformPool.destroy();
    if (m_music) {
        SVArray<SVString>t_musics;
        m_music->getAllMusic(t_musics);
        for (s32 i = 0; i<t_musics.size(); i++) {
            SVString t_path = t_musics[i];
            if (m_cb) {
                SVString msg = SVString::format("sveffectmusic_unload_%s",t_path.c_str());
                (*m_cb)(msg.c_str(), mApp);
            }
        }
        
    }
    
    m_lock->unlock();
    SVModuleBase::destroy();
    if (m_cb) {
        SVString msg = SVString::format("effectpackageremovesuccess_%s",m_module_name.c_str());
        (*m_cb)(msg.c_str(), mApp);
    }
}

void SVEffectPackage::open(){
    SVModuleBase::open();
    m_aniState = EFFECT_ANI_BEGIN;
}

void SVEffectPackage::reset(){
    m_aniState = EFFECT_ANI_BEGIN;
    for (s32 i = 0; i < m_effectUnitPool.size(); i++) {
        SVEffectUnitPtr t_unit = m_effectUnitPool[i];
        t_unit->setEnd(false);
    }
}

void SVEffectPackage::update(f32 _dt) {
    SVModuleBase::update(_dt);
    bool end = true;
    if (m_aniState == EFFECT_ANI_BEGIN) {
        for (s32 i = 0; i < m_effectUnitPool.size(); i++) {
            SVEffectUnitPtr t_unit = m_effectUnitPool[i];
            if (!t_unit->isEnd()) {
                end = false;
                break;
            }
        }
    }
    if (end && m_aniState == EFFECT_ANI_BEGIN) {
        m_aniState = EFFECT_ANI_END;
        if (m_cb) {
            SVString msg = SVString::format("effectpackageend_%s",m_module_name.c_str());
            (*m_cb)(msg.c_str(), mApp);
            reset();
        }
    }
    
    for (s32 i = 0; i < m_attachmentPool.size(); i++) {
        SVActTexAttachmentPtr t_attachment = m_attachmentPool[i];
        t_attachment->update(_dt);
    }
}

bool SVEffectPackage::procEvent(SVEventPtr _event) {
    if (_event->eventType == EVN_T_EFFECT_MUSIC_LOAD) {
        SVEffectMusicEventPtr t_event = DYN_TO_SHAREPTR(SVEffectMusicEvent, _event);
        if (t_event) {
            if (m_cb) {
                SVString msg = SVString::format("sveffectmusic_load_%s",t_event->path.c_str());
                (*m_cb)(msg.c_str(), mApp);
            }
        }
    }else if (_event->eventType == EVN_T_EFFECT_MUSIC_PLAY) {
        SVEffectMusicEventPtr t_event = DYN_TO_SHAREPTR(SVEffectMusicEvent, _event);
        if (t_event) {
            if (m_cb) {
                SVString msg = SVString::format("sveffectmusic_play_%s",t_event->path.c_str());
                (*m_cb)(msg.c_str(), mApp);
            }
        }
    }else if(_event->eventType == EVN_T_ANIMATE){
        SVAnimateEventPtr t_event = DYN_TO_SHAREPTR(SVAnimateEvent, _event);
        if (t_event) {
            for (s32 i = 0; i<m_effectUnitPool.size(); i++) {
                SVEffectUnitPtr t_unit = m_effectUnitPool[i];
                t_unit->updateAniEvent(t_event);
            }
        }
    }
    return  true;
}

void SVEffectPackage::addEffectUnit(SVNodePtr _nodePtr){
    if (_nodePtr) {
        SVEffectUnitPtr effectUnit = MakeSharedPtr<SVEffectUnit>(mApp);
        effectUnit->init(_nodePtr);
        m_effectUnitPool.append(effectUnit);
    }
}

void SVEffectPackage::addAttachment(SVActTexAttachmentPtr _attachment){
    if (_attachment) {
        m_attachmentPool.append(_attachment);
    }
}

void SVEffectPackage::addFilter(SVFilterBasePtr _filter){
    SVPictureProcessPtr t_picproc = mApp->getBasicSys()->getPicProc();
    if(_filter && t_picproc){
        _filter->create();
        t_picproc->addFilter(_filter);
        t_picproc->openFilter(_filter);
        m_filterBasePool.append(_filter);
    }
}

void SVEffectPackage::addDefrom(SVDeformImageMovePtr _deform){
    SVDeformMgrPtr t_deformMrg = mApp->getDeformMgr();
    if(_deform && t_deformMrg){
        t_deformMrg->pushDeform(_deform);
        m_deformPool.append(_deform);
    }
}

SVActTexAttachmentPtr SVEffectPackage::getTexAttachment(s32 _channel){
    for (s32 i = 0; i < m_attachmentPool.size(); i++) {
        SVActTexAttachmentPtr t_attachment = m_attachmentPool[i];
        SVActTexAttachment::TEXATTACHSPARAM t_param = t_attachment->getParam();
        if (t_param.channel == _channel) {
            return t_attachment;
        }
    }
    return nullptr;
}

void SVEffectPackage::setEffectMusic(SVEffectMusicPtr _music){
    if (_music) {
        m_music = _music;
    }
}

SVNodePtr SVEffectPackage::getNode(cptr8 _name){
    for (s32 i = 0; i < m_effectUnitPool.size(); i++) {
        SVEffectUnitPtr t_unit = m_effectUnitPool[i];
        if (strcmp(_name, t_unit->getNode()->getname()) == 0) {
            return t_unit->getNode();
        }
    }
    return nullptr;
}


