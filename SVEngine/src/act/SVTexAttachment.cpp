//
// SVTexAttachment.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVTexAttachment.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtl2D.h"
#include "../node/SVSpineNode.h"
#include "../node/SVSpriteNode.h"
#include "../basesys/SVSceneMgr.h"
#include "../node/SVScene.h"
#include "../node/SVNodeVisit.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
SVTexAttachment::SVTexAttachment(SVInst *_app)
:SVGBase(_app) {
}

SVTexAttachment::~SVTexAttachment() {
    m_srcNode = nullptr;
    m_tarNode = nullptr;
}

void SVTexAttachment::init() {
}

void SVTexAttachment::destroy() {
}

void SVTexAttachment::enter(){
}

void SVTexAttachment::exit(){
}

void SVTexAttachment::update(f32 _dt) {
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
        SVMtl2DPtr mtl2D = std::dynamic_pointer_cast<SVMtl2D>(m_srcNode->getMaterial());
        if (mtl2D) {
            mtl2D->setAlpha(t_alpha);
        }
    }
}

bool SVTexAttachment::isEnd(){
    return true;
}

void SVTexAttachment::setAttachmentTex(s32 _channel, cptr8 _data, s32 _width, s32 _height){
    if (m_srcNode) {
        SVTexturePtr texture = mApp->getTexMgr()->createUnctrlTextureWithData(_width, _height, GL_RGBA, GL_RGBA, (void *)_data);
        m_srcNode->setTexture(texture);
        SVMtl2DPtr t_mtl = MakeSharedPtr<SVMtl2D>(mApp, "normal2d_mask_c");
        m_srcNode->setMaterial(t_mtl);
        SVTexturePtr textureMask = mApp->getTexMgr()->getTextureSync(m_param.maskTexPath.c_str(),true);
        t_mtl->setTexture(1, textureMask);
    }
}

void SVTexAttachment::setAttachmentParamFromJson(RAPIDJSON_NAMESPACE::Value &item){
    if (m_srcNode) {
        m_srcNode = nullptr;
    }
    if (m_tarNode) {
        m_tarNode = nullptr;
    }
    if (item.HasMember("channel") && item["channel"].IsInt()) {
        m_param.channel = item["channel"].GetInt();
    }
    if (item.HasMember("sourcenode") && item["sourcenode"].IsString()) {
        m_param.sourceNodeName = item["sourcenode"].GetString();
    }
    if (item.HasMember("targetnode") && item["targetnode"].IsString()) {
        m_param.targetNodeName = item["targetnode"].GetString();
    }
    if (item.HasMember("bonename") && item["bonename"].IsString()) {
        m_param.boneName = item["bonename"].GetString();
    }
    if (item.HasMember("slotname") && item["slotname"].IsString()) {
        m_param.slotName = item["slotname"].GetString();
    }
    if (item.HasMember("mask") && item["mask"].IsString()) {
        m_param.maskTexPath = m_rootPath + item["mask"].GetString();
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

SVTexAttachment::TEXATTACHSPARAM SVTexAttachment::getParam(){
    return m_param;
}
