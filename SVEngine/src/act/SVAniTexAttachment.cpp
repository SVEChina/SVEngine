//
// SVAniTexAttachment.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVAniTexAttachment.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtl2D.h"
#include "../node/SVSpineNode.h"
#include "../node/SVSpriteNode.h"
#include "../basesys/SVSceneMgr.h"
#include "../node/SVScene.h"
#include "../node/SVNodeVisit.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
SVAniTexAttachment::SVAniTexAttachment(SVInst *_app)
:SVAniBase(_app) {
    m_srcNode = nullptr;
    m_tarNode = nullptr;
    m_texture = nullptr;
    m_param.channel = 9;
    m_texInfo.dataSwap = nullptr;
    m_texInfo.width = 1;
    m_texInfo.height = 1;
    m_dirty = false;
}

SVAniTexAttachment::~SVAniTexAttachment() {
    m_srcNode = nullptr;
    m_tarNode = nullptr;
    m_texture = nullptr;
    m_texInfo.dataSwap = nullptr;
    m_texInfo.width = 1;
    m_texInfo.height = 1;
}

void SVAniTexAttachment::init() {
}

void SVAniTexAttachment::destroy() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVTEXTYPE t_texType = SVTEXTYPE(E_TEX_AVATAR_0 + m_param.channel);
    if (t_renderer->hasSVTex(t_texType)) {
        t_renderer->destroySVTex(t_texType);
    }
}

void SVAniTexAttachment::enter(){
}

void SVAniTexAttachment::exit(){
}

void SVAniTexAttachment::update(f32 _dt) {
    if (m_dirty) {
        m_dirty = false;
        _genTexture();
        _findAttachmentNode();
        _refreshAttachmentNode();
    }
    if (m_tarNode && m_srcNode) {
        if (m_param.boneName.size() > 0) {
            f32 t_px;
            f32 t_py;
            m_tarNode->getBonePosition(t_px, t_py, m_param.boneName.c_str());
            m_srcNode->setPosition(t_px, t_py, 0);
            f32 t_sx;
            f32 t_sy;
            m_tarNode->getBoneScale(t_sx, t_sy, m_param.boneName.c_str());
            m_srcNode->setScale(t_sx, t_sy, 0);
        }
        if (m_param.slotName.size() > 0) {
            f32 t_alpha;
            t_alpha = m_tarNode->getSlotAlpha(m_param.slotName.c_str());
            m_srcNode->setAlpha(t_alpha);
        }
    }
}

bool SVAniTexAttachment::isEnd(){
    return true;
}

void SVAniTexAttachment::setAttachmentTex(void *_data, s32 _width, s32 _height){
    if (_data) {
        SVDataSwapPtr t_dataSwap = MakeSharedPtr<SVDataSwap>();
        t_dataSwap->writeData(_data, _width*_height*4);
        m_texInfo.dataSwap = t_dataSwap;
        m_texInfo.width = _width;
        m_texInfo.height = _height;
    }
    m_dirty = true;
}

void SVAniTexAttachment::setParam(TEXATTACHSPARAM &_param){
    m_param = _param;
    m_dirty = true;
}

void SVAniTexAttachment::setChannel(s32 _channel){
    m_param.channel = _channel;
    m_dirty = true;
}

void SVAniTexAttachment::setSrcNodeName(cptr8 _name){
    m_param.sourceNodeName = _name;
    m_dirty = true;
}

void SVAniTexAttachment::setTarNodeName(cptr8 _name){
    m_param.targetNodeName = _name;
    m_dirty = true;
}

void SVAniTexAttachment::setBoneName(cptr8 _name){
    m_param.boneName = _name;
    m_dirty = true;
}

void SVAniTexAttachment::setSlotName(cptr8 _name){
    m_param.slotName = _name;
    m_dirty = true;
}

void SVAniTexAttachment::setMataData(cptr8 _mata){
    m_param.matadata = _mata;
    m_dirty = true;
}

void SVAniTexAttachment::getParam(TEXATTACHSPARAM &_param){
    _param = m_param;
}

s32  SVAniTexAttachment::getChannel(){
    return m_param.channel;
}

