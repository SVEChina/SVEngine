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
}

SVAniTexAttachment::~SVAniTexAttachment() {
    m_srcNode = nullptr;
    m_tarNode = nullptr;
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
    if (m_tarNode && m_srcNode) {
        f32 t_px;
        f32 t_py;
        m_tarNode->getBonePosition(t_px, t_py, m_param.boneName.c_str());
        m_srcNode->setPosition(t_px, t_py, 0);
        f32 t_sx;
        f32 t_sy;
        m_tarNode->getBoneScale(t_sx, t_sy, m_param.boneName.c_str());
        m_srcNode->setScale(t_sx, t_sy, 0);
        f32 t_alpha;
        t_alpha = m_tarNode->getSlotAlpha(m_param.slotName.c_str());
        m_srcNode->setAlpha(t_alpha);
    }
}

bool SVAniTexAttachment::isEnd(){
    return true;
}

void SVAniTexAttachment::setAttachmentTex(void *_data, s32 _width, s32 _height){
    if (m_srcNode) {
        SVRendererBasePtr t_renderer = mApp->getRenderer();
        SVTEXTYPE t_texType = SVTEXTYPE(E_TEX_AVATAR_0 + m_param.channel);
        SVTexturePtr t_texture = nullptr;
        if (t_renderer->hasSVTex(t_texType)) {
            t_texture = t_renderer->getSVTex(t_texType);
        }else{
            t_texture = t_renderer->createSVTex(t_texType, _width, _height, GL_RGBA,GL_RGBA);
        }
        t_texture->setTexData(_data, _width*_height*4);
        f32 t_relH = m_srcNode->getRelativeHeight();
        f32 t_relW = m_srcNode->getRelativeWidth();
        m_srcNode->setSize(_width*(t_relH*1.0/_height), t_relH);
        m_srcNode->setTexture(t_texture);
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
        m_param.metadata = item["metadata"].GetString();
    }
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

SVAniTexAttachment::TEXATTACHSPARAM SVAniTexAttachment::getParam(){
    return m_param;
}
