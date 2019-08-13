//
// SVModelNode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVModelNode.h"
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
#include "../rendercore/SVRendererBase.h"
#include "../core/SVModel.h"
#include "../core/SVMesh.h"

//
SVModelNode::SVModelNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVModelNode";
    m_enableDebugNormal = false;
    m_pModel = nullptr;
}

SVModelNode::~SVModelNode() {
    m_pModel = nullptr;
}

void SVModelNode::update(f32 dt) {
    SVNode::update(dt);
    if(m_pModel) {
        m_pModel->update(dt,m_absolutMat,nullptr);
        //更新包围盒
        m_aabbBox = m_pModel->getBox();
    }
}

void SVModelNode::render() {
    if (!m_visible)
        return;
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
    if(m_pModel) {
        m_pModel->render();
    }
    SVNode::render();
}

void SVModelNode::setModel(SVModelPtr _model) {
    m_pModel = _model;
    if(m_pModel) {
        m_aabbBox = m_pModel->getBox();
    }
}

SVModelPtr SVModelNode::getModel() {
    return m_pModel;
}
/*
//序列化
void SVModelNode::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
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

void SVModelNode::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
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

void SVModelNode::_showDebugNormalLines(){
//    for (s32 i = 0; i<m_model->m_renderDebugMeshData.size(); i++) {
//        ModelRenderDataPtr debugRenderData = m_model->m_renderDebugMeshData[i];
//        //mesh
//        SVRenderMeshPtr renderMesh = debugRenderData->m_pMesh;
//        renderMesh->setDrawMethod(E_DM_LINES);
//        renderMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
////        renderMesh->setIndexPoolType(GL_DYNAMIC_DRAW);
//        renderMesh->setVertexType(E_VF_V3_C);
////        renderMesh->setIndexData(debugRenderData->m_pRenderIndex, debugRenderData->m_indexCount);
//        renderMesh->setVertexData(debugRenderData->m_pRenderVertex);
//        renderMesh->setVertexDataNum(debugRenderData->m_vertexCount);
//        renderMesh->createMesh();
//        //material
//        SVMtlCorePtr t_mtl = DYN_TO_SHAREPTR(SVMtlCore, debugRenderData->m_pMtl);
//        FMat4 matrix =  m_absolutMat * debugRenderData->m_globalTransform;
//        t_mtl->setModelMatrix(matrix.get());
//        t_mtl->setDepthEnable(true);
//        t_mtl->setBlendEnable(true);
//        t_mtl->setBlendState(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
//        m_pRObj->addRenderObj(renderMesh,t_mtl);
//    }
}

void SVModelNode::enableDebugNormal(bool _enable){
    m_enableDebugNormal = _enable;
}

bool SVModelNode::getDebugNormalEnable(){
    return m_enableDebugNormal;
}
