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
#include "../mtl/SVMtlAni2D.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../basesys/SVConfig.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include <typeinfo.h>
//
SVGLTFModelNode::SVGLTFModelNode(SVInst *_app)
:SVNode(_app) {
    ntype = "SVGLTFModelNode";
    m_model = nullptr;
}

SVGLTFModelNode::~SVGLTFModelNode() {
    destroyModel();
    
}

void SVGLTFModelNode::setModel(Model *_model) {
    if (m_model == _model) {
        return;
    }
    if (m_model) {
        destroyModel();
        
    }
    m_model = _model;
//    if(!m_pRObj){
//        m_pRObj = MakeSharedPtr<SVMultMeshMtlRenderObject>();
//    }
//    m_pRObj->clearMesh();
//    //回调函数
//    m_spine->m_cb_startListener = [this](s32 itrackId) -> void {
//        _spine_start();
//    };
//    //
//    m_spine->m_cb_completeListener = [this](s32 itrackId, s32 iLoopCount) -> void {
//        _spine_complete();
//    };
//    //
//    m_spine->m_cb_endListener = [this](s32 itrackId) -> void {
//        _spine_stop();
//    };
}

Model* SVGLTFModelNode::getModel() {
    return m_model;
}

void SVGLTFModelNode::destroyModel() {
    if (m_model) {
        delete m_model;
        m_model = nullptr;
    }
}

