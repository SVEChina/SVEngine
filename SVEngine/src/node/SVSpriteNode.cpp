//
// SVSpriteNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVSpriteNode.h"
#include "SVScene.h"
#include "SVCameraNode.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtl2D.h"
#include "../rendercore/SVRenderMesh.h"
#include "../core/SVGeoGen.h"
#include "../basesys/SVConfig.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderer.h"
#include "../basesys/SVStaticData.h"
//
SVSpriteNode::SVSpriteNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVSpriteNode";
    m_inTexType = E_TEX_END;
    m_pTexPath = "default";
    m_rsType = RST_SOLID_3D;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_canSelect = false;
    m_pTex = nullptr;
    m_pMesh = nullptr;
    setTexcoord(1.0,-1.0);
    setSize(100,100);
}

SVSpriteNode::SVSpriteNode(SVInst *_app,f32 _w,f32 _h)
:SVNode(_app) {
    ntype = "SVSpriteNode";
    m_inTexType = E_TEX_END;
    m_rsType = RST_SOLID_3D;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_canSelect = false;
    m_pTex = nullptr;
    m_pMesh = nullptr;
    setTexcoord(1.0,-1.0);
    setSize(_w,_h);
}

SVSpriteNode::~SVSpriteNode() {
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
    m_pTex = nullptr;
}

//
void SVSpriteNode::setSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_pMesh = SVGeoGen::genRect(mApp, m_width, m_height, 0, 0, m_width, m_height,m_aabbBox);
}

void SVSpriteNode::syncTexSize() {
    if(m_inTexType == E_TEX_END) {
        m_pTex = mApp->getTexMgr()->getTextureSync(m_pTexPath.c_str(),true);
    }else {
        m_pTex = mApp->getRenderer()->getSVTex(m_inTexType);
    }
    if(m_pTex) {
        s32 t_w = m_pTex->getwidth();
        s32 t_h = m_pTex->getheight();
        setSize(t_w,t_h);
    }
}

void SVSpriteNode::setTexcoord(f32 _x,f32 _y){
    m_texcoordX=_x;
    m_texcoordY=_y;
}

void SVSpriteNode::setMaterial(SVMtlCorePtr _mtl){
    m_pMtl = _mtl;
}

SVMtlCorePtr SVSpriteNode::getMaterial(){
    return m_pMtl;
}

f32 SVSpriteNode::getRelativeWidth(){
    return m_width;
}

f32 SVSpriteNode::getRelativeHeight(){
    return m_height;
}

f32 SVSpriteNode::getWidth(){
    f32 t_scaleX = 1.0f;
    SVNodePtr t_curNode = THIS_TO_SHAREPTR(SVSpriteNode);
    while (t_curNode) {
        t_scaleX = t_scaleX * t_curNode->getScale().x;
        if (t_curNode->getParent()) {
            t_curNode = t_curNode->getParent();
        } else {
            break;
        }
    }
    return m_width*t_scaleX;
}

f32 SVSpriteNode::getHeight(){
    f32 t_scaleY = 1.0f;
    SVNodePtr t_curNode = THIS_TO_SHAREPTR(SVSpriteNode);
    while (t_curNode) {
        t_scaleY = t_scaleY * t_curNode->getScale().y;
        if (t_curNode->getParent()) {
            t_curNode = t_curNode->getParent();
        } else {
            break;
        }
    }
    return m_height*t_scaleY;
}

void SVSpriteNode::setTexture(cptr8 _path, bool enableMipMap){
    if(m_pTexPath!=_path) {
        m_pTexPath = _path;
        m_pTex = mApp->getTexMgr()->getTextureSync(m_pTexPath.c_str(),true, enableMipMap);
    }
}

cptr8 SVSpriteNode::getTexturePath(){
    if (m_pTex) {
        return m_pTex->getname();
    }
    return m_pTexPath.c_str();
}

void SVSpriteNode::setTexture(SVTEXTYPE _textype){
    m_inTexType = _textype;
}

void SVSpriteNode::setTexture(SVTexturePtr _tex){
    if (m_pTex) {
        m_pTex = nullptr;
    }
    m_pTex = _tex;
}

void SVSpriteNode::setTexture(void * _data, s32 _w, s32 _h, bool enableMipMap){
    if (m_pTex) {
        m_pTex = nullptr;
    }
    m_pTex = mApp->getTexMgr()->createUnctrlTextureWithData(_w, _h, GL_RGBA, GL_RGBA, (void *)_data);;
}

void SVSpriteNode::setMesh(SVRenderMeshPtr _mesh){
    if (_mesh) {
        m_pMesh = _mesh;
    }
}

void SVSpriteNode::update(f32 dt) {
    SVNode::update(dt);
    if (m_pRenderObj && m_pMesh) {
        if(m_pMtl){
            m_pMtl->setBlendEnable(true);
            m_pMtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
            m_pMtl->setModelMatrix(m_absolutMat.get());
            m_pMtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
            if(m_inTexType == E_TEX_END) {
                m_pMtl->setTexture(0,m_pTex);
            }else{
                SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_inTexType);
                m_pMtl->setTexture(0,t_tex);
            }
            m_pMtl->update(dt);
            SVMtl2DPtr t_mtl2D = DYN_TO_SHAREPTR(SVMtl2D, m_pMtl);
            if (t_mtl2D) {
                t_mtl2D->setAlpha(m_alpha);
            }
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(m_pMtl);
        }else{
            //创建新的材质
            SVMtl2DPtr t_mtl = MakeSharedPtr<SVMtl2D>(mApp, "normal2d_c");
            t_mtl->setModelMatrix(m_absolutMat.get());
            t_mtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
            t_mtl->setBlendEnable(true);
            t_mtl->setBlendState(MTL_BLEND_ONE, MTL_BLEND_ONE_MINUS_SRC_ALPHA);
            t_mtl->setAlpha(1.0f);
            if(m_inTexType == E_TEX_END) {
                t_mtl->setTexture(0,m_pTex);
            }else{
                SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_inTexType);
                t_mtl->setTexture(0,t_tex);
            }
            t_mtl->update(dt);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(t_mtl);
            m_pMtl = t_mtl;
        }
    }
}

void SVSpriteNode::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVSpriteNode");
        }
    }
    SVNode::render();
}

//序列化
void SVSpriteNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    locationObj.AddMember("spriteW", m_width, _allocator);
    locationObj.AddMember("spriteH", m_height, _allocator);
    s32 pos = m_pTexPath.rfind('/');
    m_pTexName = SVString::substr(m_pTexPath.c_str(), pos+1);
    locationObj.AddMember("texture", RAPIDJSON_NAMESPACE::StringRef(m_pTexName.c_str()), _allocator);
    locationObj.AddMember("textype", s32(m_inTexType), _allocator);
    _objValue.AddMember("SVSpriteNode", locationObj, _allocator);
}

void SVSpriteNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    if (item.HasMember("spriteW") && item["spriteW"].IsFloat()) {
        m_width = item["spriteW"].GetFloat();
    }
    if (item.HasMember("spriteH") && item["spriteH"].IsFloat()) {
        m_height = item["spriteH"].GetFloat();
    }
    setSize(m_width, m_height);
    if (item.HasMember("texture") && item["texture"].IsString()) {
        SVString t_textureName = item["texture"].GetString();
        SVString t_texturePath = m_rootPath + t_textureName;
        setTexture(t_texturePath.c_str(), m_enableMipMap);
    }
    if (item.HasMember("textype") && item["textype"].IsInt()) {
        m_inTexType = SVTEXTYPE(item["textype"].GetInt());
    }
    m_dirty = true;
}
