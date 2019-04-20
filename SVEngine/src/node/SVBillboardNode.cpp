//
// SVBillboardNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVBillboardNode.h"
#include "SVScene.h"
#include "SVCameraNode.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtlBillboard.h"
#include "../rendercore/SVRenderMesh.h"
#include "../core/SVGeoGen.h"
#include "../basesys/SVConfig.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
#include "../basesys/SVStaticData.h"
//
SVBillboardNode::SVBillboardNode(SVInst *_app)
:SVSpriteNode(_app) {
    ntype = "SVBillboardNode";
    m_inTexType = E_TEX_END;
    m_pTexPath = "default";
    m_rsType = RST_SOLID_3D;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_canSelect = false;
    m_pTex = nullptr;
    m_pMesh = nullptr;
    setTexcoord(1.0,-1.0);
    setSize(100,100);
    m_pMtl = MakeSharedPtr<SVMtlBillboard>(_app);
}

SVBillboardNode::SVBillboardNode(SVInst *_app,f32 _w,f32 _h)
:SVSpriteNode(_app) {
    ntype = "SVBillboardNode";
    m_inTexType = E_TEX_END;
    m_rsType = RST_SOLID_3D;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    m_canSelect = false;
    m_pTex = nullptr;
    m_pMesh = nullptr;
    setTexcoord(1.0,-1.0);
    setSize(_w,_h);
    m_pMtl = MakeSharedPtr<SVMtlBillboard>(_app);
}

SVBillboardNode::~SVBillboardNode() {
    m_pMesh = nullptr;
    m_pRenderObj = nullptr;
    m_pMtl = nullptr;
    m_pTex = nullptr;
}

//
void SVBillboardNode::setSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_pMesh = SVGeoGen::genRect(mApp, m_width, m_height, 0, 0, m_width, m_height,m_aabbBox);
}

void SVBillboardNode::syncTexSize() {
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

void SVBillboardNode::setTexcoord(f32 _x,f32 _y){
    m_texcoordX=_x;
    m_texcoordY=_y;
}

void SVBillboardNode::setTexture(cptr8 _path, bool enableMipMap){
    if(m_pTexPath!=_path) {
        m_pTexPath = _path;
        m_pTex = mApp->getTexMgr()->getTextureSync(m_pTexPath.c_str(),true, enableMipMap);
    }
}

cptr8 SVBillboardNode::getTexturePath(){
    if (m_pTex) {
        return m_pTex->getname();
    }
    return m_pTexPath.c_str();
}

void SVBillboardNode::setTexture(SVTEXTYPE _textype){
    m_inTexType = _textype;
}

void SVBillboardNode::setTexture(SVTexturePtr _tex){
    if (m_pTex) {
        m_pTex = nullptr;
    }
    m_pTex = _tex;
}

void SVBillboardNode::setMesh(SVRenderMeshPtr _mesh){
    if (_mesh) {
        m_pMesh = _mesh;
    }
}

void SVBillboardNode::update(f32 dt) {
    SVNode::update(dt);
    if (m_pRenderObj && m_pMesh) {
        if(m_pMtl){
            SVMtlBillboardPtr t_billboard = DYN_TO_SHAREPTR(SVMtlBillboard, m_pMtl);
            if (t_billboard) {
                t_billboard->setQuadPosW(getPosition());
            }
            m_pMtl->setBlendEnable(true);
            m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            m_pMtl->setModelMatrix(m_absolutMat.get());
            m_pMtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
            if(m_inTexType == E_TEX_END) {
                m_pMtl->setTexture(0,m_pTex);
            }else{
                SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_inTexType);
                m_pMtl->setTexture(0,t_tex);
            }
            m_pMtl->update(dt);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(m_pMtl);
        }else{
            //创建新的材质
            SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
            t_mtl->setModelMatrix(m_absolutMat.get());
            t_mtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
            t_mtl->setBlendEnable(true);
            t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            if(m_inTexType == E_TEX_END) {
                t_mtl->setTexture(0,m_pTex);
            }else{
                SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(m_inTexType);
                t_mtl->setTexture(0,t_tex);
            }
            t_mtl->update(dt);
            m_pRenderObj->setMesh(m_pMesh);
            m_pRenderObj->setMtl(t_mtl);
        }
    }
}

void SVBillboardNode::render() {
    if (mApp->m_pGlobalParam->m_curScene && m_visible ){
        SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
        if (m_pRenderObj) {
            m_pRenderObj->pushCmd(t_rs, m_rsType, "SVSpriteNode");
        }
    }
    SVNode::render();
}

//序列化
void SVBillboardNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
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

void SVBillboardNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
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
