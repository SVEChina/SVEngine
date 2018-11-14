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
    _loadData();

}

GLTFModelPtr SVGLTFModelNode::getModel() {
    return m_model;
}

void SVGLTFModelNode::destroyModel() {
    if (m_model) {
        m_model = nullptr;
    }
    m_renderMeshData.destroy();
}

void SVGLTFModelNode::_loadData(){
    if (!m_model) {
        return;
    }
    m_renderMeshData.clear();
    for (s32 i = 0; i<m_model->meshes.size(); i++) {
        Mesh mesh = m_model->meshes[i];
        for (s32 j = 0; j<mesh.primitives.size(); j++) {
            ModelMeshDataPtr modelMesh = MakeSharedPtr<ModelMeshData>();
            m_renderMeshData.append(modelMesh);
            Primitive meshPrimitive = mesh.primitives[j];
            Accessor indicesAccessor = m_model->accessors[meshPrimitive.indices];
            BufferView bufferView = m_model->bufferViews[indicesAccessor.bufferView];
            Buffer buffer = m_model->buffers[bufferView.buffer];
            //texture
            s32 materialID = meshPrimitive.material;
            Material material = m_model->materials[materialID];
            ParameterMap additionalValues = material.additionalValues;
            ParameterMap::Iterator itValue = additionalValues.find("baseColorTexture");
            if( itValue!=additionalValues.end() ) {
                Parameter parameter = itValue->data;
                s32 textureIndex = parameter.TextureIndex();
                Texture texture = m_model->textures[textureIndex];
                Image image = m_model->images[texture.source];
                modelMesh->m_pTex = image.texture;
            }
            // index
            s32 byteStride = indicesAccessor.ByteStride(bufferView);
            s64 count = indicesAccessor.count;
            SVDataSwapPtr indicesData = MakeSharedPtr<SVDataSwap>();
            modelMesh->m_indexCount = count;
            modelMesh->m_pRenderIndex = indicesData;
            switch (indicesAccessor.componentType) {
                case SVGLTF_COMPONENT_TYPE_BYTE:{
                    c8 *dataAddress = (c8 *)buffer.data->getData() + bufferView.byteOffset +indicesAccessor.byteOffset;
                    indicesData->writeData(dataAddress, count*byteStride);
                    break;
                }case SVGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:{
                    u8 *dataAddress = (u8 *)buffer.data->getData() + bufferView.byteOffset +indicesAccessor.byteOffset;
                    indicesData->writeData(dataAddress, count*byteStride);
                    break;
                }case SVGLTF_COMPONENT_TYPE_SHORT:{
                    s16 *dataAddress = (s16 *)buffer.data->getData() + bufferView.byteOffset +indicesAccessor.byteOffset;
                    indicesData->writeData(dataAddress, count*byteStride);
                    break;
                }case SVGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:{
                    u16 *dataAddress = (u16 *)buffer.data->getData() + bufferView.byteOffset +indicesAccessor.byteOffset;
                    indicesData->writeData(dataAddress, count*byteStride);
                    break;
                }case SVGLTF_COMPONENT_TYPE_INT:{
                    s32 *dataAddress = (s32 *)buffer.data->getData() + bufferView.byteOffset +indicesAccessor.byteOffset;
                    indicesData->writeData(dataAddress, count*byteStride);
                    break;
                }case SVGLTF_COMPONENT_TYPE_UNSIGNED_INT:{
                    u32 *dataAddress = (u32 *)buffer.data->getData() + bufferView.byteOffset +indicesAccessor.byteOffset;
                    indicesData->writeData(dataAddress, count*byteStride);
                    break;
                }
                default:
                    break;
            }
            switch (meshPrimitive.mode) {
                case SVGLTF_MODE_TRIANGLE_FAN:
                   //will to do
                    break;
                case SVGLTF_MODE_TRIANGLE_STRIP:
                    //will to do
                    break;
                case SVGLTF_MODE_TRIANGLES:  // this is the simpliest case to handle
                {
                    SVArray<FVec3> t_postions;
                    SVArray<FVec3> t_normals;
                    SVArray<FVec2> t_texcoord0s;
                    SVArray<FVec2> t_texcoord1s;
                    SVMap<SVString, s32>::Iterator primitiveIt = meshPrimitive.attributes.begin();
                    while ( primitiveIt!=meshPrimitive.attributes.end() ) {
                        const s32 attributID = primitiveIt->data;
                        Accessor attribAccessor = m_model->accessors[attributID];
                        BufferView bufferView = m_model->bufferViews[attribAccessor.bufferView];
                        Buffer buffer = m_model->buffers[bufferView.buffer];
                        const s32 byte_stride = attribAccessor.ByteStride(bufferView);
                        const u64 count = attribAccessor.count;
                        const u8 *dataPtr = (u8 *)buffer.data->getData() + bufferView.byteOffset + attribAccessor.byteOffset;
                        //positon
                        if ( strcmp(primitiveIt->key, "POSITION") == 0 ) {
                            // get the position min/max for computing the boundingbox
                            FVec3 minV;
                            minV.x = attribAccessor.minValues[0];
                            minV.y = attribAccessor.minValues[1];
                            minV.z = attribAccessor.minValues[2];
                            FVec3 maxV;
                            maxV.x = attribAccessor.maxValues[0];
                            maxV.y = attribAccessor.maxValues[1];
                            maxV.z = attribAccessor.maxValues[2];
                            m_aabbBox.set(minV, maxV);
                            switch (attribAccessor.type) {
                                case SVGLTF_TYPE_VEC3: {
                                    switch (attribAccessor.componentType) {
                                        case SVGLTF_COMPONENT_TYPE_FLOAT:{
                                            f32 *data = (f32 *)dataPtr;
                                            for (s32 pt = 0; pt<count*3; pt += 3) {
                                                FVec3 t_p;
                                                t_p.x = data[pt + 0];
                                                t_p.y = data[pt + 1];
                                                t_p.z = data[pt + 2];
                                                t_postions.append(t_p);
                                            }
                                            break;
                                        }
                                        case SVGLTF_COMPONENT_TYPE_DOUBLE:{
                                            f64 *data = (f64 *)dataPtr;
                                            for (s32 pt = 0; pt<count*3; pt += 3) {
                                                FVec3 t_p;
                                                t_p.x = data[pt + 0];
                                                t_p.y = data[pt + 1];
                                                t_p.z = data[pt + 2];
                                                t_postions.append(t_p);
                                            }
                                            break;
                                        }
                                        default:
                                            break;
                                        
                                    }
                                    break;
                                }
                                default:
                                    
                                    break;
                            }
                        }
                        //normal
                        if (strcmp(primitiveIt->key, "NORMAL") == 0){
                            switch (attribAccessor.type) {
                                case SVGLTF_TYPE_VEC3: {
                                    switch (attribAccessor.componentType) {
                                        case SVGLTF_COMPONENT_TYPE_FLOAT: {
                                            f32 *data = (f32 *)dataPtr;
                                            for (s32 pt = 0; pt<count*3; pt += 3) {
                                                FVec3 t_p;
                                                t_p.x = data[pt + 0];
                                                t_p.y = data[pt + 1];
                                                t_p.z = data[pt + 2];
                                                t_normals.append(t_p);
                                            }
                                            break;
                                        }
                                        case SVGLTF_COMPONENT_TYPE_DOUBLE: {
                                          //will to do
                                            break;
                                        }
                                        default:
                                            break;
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                        //texcoord0
                        if (strcmp(primitiveIt->key, "TEXCOORD_0") == 0) {
                            switch (attribAccessor.type) {
                                case SVGLTF_TYPE_VEC2: {
                                    switch (attribAccessor.componentType) {
                                        case SVGLTF_COMPONENT_TYPE_FLOAT: {
                                            f32 *data = (f32 *)dataPtr;
                                            for (s32 pt = 0; pt<count*2; pt += 2) {
                                                FVec2 t_p;
                                                t_p.x = data[pt + 0];
                                                t_p.y = data[pt + 1];
                                                t_texcoord0s.append(t_p);
                                            }
                                            break;
                                        }
                                        case SVGLTF_COMPONENT_TYPE_DOUBLE: {
                                            //will to do
                                            break;
                                        }
                                        default:
                                            break;
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                        //texcoord1
                        if (strcmp(primitiveIt->key, "TEXCOORD_1") == 0) {
                            switch (attribAccessor.type) {
                                case SVGLTF_TYPE_VEC2: {
                                    switch (attribAccessor.componentType) {
                                        case SVGLTF_COMPONENT_TYPE_FLOAT: {
                                            f32 *data = (f32 *)dataPtr;
                                            for (s32 pt = 0; pt<count*2; pt += 2) {
                                                FVec2 t_p;
                                                t_p.x = data[pt + 0];
                                                t_p.y = data[pt + 1];
                                                t_texcoord1s.append(t_p);
                                            }
                                            break;
                                        }
                                        case SVGLTF_COMPONENT_TYPE_DOUBLE: {
                                            //will to do
                                            break;
                                        }
                                        default:
                                            break;
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                        primitiveIt++;
                    }
                    SVArray<V3_N_T0_T1> renderVertexData;
                    for (s32 vi = 0; vi <t_postions.size(); vi++) {
                        V3_N_T0_T1 vertexPt;
                        vertexPt.x = t_postions[i].x;
                        vertexPt.y = t_postions[i].y;
                        vertexPt.z = t_postions[i].z;
                        vertexPt.nx = t_normals[i].x;
                        vertexPt.ny = t_normals[i].y;
                        vertexPt.nz = t_normals[i].z;
                        vertexPt.t0x = t_texcoord0s[i].x;
                        vertexPt.t0y = t_texcoord0s[i].y;
                        if (t_texcoord1s.size() > 0) {
                            //有第二张纹理的时候需要
                            vertexPt.t1x = t_texcoord1s[i].x;
                            vertexPt.t1y = t_texcoord1s[i].y;
                        }
                        renderVertexData.append(vertexPt);
                    }
                    SVDataSwapPtr vertexData = MakeSharedPtr<SVDataSwap>();
                    s32 t_len = (s32) (sizeof(V3_N_T0_T1) * renderVertexData.size());
                    vertexData->writeData(renderVertexData.get(), t_len);
                    modelMesh->m_vertexCount = t_postions.size();
                    modelMesh->m_pRenderVertex = vertexData;
                    break;
                }
             }
        }
    }
}

void SVGLTFModelNode::update(f32 dt) {
    if( m_pRObj && m_model) {
        SVNode::update(dt);
        m_visible = true;
        //更新模型
        m_pRObj->clearMesh();
        for (s32 i = 0; i<m_renderMeshData.size(); i++) {
            ModelMeshDataPtr meshData = m_renderMeshData[i];
            SVRenderMeshPtr renderMesh = MakeSharedPtr<SVRenderMesh>(mApp);
            renderMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
            renderMesh->setIndexPoolType(GL_DYNAMIC_DRAW);
            renderMesh->setVertexType(E_VF_V3_T0_T1);
            renderMesh->setIndexData(meshData->m_pRenderIndex, meshData->m_indexCount);
            renderMesh->setVertexData(meshData->m_pRenderVertex);
            renderMesh->setVertexDataNum(meshData->m_vertexCount);
            renderMesh->createMesh();
            SVMtl3DPtr t_mtl = MakeSharedPtr<SVMtl3D>(mApp);
            t_mtl->setModelMatrix(m_absolutMat.get());
            t_mtl->setTexture(0,meshData->m_pTex);
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
        m_pRObj->pushCmd(t_rs, m_rsType, "SVGLTFModelNode");
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
