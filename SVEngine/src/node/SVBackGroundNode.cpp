//
// SVBackGroundNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBackGroundNode.h"
#include "SVScene.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVOpEvent.h"
#include "../basesys/SVSceneMgr.h"
#include "../basesys/SVStaticData.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderObject.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "../core/SVGeoGen.h"
#include "../core/SVPass.h"
#include "../app/SVGlobalMgr.h"
#include "../mtl/SVMtlFaceShape.h"
#include "../mtl/SVMtlShapeVaried.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVTexMgr.h"
#include "../detect/SVDetectMgr.h"
#include "../detect/SVDetectST.h"
#include "../core/SVImageUsingMove.h"
#include "../core/SVDeformImageMove.h"
#include "../basesys/SVDeformMgr.h"
#include "../base/SVVec2.h"

SVBackGroundNode::SVBackGroundNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVBackGroundNode";
    m_pTexName = "";
    m_rsType = RST_SKY;
    m_useTexType = E_TEX_END;
    m_width = 720.0f;
    m_height = 1280.0f;
    m_texcoordX = 1.0f;
    m_texcoordY = -1.0f;
    m_inScreen = false;
    m_isSyncTex = true;
    setSize(m_width, m_height);
    m_pTex = nullptr;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    setInScreen(m_inScreen);
}

SVBackGroundNode::SVBackGroundNode(SVInst *_app,f32 _w,f32 _h)
:SVNode(_app){
    ntype = "SVBackGroundNode";
    m_pTexName = "";
    m_rsType = RST_SKY;
    m_useTexType = E_TEX_END;
    m_width = _w;
    m_height = _h;
    m_texcoordX = 1.0f;
    m_texcoordY = -1.0f;
    m_inScreen = false;
    m_isSyncTex = true;
    setSize(m_width, m_height);
    m_pTex = nullptr;
    m_pRenderObj = MakeSharedPtr<SVRenderObject>();
    setInScreen(m_inScreen);
}

SVBackGroundNode::~SVBackGroundNode(){
    disableDeform();
    m_pRenderObj = nullptr;
}

void SVBackGroundNode::setSize(f32 _w,f32 _h) {
    m_width = _w;
    m_height = _h;
    m_pMesh = SVGeoGen::genRect(mApp, m_width, m_height, 0, 0, m_width, m_height,m_aabbBox);
    m_isSyncTex = false;
}

void SVBackGroundNode::syncTexSize() {
    if(m_pTex) {
        setSize(m_pTex->getwidth(),m_pTex->getheight());
    }else{
        SVTexturePtr tmpTex = mApp->getRenderer()->getSVTex(m_useTexType);
        if(tmpTex){
            setSize(tmpTex->getwidth(),tmpTex->getheight());
        }
    }
    m_isSyncTex = true;
}

//屏幕空间下材质更新
void SVBackGroundNode::setInScreen(bool _inscreen) {
    m_inScreen = _inscreen;
    if(m_inScreen) {
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "screennor");
        m_pMtl->setBlendEnable(false);
        m_pMtl->setDepthEnable(false);
    }else{
        m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
        m_pMtl->setBlendEnable(false);
        m_pMtl->setDepthEnable(false);
    }
}

void SVBackGroundNode::setTexture(cptr8 _path) {
    if(m_pTexName!=_path) {
        m_pTexName = _path;
        m_pTex = mApp->getTexMgr()->getTextureSync(m_pTexName.c_str(),true);
        if(m_isSyncTex){
            syncTexSize();
        }
    }
    m_useTexType = E_TEX_END;
}

void SVBackGroundNode:: setTexture(SVTEXTYPE _textype) {
    m_useTexType = _textype;
    m_pTex = nullptr;
    if(m_isSyncTex){
        syncTexSize();
    }
}

SVDeformImageMovePtr SVBackGroundNode::getDeform(){
    return m_pDeform;
}