void SVGLTFModelNode::_loadData(){
    if (!m_model) {
        return;
    }
    for (s32 i = 0; i<m_model->meshes.size(); i++) {
        Mesh mesh = m_model->meshes[i];
        
        for (s32 j = 0; j<mesh.primitives.size(); j++) {
            Primitive meshPrimitive = mesh.primitives[i];
            
            Accessor indicesAccessor = m_model->accessors[meshPrimitive.indices];
            
            BufferView bufferView = m_model->bufferViews[indicesAccessor.bufferView];
            
            Buffer buffer = m_model->buffers[bufferView.buffer];
            
            s32 byteStride = indicesAccessor.ByteStride(bufferView);
            
            s64 count = indicesAccessor.count;
            
//            std::type_info& t0 = typeid(c8);
            // index
            switch (indicesAccessor.componentType) {
                case SVGLTF_COMPONENT_TYPE_BYTE:{
                    c8 *dataAddress = (c8 *)buffer.data->getData() + bufferView.byteOffset +indicesAccessor.byteOffset;
//                    addressTypeName = typeid(dataAddress).name();
//                    indicesArrayPtr =
//                    std::unique_ptr<intArray<char> >(new intArray<char>(
//                                                                        arrayAdapter<char>(dataAddress, count, byteStride)));
                    break;
                    
                }case SVGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
//                    indicesArrayPtr = std::unique_ptr<intArray<unsigned char> >(
//                                                                                new intArray<unsigned char>(arrayAdapter<unsigned char>(
//                                                                                                                                        dataAddress, count, byteStride)));
                    break;
                    
                case SVGLTF_COMPONENT_TYPE_SHORT:
//                    indicesArrayPtr =
//                    std::unique_ptr<intArray<short> >(new intArray<short>(
//                                                                          arrayAdapter<short>(dataAddress, count, byteStride)));
                    break;
                    
                case SVGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
//                    indicesArrayPtr = std::unique_ptr<intArray<unsigned short> >(
//                                                                                 new intArray<unsigned short>(arrayAdapter<unsigned short>(
//                                                                                                                                           dataAddress, count, byteStride)));
                    break;
                    
                case SVGLTF_COMPONENT_TYPE_INT:
//                    indicesArrayPtr = std::unique_ptr<intArray<int> >(new intArray<int>(
//                                                                                        arrayAdapter<int>(dataAddress, count, byteStride)));
                    break;
                    
                case SVGLTF_COMPONENT_TYPE_UNSIGNED_INT:
//                    indicesArrayPtr = std::unique_ptr<intArray<unsigned int> >(
//                                                                               new intArray<unsigned int>(arrayAdapter<unsigned int>(
//                                                                                                                                     dataAddress, count, byteStride)));
                    break;
                default:
                    break;
            }
//            const auto &indices = dataAddress;
//
//            if (indicesArrayPtr) {
//                std::cout << "indices: ";
//                for (size_t i(0); i < indicesArrayPtr->size(); ++i) {
//                    std::cout << indices[i] << " ";
//                    loadedMesh.faces.push_back(indices[i]);
//                }
//                std::cout << '\n';
//            }
//
//            switch (meshPrimitive.mode) {
//                    // We re-arrange the indices so that it describe a simple list of
//                    // triangles
//                case TINYGLTF_MODE_TRIANGLE_FAN:
//                    if (!convertedToTriangleList) {
//                        std::cout << "TRIANGLE_FAN\n";
//                        // This only has to be done once per primitive
//                        convertedToTriangleList = true;
//
//                        // We steal the guts of the vector
//                        auto triangleFan = std::move(loadedMesh.faces);
//                        loadedMesh.faces.clear();
//
//                        // Push back the indices that describe just one triangle one by one
//                        for (size_t i{2}; i < triangleFan.size(); ++i) {
//                            loadedMesh.faces.push_back(triangleFan[0]);
//                            loadedMesh.faces.push_back(triangleFan[i - 1]);
//                            loadedMesh.faces.push_back(triangleFan[i]);
//                        }
//                    }
//                case TINYGLTF_MODE_TRIANGLE_STRIP:
//                    if (!convertedToTriangleList) {
//                        std::cout << "TRIANGLE_STRIP\n";
//                        // This only has to be done once per primitive
//                        convertedToTriangleList = true;
//
//                        auto triangleStrip = std::move(loadedMesh.faces);
//                        loadedMesh.faces.clear();
//
//                        for (size_t i{2}; i < triangleStrip.size(); ++i) {
//                            loadedMesh.faces.push_back(triangleStrip[i - 2]);
//                            loadedMesh.faces.push_back(triangleStrip[i - 1]);
//                            loadedMesh.faces.push_back(triangleStrip[i]);
//                        }
//                    }
//                case TINYGLTF_MODE_TRIANGLES:  // this is the simpliest case to handle
//
//                {
//                    std::cout << "TRIANGLES\n";
//
//                    for (const auto &attribute : meshPrimitive.attributes) {
//                        const auto attribAccessor = model.accessors[attribute.second];
//                        const auto &bufferView =
//                        model.bufferViews[attribAccessor.bufferView];
//                        const auto &buffer = model.buffers[bufferView.buffer];
//                        const auto dataPtr = buffer.data.data() + bufferView.byteOffset +
//                        attribAccessor.byteOffset;
//                        const auto byte_stride = attribAccessor.ByteStride(bufferView);
//                        const auto count = attribAccessor.count;
//
//                        std::cout << "current attribute has count " << count
//                        << " and stride " << byte_stride << " bytes\n";
//
//                        std::cout << "attribute string is : " << attribute.first << '\n';
//                        if (attribute.first == "POSITION") {
//                            std::cout << "found position attribute\n";
//
//                            // get the position min/max for computing the boundingbox
//                            pMin.x = attribAccessor.minValues[0];
//                            pMin.y = attribAccessor.minValues[1];
//                            pMin.z = attribAccessor.minValues[2];
//                            pMax.x = attribAccessor.maxValues[0];
//                            pMax.y = attribAccessor.maxValues[1];
//                            pMax.z = attribAccessor.maxValues[2];
//
//                            switch (attribAccessor.type) {
//                                case TINYGLTF_TYPE_VEC3: {
//                                    switch (attribAccessor.componentType) {
//                                        case TINYGLTF_COMPONENT_TYPE_FLOAT:
//                                            std::cout << "Type is FLOAT\n";
//                                            // 3D vector of float
//                                            v3fArray positions(
//                                                               arrayAdapter<v3f>(dataPtr, count, byte_stride));
//
//                                            std::cout << "positions's size : " << positions.size()
//                                            << '\n';
//
//                                            for (size_t i{0}; i < positions.size(); ++i) {
//                                                const auto v = positions[i];
//                                                std::cout << "positions[" << i << "]: (" << v.x << ", "
//                                                << v.y << ", " << v.z << ")\n";
//
//                                                loadedMesh.vertices.push_back(v.x * scale);
//                                                loadedMesh.vertices.push_back(v.y * scale);
//                                                loadedMesh.vertices.push_back(v.z * scale);
//                                            }
//                                    }
//                                    break;
//                                case TINYGLTF_COMPONENT_TYPE_DOUBLE: {
//                                    std::cout << "Type is DOUBLE\n";
//                                    switch (attribAccessor.type) {
//                                        case TINYGLTF_TYPE_VEC3: {
//                                            v3dArray positions(
//                                                               arrayAdapter<v3d>(dataPtr, count, byte_stride));
//                                            for (size_t i{0}; i < positions.size(); ++i) {
//                                                const auto v = positions[i];
//                                                std::cout << "positions[" << i << "]: (" << v.x
//                                                << ", " << v.y << ", " << v.z << ")\n";
//
//                                                loadedMesh.vertices.push_back(v.x * scale);
//                                                loadedMesh.vertices.push_back(v.y * scale);
//                                                loadedMesh.vertices.push_back(v.z * scale);
//                                            }
//                                        } break;
//                                        default:
//                                            // TODO Handle error
//                                            break;
//                                    }
//                                    break;
//                                default:
//                                    break;
//                                }
//                                } break;
//                            }
//                        }
            
        }
    }
}

void SVGLTFModelNode::update(f32 dt) {
    
}

void SVGLTFModelNode::render() {
    if (!m_visible)
        return;
    if (!mApp->m_pGlobalParam->m_curScene)
        return;
//    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
//    if (m_pRObj) {
//        m_pRObj->pushCmd(t_rs, m_rsType, "SVSpineNode");
//    }
//    SVNode::render();
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
