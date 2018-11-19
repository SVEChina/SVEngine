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
#include "../base/SVQuat.h"
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
    m_isFirstBoundingBox = true;
    m_renderMeshData.clear();
    m_boundingBox.clear();
    for (s32 i = 0; i<m_model->meshes.size(); i++) {
        Mesh mesh = m_model->meshes[i];
        for (s32 j = 0; j<mesh.primitives.size(); j++) {
            ModelMeshDataPtr modelMesh = MakeSharedPtr<ModelMeshData>();
            modelMesh->m_boundBox.clear();
            m_renderMeshData.append(modelMesh);
            Primitive meshPrimitive = mesh.primitives[j];
            Accessor indicesAccessor = m_model->accessors[meshPrimitive.indices];
            BufferView bufferView = m_model->bufferViews[indicesAccessor.bufferView];
            Buffer buffer = m_model->buffers[bufferView.buffer];
            //texture
            s32 materialID = meshPrimitive.material;
            Material material = m_model->materials[materialID];
            ParameterMap values = material.values;
            ParameterMap::Iterator itValue = values.find("baseColorTexture");
            if( itValue!=values.end() ) {
                Parameter parameter = itValue->data;
                s32 textureIndex = parameter.TextureIndex();
                Texture texture = m_model->textures[textureIndex];
                Image image = m_model->images[texture.source];
                modelMesh->m_pTex = image.texture;
            }
            // index
            s32 byteStride = indicesAccessor.ByteStride(bufferView);
            s64 count = indicesAccessor.count;
            const u8 *dataAddress = (u8 *)buffer.data->getData() + bufferView.byteOffset +indicesAccessor.byteOffset;
            switch (indicesAccessor.componentType) {
                case SVGLTF_COMPONENT_TYPE_BYTE:{
                    c8 *data = (c8 *)dataAddress;
                    //...
                    break;
                }case SVGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:{
                    u8 *data = (u8 *)dataAddress;
                    //...
                    break;
                }case SVGLTF_COMPONENT_TYPE_SHORT:{
                    s16 *data = (s16 *)dataAddress;
                    SVArray<u16> t_indices;
                    for (s32 pt = 0; pt<count; pt ++) {
                        u16 t_index = (u16)data[pt];
                        t_indices.append(t_index);
                    }
                    SVDataSwapPtr indicesData = MakeSharedPtr<SVDataSwap>();
                    s32 t_len = (s32) (sizeof(u16) * t_indices.size());
                    indicesData->writeData(t_indices.get(), t_len);
                    modelMesh->m_indexCount = count;
                    modelMesh->m_pRenderIndex = indicesData;
                    break;
                }case SVGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:{
                    u16 *data = (u16 *)dataAddress;
                    SVArray<u16> t_indices;
                    for (s32 pt = 0; pt<count; pt ++) {
                        u16 t_index = (u16)data[pt];
                        t_indices.append(t_index);
                    }
                    SVDataSwapPtr indicesData = MakeSharedPtr<SVDataSwap>();
                    s32 t_len = (s32) (sizeof(u16) * t_indices.size());
                    indicesData->writeData(t_indices.get(), t_len);
                    modelMesh->m_indexCount = count;
                    modelMesh->m_pRenderIndex = indicesData;
                    break;
                }case SVGLTF_COMPONENT_TYPE_INT:{
                    s32 *data = (s32 *)dataAddress;
                    SVArray<u16> t_indices;
                    for (s32 pt = 0; pt<count; pt ++) {
                        u16 t_index = (u16)data[pt];
                        t_indices.append(t_index);
                    }
                    SVDataSwapPtr indicesData = MakeSharedPtr<SVDataSwap>();
                    s32 t_len = (s32) (sizeof(u16) * t_indices.size());
                    indicesData->writeData(t_indices.get(), t_len);
                    modelMesh->m_indexCount = count;
                    modelMesh->m_pRenderIndex = indicesData;
                    break;
                }case SVGLTF_COMPONENT_TYPE_UNSIGNED_INT:{
                    u32 *data = (u32 *)dataAddress;
                    SVArray<u16> t_indices;
                    for (s32 pt = 0; pt<count; pt ++) {
                        u16 t_index = (u16)data[pt];
                        t_indices.append(t_index);
                    }
                    SVDataSwapPtr indicesData = MakeSharedPtr<SVDataSwap>();
                    s32 t_len = (s32) (sizeof(u16) * t_indices.size());
                    indicesData->writeData(t_indices.get(), t_len);
                    modelMesh->m_indexCount = count;
                    modelMesh->m_pRenderIndex = indicesData;
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
                            modelMesh->m_boundBox.expand(minV);
                            FVec3 maxV;
                            maxV.x = attribAccessor.maxValues[0];
                            maxV.y = attribAccessor.maxValues[1];
                            maxV.z = attribAccessor.maxValues[2];
                            modelMesh->m_boundBox.expand(maxV);
                            
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
//                                                t_p.x = (t_p.x + 1)*0.5;
//                                                t_p.x = t_p.x*0.5;
                                                t_p.y = data[pt + 1];
//                                                t_p.y = (t_p.y + 1)*0.5;
//                                                t_p.y = t_p.y*0.5;
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
                    SVArray<V3_T0> renderVertexData;
                    for (s32 vi = 0; vi <t_postions.size(); vi++) {
                        V3_T0 vertexPt;
                        vertexPt.x = t_postions[vi].x;
                        vertexPt.y = t_postions[vi].y;
                        vertexPt.z = t_postions[vi].z;
//                        vertexPt.nx = t_normals[vi].x;
//                        vertexPt.ny = t_normals[vi].y;
//                        vertexPt.nz = t_normals[vi].z;
                        if (vi < t_texcoord0s.size()) {
                            vertexPt.t0x = t_texcoord0s[vi].x;
                            vertexPt.t0y = t_texcoord0s[vi].y;
                        }
                        renderVertexData.append(vertexPt);
                    }
                    SVDataSwapPtr vertexData = MakeSharedPtr<SVDataSwap>();
                    s32 t_len = (s32) (sizeof(V3_T0) * renderVertexData.size());
                    vertexData->writeData(renderVertexData.get(), t_len);
                    modelMesh->m_vertexCount = renderVertexData.size();
                    modelMesh->m_pRenderVertex = vertexData;
                    break;
                }
             }
        }
    }
    _refreshModelMatrix();
}

