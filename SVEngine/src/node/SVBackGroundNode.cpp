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
#include "../base/SVVec2.h"

SVBackGroundNode::SVBackGroundNode(SVInst *_app)
:SVSpriteNode(_app,720,1280){
    ntype = "SVDeformNode";
    m_rsType = RST_BACKGROUND;
    m_useTexType = E_TEX_END;
    m_texcoordX = 1.0f;
    m_texcoordY = -1.0f;
}

SVBackGroundNode::SVBackGroundNode(SVInst *_app,f32 _w,f32 _h)
:SVSpriteNode(_app,_w,_h){
    ntype = "SVDeformNode";
    m_rsType = RST_BACKGROUND;
    m_useTexType = E_TEX_END;
    m_texcoordX = 1.0f;
    m_texcoordY = -1.0f;
}

SVBackGroundNode::~SVBackGroundNode(){
    disableDeform();
}

void SVBackGroundNode::update(f32 _dt){
    if( m_pDeform ){
        SVNode::update(_dt);
        if (m_pRenderObj && m_pMesh) {
            if(m_pMtl){
                m_pMtl->setBlendEnable(true);
                m_pMtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                m_pMtl->setModelMatrix(m_absolutMat.get());
                m_pMtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
                m_pMtl->setTexture(0,m_useTexType);
                m_pMtl->update(_dt);
                m_pRenderObj->setMesh(m_pMesh);
                m_pRenderObj->setMtl(m_pMtl);
            }else{
                //创建新的材质
                SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
                t_mtl->setModelMatrix(m_absolutMat.get());
                t_mtl->setTexcoordFlip(m_texcoordX, m_texcoordY);
                t_mtl->setBlendEnable(true);
                t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                t_mtl->setTexture(0,m_useTexType);
                t_mtl->update(_dt);
                m_pRenderObj->setMesh(m_pMesh);
                m_pRenderObj->setMtl(t_mtl);
            }
        }
        m_pDeform->update(_dt);
    }else{
        SVSpriteNode::update(_dt);
    }
}

void SVBackGroundNode::render(){
    if(m_pDeform){
        if ( m_visible ){
            //形变算法
            m_pDeform->render();
            //绘制形变结果
            SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
            if (m_pRenderObj) {
                m_pRenderObj->pushCmd(t_rs, m_rsType, "SVBackGroundNode");
            }
        }
        SVNode::render();
    }else{
        SVSpriteNode::render();
    }
}

void SVBackGroundNode::setTexture(cptr8 _path) {
    SVSpriteNode::setTexture(_path);
    if( isDeform() ) {
        disableDeform();
        enableDeform(m_useTexType);
    }
}

void SVBackGroundNode::setTexture(SVTEXTYPE _textype) {
    SVSpriteNode::setTexture(_textype);
    //重新创建一下纹理
    if( isDeform() ) {
        disableDeform();
        enableDeform(m_useTexType);
    }
}

SVDeformImageMovePtr SVBackGroundNode::getDeform(){
    return m_pDeform;
}

bool SVBackGroundNode::enableDeform(SVTEXTYPE _textype) {
    SVTexturePtr t_tex = mApp->getRenderer()->getSVTex(_textype);
    if(t_tex){
        return false;
    }
    //
    if(m_inTexType == E_TEX_END) {
        if(m_pTex) {
            if(!m_pDeform){
                m_pDeform=MakeSharedPtr<SVDeformImageMove>(mApp);
            }
            SVTexturePtr m_texout =
            mApp->getRenderer()->createSVTex(_textype,
                                             m_pTex->getwidth(),
                                             m_pTex->getheight(),
                                             GL_RGBA);
            m_pDeform->init(m_pTex,m_texout);
            m_useTexType = _textype;
            return true;
        }
    }else{
        SVTexturePtr t_innerTex = mApp->getRenderer()->getSVTex(m_inTexType);
        if(t_innerTex){
            if(!m_pDeform){
                m_pDeform=MakeSharedPtr<SVDeformImageMove>(mApp);
            }
            SVTexturePtr m_texout =
            mApp->getRenderer()->createSVTex(_textype,
                                             t_innerTex->getwidth(),
                                             t_innerTex->getheight(),
                                             GL_RGBA);
            m_pDeform->init(t_innerTex,m_texout);
            m_useTexType = _textype;
            return true;
        }
    }
    return false;
}

void SVBackGroundNode::disableDeform() {
    if(m_pDeform){
        m_pDeform = nullptr;
        mApp->getRenderer()->destroySVTex(m_useTexType);
        m_useTexType = E_TEX_END;;
    }
}

bool SVBackGroundNode::isDeform() {
    if(m_pDeform){
        return true;
    }
    return false;
}

//序列化
void SVBackGroundNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
    RAPIDJSON_NAMESPACE::Value locationObj(RAPIDJSON_NAMESPACE::kObjectType);//创建一个Object类型的元素
    _toJsonData(_allocator, locationObj);
    //sprite的属性 文件名 宽 高 纹理类型
    locationObj.AddMember("spriteW", m_width, _allocator);
    locationObj.AddMember("spriteH", m_height, _allocator);
    locationObj.AddMember("texname", RAPIDJSON_NAMESPACE::StringRef(m_pTexName.c_str()), _allocator);
    locationObj.AddMember("textype", s32(m_inTexType), _allocator);
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
    
    if (item.HasMember("spriteW") && item["spriteW"].IsInt()) {
        m_width = item["spriteW"].GetInt();
    }
    if (item.HasMember("spriteH") && item["spriteH"].IsInt()) {
        m_height = item["spriteH"].GetInt();
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
        enableDeform(m_useTexType);
    }
}
