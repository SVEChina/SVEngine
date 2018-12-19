//
// SVGLTFModelNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVGLTFModelNode.h"
#include "SVCameraNode.h"
#include "SVScene.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderObject.h"
#include "../event/SVEventMgr.h"
#include "../event/SVEvent.h"
#include "../event/SVOpEvent.h"
#include "../mtl/SVMtl3D.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/renderer/SVRendererBase.h"

//
SVGLTFModelNode::SVGLTFModelNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVGLTFModelNode";
    m_model = nullptr;
}

SVGLTFModelNode::~SVGLTFModelNode() {
    destroyModel();
    
}

void SVGLTFModelNode::setModel(GLTFModelPtr _model) {
    if (m_model == _model) {
        return;
    }
    if (m_model) {
        destroyModel();
    }
    m_model = _model;
    if(!m_pRObj){
        m_pRObj = MakeSharedPtr<SVMultMeshMtlRenderObject>();
    }
    m_pRObj->clearMesh();
}

GLTFModelPtr SVGLTFModelNode::getModel() {
    return m_model;
}

void SVGLTFModelNode::destroyModel() {
    if (m_model) {
        m_model = nullptr;
    }
}

void SVGLTFModelNode::update(f32 dt) {
    if( m_pRObj && m_model) {
        SVNode::update(dt);
        m_visible = true;
        m_drawBox = true;
        m_canSelect = true;
        //
        m_aabbBox.clear();
        m_pRObj->clearMesh();
        for (s32 i = 0; i<m_model->m_renderMeshData.size(); i++) {
            ModelMeshDataPtr meshData = m_model->m_renderMeshData[i];
            //计算包围盒
            m_aabbBox.expand(meshData->m_boundBox.getMin());
            m_aabbBox.expand(meshData->m_boundBox.getMax());
            //mesh
            SVRenderMeshPtr renderMesh = meshData->m_pMesh;
            renderMesh->setDrawMethod(E_DM_TRIANGLES);
            renderMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
            renderMesh->setIndexPoolType(GL_DYNAMIC_DRAW);
            renderMesh->setVertexType(E_VF_V3_N_C_T0);
            renderMesh->setIndexData(meshData->m_pRenderIndex, meshData->m_indexCount);
            renderMesh->setVertexData(meshData->m_pRenderVertex);
            renderMesh->setVertexDataNum(meshData->m_vertexCount);
            renderMesh->createMesh();
            //material
            SVMtl3DPtr t_mtl = meshData->m_pMtl;
            FMat4 matrix =  m_absolutMat * meshData->m_globalTransform;
            t_mtl->setModelMatrix(matrix.get());
            t_mtl->setDepthEnable(true);
            t_mtl->setBlendEnable(true);
            t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            m_pRObj->addRenderObj(renderMesh,t_mtl);
        }
    }else{
        m_visible = false;
    }
}

void SVGLTFModelNode::render() {
    if (!m_visible)
        return;
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (m_pRObj) {
        m_pRObj->pushCmd(t_rs, m_rsType, ntype);
    }
    SVNode::render();
}

/*
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
    locationObj.AddMember("spine", m_hasSpine, _allocator);
    _objValue.AddMember("SVSpineNode", locationObj, _allocator);
}

void SVSpineNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
    if (item.HasMember("aniname") && item["aniname"].IsString()) {
        m_cur_aniname = item["aniname"].GetString();
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
        SVSpinePtr t_spine = SVSpine::createSpine(mApp, m_rootPath + t_json, m_rootPath + t_atlas, 1.0f);
        if ( t_spine ) {
            s32 len = t_atlas.size();
            s32 pos = t_atlas.rfind('.');
            SVString t_spineName = SVString::substr(t_atlas.c_str(), 0, pos);
            t_spine->setSpineName(t_spineName.c_str());
            setSpine(t_spine);
        }
    }
}
 */
