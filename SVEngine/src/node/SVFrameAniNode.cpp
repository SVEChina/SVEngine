//
// SVFrameAniNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVFrameAniNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../event/SVEventMgr.h"
#include "../basesys/SVConfig.h"
#include "../core/SVModel.h"
#include "../core/SVAnimateSkin.h"

SVFrameAniNode::SVFrameAniNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVFrameAniNode";
    m_rsType = RST_SOLID_3D;
    m_canSelect = false;
    m_accTime = 0.0f;
    m_totalTime = 0.0f;
    m_pActTex = nullptr;
    m_pMesh = nullptr;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    setSize(100,100);
}

SVFrameAniNode::~SVFrameAniNode() {
    m_pActTex = nullptr;
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
}

//
void SVFrameAniNode::setSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_pMesh = SVGeoGen::genRect(mApp, m_width, m_height, 0, 0, m_width, m_height,m_aabbBox);
}

f32 SVFrameAniNode::getRelativeWidth(){
    return m_width;
}

f32 SVFrameAniNode::getRelativeHeight(){
    return m_height;
}

void SVFrameAniNode::update(f32 dt) {
    SVNode::update(dt);
    //时间更新
    m_accTime += dt;
    if(m_loop) {
        if(m_accTime>m_totalTime) {
            m_accTime -= m_totalTime;
        }
    }else{
        if(m_accTime>m_totalTime) {
            m_accTime = m_totalTime;
        }
    }
    //根据时间计算激活纹理
    m_pActTex = _selectTex(m_accTime);
    if(!m_pActTex) {
        //m_pActTex = ;
    }
    //更新材质
    if (m_pRenderObj && m_pMesh && m_pMtl) {
        m_pMtl->setBlendEnable(true);
        m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        m_pMtl->setModelMatrix(m_absolutMat.get());
        //m_pMtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
        m_pMtl->setTexture(0,m_pActTex);
        m_pMtl->update(dt);
        m_pRenderObj->setMesh(m_pMesh);
        m_pRenderObj->setMtl(m_pMtl);
    }
}

void SVFrameAniNode::render() {
    if (m_visible && m_pRenderObj ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene()
        m_pRenderObj->pushCmd(t_rs, m_rsType, "SVFrameAniNode");
    }
    SVNode::render();
}

FrameTex SVFrameAniNode::_selectTex(f32 _time) {
    //二分法查目标
    
    //FRAMEPOOL m_framePool;
    
    return nullptr;
}

//播放控制
void SVFrameAniNode::play() {
}

void SVFrameAniNode::pause() {
}

void SVFrameAniNode::stop() {
}

void SVFrameAniNode::pushFrame(cptr8 _texname) {
    
}

void SVFrameAniNode::clearFrame() {
    
}


//f32 SVSpriteNode::getWidth(){
//    f32 t_scaleX = 1.0f;
//    SVNodePtr t_curNode = THIS_TO_SHAREPTR(SVSpriteNode);
//    while (t_curNode) {
//        t_scaleX = t_scaleX * t_curNode->getScale().x;
//        if (t_curNode->getParent()) {
//            t_curNode = t_curNode->getParent();
//        } else {
//            break;
//        }
//    }
//    return m_width*t_scaleX;
//}
//
//f32 SVSpriteNode::getHeight(){
//    f32 t_scaleY = 1.0f;
//    SVNodePtr t_curNode = THIS_TO_SHAREPTR(SVSpriteNode);
//    while (t_curNode) {
//        t_scaleY = t_scaleY * t_curNode->getScale().y;
//        if (t_curNode->getParent()) {
//            t_curNode = t_curNode->getParent();
//        } else {
//            break;
//        }
//    }
//    return m_height*t_scaleY;
//}

//序列化
void SVFrameAniNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
//    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
//    _toJsonData(_allocator, locationObj);
//    locationObj.AddMember("spriteW", m_width, _allocator);
//    locationObj.AddMember("spriteH", m_height, _allocator);
//    s32 pos = m_pTexPath.rfind('/');
//    m_pTexName = SVString::substr(m_pTexPath.c_str(), pos+1);
//    locationObj.AddMember("texture", RAPIDJSON_NAMESPACE::StringRef(m_pTexName.c_str()), _allocator);
//    locationObj.AddMember("textype", s32(m_inTexType), _allocator);
//    _objValue.AddMember("SVSpriteNode", locationObj, _allocator);
}

void SVFrameAniNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
//    _fromJsonData(item);
//    if (item.HasMember("spriteW") && item["spriteW"].IsFloat()) {
//        m_width = item["spriteW"].GetFloat();
//    }
//    if (item.HasMember("spriteH") && item["spriteH"].IsFloat()) {
//        m_height = item["spriteH"].GetFloat();
//    }
//    setSize(m_width, m_height);
//    if (item.HasMember("texture") && item["texture"].IsString()) {
//        SVString t_textureName = item["texture"].GetString();
//        SVString t_texturePath = m_rootPath + t_textureName;
//        setTexture(t_texturePath.c_str(), m_enableMipMap);
//    }
//    if (item.HasMember("textype") && item["textype"].IsInt()) {
//        m_inTexType = SVTEXTYPE(item["textype"].GetInt());
//    }
//    m_dirty = true;
}