bool SVBackGroundNode::enableDeform() {
    SVTexturePtr t_innerTex = mApp->getRenderer()->getSVTex(E_TEX_MAIN);
    SVRendererBasePtr t_renderer =  mApp->getRenderer();
    t_renderer->createSVTex(E_TEX_FILTER_DEFORM01, t_innerTex->getwidth(), t_innerTex->getheight(), GL_RGBA);
    SVTexturePtr t_innerTexout = mApp->getRenderer()->getSVTex(E_TEX_FILTER_DEFORM01);
    
    if(!m_pDeform){
        m_pDeform=MakeSharedPtr<SVDeformImageMove>(mApp);
    }
    m_pDeform->init(t_innerTex,t_innerTexout);
    setTexture(E_TEX_FILTER_DEFORM01);
    return true;
}

void SVBackGroundNode::disableDeform() {
    if(m_pDeform){
        m_pDeform = nullptr;
    }
}

bool SVBackGroundNode::isDeform() {
    if(m_pDeform){
        return true;
    }
    return false;
}

void SVBackGroundNode::update(f32 _dt){
    
    if(m_pDeform){
        mApp->getDeformMgr()->pushDeform(m_pDeform);
    }
    
    if(m_inScreen) {
        //屏幕空间下更新
            m_pRenderObj->setMesh(mApp->getDataMgr()->m_screenMesh);
            if(m_pMtl){
                m_pMtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
                if(m_useTexType!=E_TEX_END){
                    m_pMtl->setTexture(0,m_useTexType);
                }else{
                    m_pMtl->setTexture(0,m_pTex);
                }
                m_pRenderObj->setMtl(m_pMtl->clone());
            }else{
                m_pRenderObj->setMtl(nullptr);
            }
    }else{
        //三维空间下更新
        SVNode::update(_dt);
        m_pRenderObj->setMesh(m_pMesh);
        if(m_pMtl){
            m_pMtl->setModelMatrix(m_absolutMat.get());
            m_pMtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
            if(m_useTexType!=E_TEX_END){
                m_pMtl->setTexture(0,m_useTexType);
            }else{
                m_pMtl->setTexture(0,m_pTex);
            }
            m_pMtl->update(_dt);
            m_pRenderObj->setMtl(m_pMtl->clone());
        }else{
            m_pRenderObj->setMtl(nullptr);
        }
    }
}

void SVBackGroundNode::render(){
    if ( !m_visible )
        return ;
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (m_pRenderObj) {
        m_pRenderObj->pushCmd(t_rs, m_rsType, "SVBackGroundNode");
    }
}

//序列化
void SVBackGroundNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    //sprite的属性 文件名 宽 高 纹理类型
    locationObj.AddMember("width", m_width, _allocator);
    locationObj.AddMember("height", m_height, _allocator);
    locationObj.AddMember("texname", RAPIDJSON_NAMESPACE::StringRef(m_pTexName.c_str()), _allocator);
    locationObj.AddMember("textype", s32(m_useTexType), _allocator);
    locationObj.AddMember("useTextype", s32(m_useTexType), _allocator);
    //是否开了形变算法
    bool t_hasDeform = isDeform();
    locationObj.AddMember("hasDeform", t_hasDeform, _allocator);
    if(t_hasDeform){
        m_pDeform->toJSON(_allocator, locationObj);
    }
    _objValue.AddMember("SVBackGroundNode", locationObj, _allocator);
}

void SVBackGroundNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    if (item.HasMember("width") && item["width"].IsInt()) {
        m_width = item["width"].GetInt();
    }
    if (item.HasMember("height") && item["height"].IsInt()) {
        m_height = item["height"].GetInt();
    }
    if (item.HasMember("textype") && item["textype"].IsInt()) {
       // m_inTexType = SVTEXTYPE(item["textype"].GetInt());
    }
    if (item.HasMember("texname") && item["texname"].IsString()) {
        m_pTexName = item["texname"].GetString();
    }
    if (item.HasMember("useTextype") && item["useTextype"].IsInt()) {
        m_useTexType = SVTEXTYPE(item["useTextype"].GetInt());
    }
    //
    bool t_hasDeform = false;
    if (item.HasMember("hasDeform") && item["hasDeform"].IsBool()) {
        t_hasDeform = item["hasDeform"].GetBool();
    }
    if(t_hasDeform){
        if(m_pDeform){
            disableDeform();
        }
        m_pDeform = MakeSharedPtr<SVDeformImageMove>(mApp);
        m_pDeform->fromJSON(item["sv_deform"]);
        enableDeform();
    }
}