void SVGLTFModelNode::_refreshModelMatrix(){
    if (!m_model) {
        return;
    }
    m_readyRenderMeshData.clear();
    SVArray<s32> sceneNodes = m_model->scenes[m_model->defaultScene].nodes;
    for (s32 i = 0; i<sceneNodes.size(); i++) {
        s32 nodeIndex = sceneNodes[i];
        Node node = m_model->nodes[nodeIndex];
        FMat4 mat;
        mat.setIdentity();
        _refreshMeshGlobalMat(node, mat);
    }
}

void SVGLTFModelNode::_refreshMeshGlobalMat(Node _node, FMat4 _mat4){
    FMat4 localTransform;
    localTransform.setIdentity();
    //translate
    FMat4 matT;
    matT.setIdentity();
    if (_node.translation.size() > 0) {
        matT.setTranslate(FVec3(_node.translation[0], _node.translation[1], _node.translation[2]));
    }

    //rotation
    FMat4 matR;
    matR.setIdentity();
    if (_node.rotation.size() > 0) {
        matR.set(SVQuat(FVec4(_node.rotation[0], _node.rotation[1], _node.rotation[2], _node.rotation[3])));
    }
    //scale
    FMat4 matS;
    matS.setIdentity();
    if (_node.scale.size() > 0) {
        matS.setScale(FVec3(_node.scale[0], _node.scale[1], _node.scale[2]));
    }
    localTransform = matT * matR * matS;

    //matrix
    if (_node.matrix.size() > 0) {
        FVec4 col0(_node.matrix[0], _node.matrix[1], _node.matrix[2], _node.matrix[3]);
        FVec4 col1(_node.matrix[4], _node.matrix[5], _node.matrix[6], _node.matrix[7]);
        FVec4 col2(_node.matrix[8], _node.matrix[9], _node.matrix[10], _node.matrix[11]);
        FVec4 col3(_node.matrix[12], _node.matrix[13], _node.matrix[14], _node.matrix[15]);
        localTransform.setColumn(0, col0);
        localTransform.setColumn(1, col1);
        localTransform.setColumn(2, col2);
        localTransform.setColumn(3, col3);
    }
    
    FMat4 mat = _mat4 * localTransform;
    if (_node.mesh >= 0) {
        ModelMeshDataPtr meshData = m_renderMeshData[_node.mesh];
        meshData->m_globalTransform = mat;
        meshData->m_boundBox.setTransform(mat);
//        m_readyRenderMeshData.append(meshData);
    }
    
    for (s32 i = 0; i<_node.children.size(); i++) {
        s32 childIndex = _node.children[i];
        Node node = m_model->nodes[childIndex];
        _refreshMeshGlobalMat(node, mat);
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
        //更新模型
//        _refreshModelMatrix();
        m_pRObj->clearMesh();
        for (s32 i = 0; i<m_renderMeshData.size(); i++) {
            ModelMeshDataPtr meshData = m_renderMeshData[i];
            //计算包围盒
            m_aabbBox.expand(meshData->m_boundBox.getMin());
            m_aabbBox.expand(meshData->m_boundBox.getMax());
            //mesh
            SVRenderMeshPtr renderMesh = MakeSharedPtr<SVRenderMesh>(mApp);
            renderMesh->setDrawMethod(E_DM_TRIANGLES);
            renderMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
            renderMesh->setIndexPoolType(GL_DYNAMIC_DRAW);
            renderMesh->setVertexType(E_VF_V3_T0);
            renderMesh->setIndexData(meshData->m_pRenderIndex, meshData->m_indexCount);
            renderMesh->setVertexData(meshData->m_pRenderVertex);
            renderMesh->setVertexDataNum(meshData->m_vertexCount);
            renderMesh->createMesh();
            //material
            SVMtl3DPtr t_mtl = MakeSharedPtr<SVMtl3D>(mApp);
            FMat4 matrix =  m_absolutMat * meshData->m_globalTransform;
            t_mtl->setModelMatrix(matrix.get());
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