cptr8 SVAniTexAttachment::getSrcNodeName(){
    return m_param.sourceNodeName.c_str();
}

cptr8 SVAniTexAttachment::getTarNodeName(){
    return m_param.targetNodeName.c_str();
}

cptr8 SVAniTexAttachment::getBoneName(){
    return m_param.boneName.c_str();
}

cptr8 SVAniTexAttachment::getSlotName(){
    return m_param.slotName.c_str();
}

cptr8 SVAniTexAttachment::getMataData(){
    return m_param.matadata.c_str();
}

void SVAniTexAttachment::_genTexture(){
    if (m_texInfo.dataSwap) {
        if (m_texture) {
            m_texture = nullptr;
        }
        SVRendererBasePtr t_renderer = mApp->getRenderer();
        SVTEXTYPE t_texType = SVTEXTYPE(E_TEX_AVATAR_0 + m_param.channel);
        if (t_renderer->hasSVTex(t_texType)) {
            m_texture = t_renderer->getSVTex(t_texType);
        }else{
            m_texture = t_renderer->createSVTex(t_texType, m_texInfo.width, m_texInfo.height, GL_RGBA,GL_RGBA);
        }
        m_texture->setTexData(m_texInfo.dataSwap->getData(), m_texInfo.dataSwap->getSize());
    }
}

void SVAniTexAttachment::_findAttachmentNode(){
    SVVisitorNodeNamePtr srcvisitor = MakeSharedPtr<SVVisitorNodeName>(m_param.sourceNodeName.c_str());
    mApp->getSceneMgr()->getScene()->visit(srcvisitor);
        if (srcvisitor->m_nodeSet.size() > 0) {
        SVNodePtr t_node = srcvisitor->m_nodeSet[0];
        SVSpriteNodePtr t_spriteNode = DYN_TO_SHAREPTR(SVSpriteNode, t_node);
        if (t_spriteNode) {
            m_srcNode = t_spriteNode;
        }
    }
    SVVisitorNodeNamePtr tarvisitor = MakeSharedPtr<SVVisitorNodeName>(m_param.targetNodeName.c_str());
    mApp->getSceneMgr()->getScene()->visit(tarvisitor);
    if (tarvisitor->m_nodeSet.size() > 0) {
        SVNodePtr t_node = tarvisitor->m_nodeSet[0];
        SVSpineNodePtr t_spineNode = DYN_TO_SHAREPTR(SVSpineNode, t_node);
        if (t_spineNode) {
            m_tarNode = t_spineNode;
        }
    }
}

void SVAniTexAttachment::_refreshAttachmentNode(){
    if (m_texture && m_srcNode) {
        f32 t_relH = m_srcNode->getRelativeHeight();
        f32 t_relW = m_srcNode->getRelativeWidth();
        f32 t_texW = m_texture->getwidth();
        f32 t_texH = m_texture->getheight();
        m_srcNode->setSize(t_texW*(t_relH*1.0/t_texH), t_texH);
        m_srcNode->setTexture(m_texture);
    }
}

void SVAniTexAttachment::fromJson(RAPIDJSON_NAMESPACE::Value &item){
    if (item.HasMember("channel") && item["channel"].IsInt()) {
        m_param.channel = item["channel"].GetInt();
    }
    if (item.HasMember("srcnode") && item["srcnode"].IsString()) {
        m_param.sourceNodeName = item["srcnode"].GetString();
    }
    if (item.HasMember("tarspine") && item["tarspine"].IsString()) {
        m_param.targetNodeName = item["tarspine"].GetString();
    }
    if (item.HasMember("bonename") && item["bonename"].IsString()) {
        m_param.boneName = item["bonename"].GetString();
    }
    if (item.HasMember("slotname") && item["slotname"].IsString()) {
        m_param.slotName = item["slotname"].GetString();
    }
    if (item.HasMember("metadata") && item["metadata"].IsString()) {
        m_param.matadata = item["metadata"].GetString();
    }
    m_dirty = true;
}

SVAniTexAttachment::TEXATTACHSPARAM SVAniTexAttachment::getParam(){
    return m_param;
}
