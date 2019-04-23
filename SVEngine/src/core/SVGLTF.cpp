//
// SVglTF.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVGLTF.h"
#include "../file/SVParseDef.h"
#include "../base/SVDataChunk.h"
#include "../file/SVFileMgr.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../app/SVInst.h"
#include "../base/SVDataSwap.h"
#include "../base/SVQuat.h"
#include "../mtl/SVMtl3D.h"
#include "../rendercore/SVRenderMesh.h"

SVGLTF::SVGLTF(SVInst *_app)
:SVGBase(_app) {
}

SVGLTF::~SVGLTF() {
    
}

GLTFModelPtr SVGLTF::loadFromFile(cptr8 _filename){
    SVDataChunk tDataStream;
    bool tflag = mApp->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, _filename);
    if (!tflag)
        return nullptr;
    SV_LOG_INFO("SVglTF :load glTF JSON sucess\n");
    SV_LOG_INFO("SVglTF :filedata %s\n", tDataStream.m_data);
    if (!tDataStream.m_data) {
        SV_LOG_ERROR("SVglTF :data stream is null");
        return nullptr;
    }
    
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_ERROR("SVglTF :rapidjson error code:%d \n", code);
        return nullptr;
    }
    
    if (doc.HasMember("scenes") && doc["scenes"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s scenes object not found in .gltf or not an array type \n", _filename);
        return nullptr;
    }
    
    if (doc.HasMember("nodes") && doc["nodes"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s nodes object not found in .gltf or not an array type \n", _filename);
        return nullptr;
    }
    
    if (doc.HasMember("accessors") && doc["accessors"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s accessors object not found in .gltf or not an array type \n", _filename);
        return nullptr;
    }
    
    if (doc.HasMember("buffers") && doc["buffers"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s buffers object not found in .gltf or not an array type \n", _filename);
        return nullptr;
    }
    
    if (doc.HasMember("bufferViews") && doc["bufferViews"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s bufferViews object not found in .gltf or not an array type \n", _filename);
        return nullptr;
    }
    GLTFModelPtr _model = MakeSharedPtr<GLTFModel>();
    _model->buffers.clear();
    _model->bufferViews.clear();
    _model->accessors.clear();
    _model->meshes.clear();
    _model->cameras.clear();
    _model->nodes.clear();
    _model->extensionsUsed.clear();
    _model->extensionsRequired.clear();
    _model->defaultScene = -1;
    
    // 1. Parse Asset
    {
        if (doc.HasMember("asset") && doc["asset"].IsObject()) {
            RAPIDJSON_NAMESPACE::Value &item = doc["asset"];
            _parseAsset(&(_model->asset), item);
        }
    }
    
    // 2. Parse extensionUsed
    {
        if (doc.HasMember("extensionsUsed") && doc["extensionsUsed"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_extensionsUseds = doc["extensionsUsed"];
            for (s32 i = 0; i < t_extensionsUseds.Size(); ++i) {
                RAPIDJSON_NAMESPACE::Value &t_extensionsUsedItem = t_extensionsUseds[i];
                _model->extensionsUsed.append(t_extensionsUsedItem.GetString());
            }
        }
    }
    
    {
        if (doc.HasMember("extensionsRequired") && doc["extensionsRequired"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_extensionsRequireds = doc["extensionsRequired"];
            for (s32 i = 0; i < t_extensionsRequireds.Size(); ++i) {
                RAPIDJSON_NAMESPACE::Value &t_extensionsRequiredItem = t_extensionsRequireds[i];
                _model->extensionsRequired.append(t_extensionsRequiredItem.GetString());
            }
        }
    }
    
    // 3. Parse Buffer
    {
        if (doc.HasMember("buffers") && doc["buffers"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_buffers = doc["buffers"];
            for (s32 i = 0; i<t_buffers.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_bufferItem = t_buffers[i];
                if (t_bufferItem.IsObject()) {
                    Buffer buffer;
                    if (!_parseBuffer(&buffer, t_bufferItem, _filename)) {
                        return nullptr;
                    }
                    _model->buffers.append(buffer);
                }else{
                    SV_LOG_ERROR("SVglTF Error: 'buffers' does not contain an JSON object.");
                    return nullptr;
                }
            }
        }
    }
    
    // 4. Parse BufferView
    {
        if (doc.HasMember("bufferViews") && doc["bufferViews"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_bufferViews = doc["bufferViews"];
            for (s32 i = 0; i<t_bufferViews.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_bufferViewItem = t_bufferViews[i];
                if (!t_bufferViewItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error: 'bufferViews' does not contain an JSON object.");
                    return nullptr;
                }
                BufferView bufferView;
                if (!_parseBufferView(&bufferView, t_bufferViewItem)) {
                    return nullptr;
                }
                _model->bufferViews.append(bufferView);
            }
        }
    }
    
    // 5. Parse Accessor
    {
        if (doc.HasMember("accessors") && doc["accessors"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_accessors = doc["accessors"];
            for (s32 i=0; i<t_accessors.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_accessorItem = t_accessors[i];
                if (!t_accessorItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error: `accessors' does not contain an JSON object.");
                    return nullptr;
                }
                Accessor accessor;
                if (!_parseAccessor(&accessor, t_accessorItem)) {
                    return nullptr;
                }
                _model->accessors.append(accessor);
            }
        }
    }
    
    // 6. Parse Mesh
    {
        if (doc.HasMember("meshes") && doc["meshes"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_meshes = doc["meshes"];
            for (s32 i=0; i<t_meshes.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_meshItem = t_meshes[i];
                if (!t_meshItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error:`meshes' does not contain an JSON object.");
                    return nullptr;
                }
                Mesh mesh;
                if (!_parseMesh(&mesh, t_meshItem)) {
                    return nullptr;
                }
                _model->meshes.append(mesh);
            }
        }
    }
    
    // 7. Parse Node
    {
        if (doc.HasMember("nodes") && doc["nodes"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_nodes = doc["nodes"];
            for (s32 i=0; i<t_nodes.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_nodeItem = t_nodes[i];
                if (!t_nodeItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error:`nodes' does not contain an JSON object.");
                    return nullptr;
                }
                Node node;
                if (!_parseNode(&node, t_nodeItem)) {
                    return nullptr;
                }
                _model->nodes.append(node);
            }
        }
    }
    
    // 8. Parse scenes.
    {
        if (doc.HasMember("scenes") && doc["scenes"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_scenes = doc["scenes"];
            for (s32 i =0; i<t_scenes.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_sceneItem = t_scenes[i];
                if (!t_sceneItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error :`scenes' does not contain an JSON object.");
                    return nullptr;
                }
                SVArray<s32> nodesIds;
                if (t_sceneItem.HasMember("nodes") && t_sceneItem["nodes"].IsArray()) {
                    RAPIDJSON_NAMESPACE::Value &t_nodes = t_sceneItem["nodes"];
                    for (s32 j = 0; j<t_nodes.Size(); j++) {
                        nodesIds.append(t_nodes[j].GetInt());
                    }
                }
                
                Scene scene;
                if (t_sceneItem.HasMember("name") && t_sceneItem["name"].IsString()) {
                    scene.name = t_sceneItem["name"].GetString();
                }
                scene.nodes = nodesIds;
                _model->scenes.append(scene);
                
            }
        }
    }
    
    // 9. Parse default scenes.
    {
        if (doc.HasMember("scene") && doc["scene"].IsNumber()) {
            RAPIDJSON_NAMESPACE::Value &t_scene = doc["scene"];
            const s32 defaultScene = t_scene.GetInt();
            _model->defaultScene = defaultScene;
        }
    }
    
    // 10. Parse Material
    {
        if (doc.HasMember("materials") && doc["materials"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_materials = doc["materials"];
            for (s32 i =0; i<t_materials.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_material = t_materials[i];
                if (!t_material.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error :'materials' does not contain an JSON object.");
                    return nullptr;
                }
                Material material;
                if (t_material.HasMember("name") && t_material["name"].IsString()) {
                    material.name = t_material["name"].GetString();
                }
                
                if (!_parseMaterial(&material, t_material)) {
                    return nullptr;
                }
                _model->materials.append(material);
                
            }
        }
    }
    
    // 11. Parse Image
    {
        if (doc.HasMember("images") && doc["images"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_images = doc["images"];
            for (s32 i =0; i<t_images.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_image = t_images[i];
                if (!t_image.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error :'images' does not contain an JSON object.");
                    return nullptr;
                }
                Image image;
                if (!_parseImage(&image, t_image, _filename)) {
                    return nullptr;
                }
                if (image.bufferView != -1) {
                    // Load image from the buffer view.
                    
                    // not supporte now!!!!!
                    SV_LOG_ERROR("SVglTF Error :read image from buffer does not supporte now.");
                    return nullptr;
                }
                _model->images.append(image);
            }
        }
    }
        
    // 12. Parse Texture
    {
        if (doc.HasMember("textures") && doc["textures"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_textures = doc["textures"];
            for (s32 i=0; i<t_textures.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_textureItem = t_textures[i];
                if (!t_textureItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error :'textures' does not contain an JSON object.");
                    return nullptr;
                }
                    Texture texture;
                    if (!_parseTexture(&texture, t_textureItem, _filename)) {
                        return nullptr;
                    }
                    _model->textures.append(texture);
            }
        }
    }
    
    // 13. Parse Animation
    {
        if (doc.HasMember("animations") && doc["animations"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_animations = doc["animations"];
            for (s32 i=0; i<t_animations.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_animationItem = t_animations[i];
                if (!t_animationItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error :'animations' does not contain an JSON object.");
                    return nullptr;
                }
                Animation animation;
                if (!_parseAnimation(&animation, t_animationItem)) {
                    return nullptr;
                }
                _model->animations.append(animation);
            }
        }
    }
    
    // 14. Parse Skin
    {
        if (doc.HasMember("skins") && doc["skins"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_skins = doc["skins"];
            for (s32 i=0; i<t_skins.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_skinItem = t_skins[i];
                if (!t_skinItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error :'skins' does not contain an JSON object.");
                    return nullptr;
                }
                Skin skin;
                if (!_parseSkin(&skin, t_skinItem)) {
                    return nullptr;
                }
                _model->skins.append(skin);
            }
        }
    }
    
    // 15. Parse Sampler
    {
        if (doc.HasMember("samplers") && doc["samplers"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_samplers = doc["samplers"];
            for (s32 i=0; i<t_samplers.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_samplerItem = t_samplers[i];
                if (!t_samplerItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error :'samplers' does not contain an JSON object.");
                    return nullptr;
                }
                Sampler sampler;
                if (!_parseSampler(&sampler, t_samplerItem)) {
                    return nullptr;
                }
                _model->samplers.append(sampler);
            }
        }
    }
    
    // 16. Parse Camera
    {
//        if (doc.HasMember("cameras") && doc["cameras"].IsArray()) {
//            RAPIDJSON_NAMESPACE::Value &t_cameras = doc["cameras"];
//            for (s32 i=0; i<t_cameras.Size(); i++) {
//                RAPIDJSON_NAMESPACE::Value &t_cameraItem = t_cameras[i];
//                if (!t_cameraItem.IsObject()) {
//                    SV_LOG_ERROR("SVglTF Error :'cameras' does not contain an JSON object.");
//                    return false;
//                }
//                Camera camera;
//                if (!ParseCamera(&camera, err, it->get<json>())) {
//                    return false;
//                }
//
//                model->cameras.push_back(camera);
//            }
//        }
    }
    
    //load mesh data
    _loadMeshData(_model);
    //load animation
    _loadAnimationData(_model);
    return _model;
}

bool SVGLTF::_parseAsset(Asset *_asset , RAPIDJSON_NAMESPACE::Value &_item){
    if (_item.HasMember("version") && _item["version"].IsString()) {
        _asset->version = _item["version"].GetString();
    }
    if (_item.HasMember("generator") && _item["generator"].IsString()) {
        _asset->generator = _item["generator"].GetString();
    }
    if (_item.HasMember("minVersion") && _item["minVersion"].IsString()) {
        _asset->minVersion = _item["minVersion"].GetString();
    }
    return true;
}

bool SVGLTF::_parseBuffer(Buffer *_buffer, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir){
    
    f64 byteLength;
    if (_item.HasMember("byteLength") && _item["byteLength"].IsNumber()) {
        byteLength = _item["byteLength"].GetDouble();
    }else{
        return false;
    }
    
    _buffer->uri = "";
    if (_item.HasMember("uri") && _item["uri"].IsString()) {
        _buffer->uri = _item["uri"].GetString();
    }else{
        return false;
    }
    
    if (_buffer->uri.size() == 0) {
        SV_LOG_ERROR("SVglTF Error :'uri' is missing from non binary glTF file buffer.\n");
        return false;
    }
    
    if (_item.HasMember("type") && _item["type"].IsString()) {
        SVString ty = _item["type"].GetString();
        if (strcmp(ty.c_str(), "arraybuffer") == 0) {
            // buffer.type = "arraybuffer";
        }
    }
    
    s64 bytes = s64(byteLength);
    if (_isDataURI((_buffer->uri).c_str())) {
        //nothing, maybe will..
        return false;
    }else{
        // External .bin file.
        SVString t_baseDir = _basedir;
        s32 t_pos = t_baseDir.rfind('/');
        SVString t_bufferUri = SVString::substr(t_baseDir.c_str(), 0, t_pos+1) + _buffer->uri;
        if (!_loadExternalFile(_buffer->data, t_bufferUri.c_str(), bytes)) {
            return false;
        }
    }
    
    return true;
}

bool SVGLTF::_parseBufferView(BufferView *_bufferView, RAPIDJSON_NAMESPACE::Value &_item){
    f64 buffer = -1.0;
    if (_item.HasMember("buffer") && _item["buffer"].IsNumber()) {
        buffer = _item["buffer"].GetDouble();
    }
    
    f64 byteOffset = 0.0;
    if (_item.HasMember("byteOffset") && _item["byteOffset"].IsNumber()) {
        byteOffset = _item["byteOffset"].GetDouble();
    }
    
    f64 byteLength = 1.0;
    if (_item.HasMember("byteLength") && _item["byteLength"].IsNumber()) {
        byteLength = _item["byteLength"].GetDouble();
    }
    
    s64 byteStride = 0;
    f64 byteStrideValue = 0.0;
    if (!_item.HasMember("byteStride")) {
        byteStride = 0;
    }else{
        byteStrideValue = _item["byteStride"].GetDouble();
        byteStride = s64(byteStrideValue);
    }
    
    if ((byteStride > 252) || ((byteStride % 4) != 0)) {
        SV_LOG_ERROR("SVGLTF Error:Invalid 'byteStride' value. `byteStride' must be the multiple of");
        return false;
    }
    
    f64 target = 0.0;
    if (_item.HasMember("target") && _item["target"].IsNumber()) {
        target = _item["target"].GetDouble();
    }
    s32 targetValue = s32(target);
    if ((targetValue == SVGLTF_TARGET_ARRAY_BUFFER) ||
        (targetValue == SVGLTF_TARGET_ELEMENT_ARRAY_BUFFER)) {
        // OK
    } else {
        targetValue = 0;
    }
    _bufferView->target = targetValue;
    
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _bufferView->name = _item["name"].GetString();
    }
    _bufferView->buffer = s32(buffer);
    _bufferView->byteOffset = s64(byteOffset);
    _bufferView->byteLength = s64(byteLength);
    _bufferView->byteStride = s64(byteStride);
    
    return true;
}

bool SVGLTF::_parseAccessor(Accessor *_accessor, RAPIDJSON_NAMESPACE::Value &_item){
    f64 bufferView = -1.0;
    
    if (_item.HasMember("bufferView") && _item["bufferView"].IsNumber()) {
        bufferView = _item["bufferView"].GetDouble();
    }
    
    f64 byteOffset = 0.0;
    if (_item.HasMember("byteOffset") && _item["byteOffset"].IsNumber()) {
        byteOffset = _item["byteOffset"].GetDouble();
    }
    
    bool normalized = false;
    if (_item.HasMember("normalized") && _item["normalized"].IsBool()) {
        normalized = _item["normalized"].GetBool();
    }
    
    f64 componentType = 0.0;
    if (_item.HasMember("componentType") && _item["componentType"].IsNumber()) {
        componentType = _item["componentType"].GetDouble();
    }
    
    f64 count = 0.0;
    if (_item.HasMember("count") && _item["count"].IsNumber()) {
        count = _item["count"].GetDouble();
    }
    
    SVString type;
    if (_item.HasMember("type") && _item["type"].IsString()) {
        type = _item["type"].GetString();
    }
    
    if (strcmp(type.c_str(), "SCALAR") == 0) {
        _accessor->type = SVGLTF_TYPE_SCALAR;
    } else if (strcmp(type.c_str(), "VEC2") == 0) {
        _accessor->type = SVGLTF_TYPE_VEC2;
    } else if (strcmp(type.c_str(), "VEC3") == 0) {
        _accessor->type = SVGLTF_TYPE_VEC3;
    } else if (strcmp(type.c_str(), "VEC4") == 0) {
        _accessor->type = SVGLTF_TYPE_VEC4;
    } else if (strcmp(type.c_str(), "MAT2") == 0) {
        _accessor->type = SVGLTF_TYPE_MAT2;
    } else if (strcmp(type.c_str(), "MAT3") == 0) {
        _accessor->type = SVGLTF_TYPE_MAT3;
    } else if (strcmp(type.c_str(), "MAT4") == 0) {
        _accessor->type = SVGLTF_TYPE_MAT4;
    } else {
        SV_LOG_ERROR("Unsupported `type` for accessor object.");
        return false;
    }
    
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _accessor->name = _item["name"].GetString();
    }
    
    _accessor->minValues.clear();
    _accessor->maxValues.clear();
    
    if (_item.HasMember("min") && _item["min"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &mins = _item["min"];
        for (s32 i= 0 ; i<mins.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_minItem = mins[i];
            _accessor->minValues.append(t_minItem.GetDouble());
        }
    }
    if (_item.HasMember("max") && _item["max"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &maxs = _item["max"];
        for (s32 i= 0 ; i<maxs.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_maxItem = maxs[i];
            _accessor->maxValues.append(t_maxItem.GetDouble());
        }
    }
    
    _accessor->count = static_cast<s64>(count);
    _accessor->bufferView = static_cast<s32>(bufferView);
    _accessor->byteOffset = static_cast<s64>(byteOffset);
    _accessor->normalized = normalized;
    {
        s32 comp = static_cast<s32>(componentType);
        if (comp >= SVGLTF_COMPONENT_TYPE_BYTE &&
            comp <= SVGLTF_COMPONENT_TYPE_DOUBLE) {
            // OK
            _accessor->componentType = comp;
        } else {
            SV_LOG_ERROR("Invalid `componentType` in accessor.");
            return false;
        }
    }
    return true;
}

bool SVGLTF::_parseMesh(Mesh *_mesh, RAPIDJSON_NAMESPACE::Value &_item){
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _mesh->name = _item["name"].GetString();
    }
    
    _mesh->primitives.clear();
    if (_item.HasMember("primitives") && _item["primitives"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_primitives = _item["primitives"];
        for (s32 i = 0; i<t_primitives.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_primitiveItem = t_primitives[i];
            Primitive primitive;
            if (_parsePrimitive(&primitive, t_primitiveItem)) {
                // Only add the primitive if the parsing succeeds.
                _mesh->primitives.append(primitive);
            }
        }
    }
    
    // Look for morph targets
    if (_item.HasMember("targets") && _item["targets"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_targets = _item["targets"];
        for(auto iter = t_targets.MemberBegin(); iter != t_targets.MemberEnd(); ++iter){
            SVMap<SVString, s32> targetAttribues;
            cptr8 key = (iter->name).GetString();
            RAPIDJSON_NAMESPACE::Value &value = iter->value;
            targetAttribues.append(SVString(key), value.GetInt());
            _mesh->targets.append(targetAttribues);
        }
    }
    
    
    // Should probably check if has targets and if dimensions fit
    if (_item.HasMember("weights") && _item["weights"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_weights = _item["weights"];
        for (s32 i=0; i<t_weights.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_weightItem = t_weights[i];
            _mesh->weights.append(t_weightItem.GetDouble());
        }
    }
    return true;
}

bool SVGLTF::_parsePrimitive(Primitive *_primitive, RAPIDJSON_NAMESPACE::Value &_item){
    f64 material = -1.0;
    if (_item.HasMember("material") && _item["material"].IsNumber()) {
        material = _item["material"].GetDouble();
    }
    _primitive->material = s32(material);
    
    f64 mode = static_cast<f64>(SVGLTF_MODE_TRIANGLES);
    if (_item.HasMember("mode") && _item["mode"].IsNumber()) {
        mode = _item["mode"].GetDouble();
    }
    s32 primMode = static_cast<s32>(mode);
    _primitive->mode = primMode;  // Why only triangled were supported ?
    
    f64 indices = -1.0;
    if (_item.HasMember("indices") && _item["indices"].IsNumber()) {
        indices = _item["indices"].GetDouble();
    }
    _primitive->indices = static_cast<s32>(indices);
    
    if (_item.HasMember("attributes") && _item["attributes"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &t_attributes = _item["attributes"];
        for(auto iter = t_attributes.MemberBegin(); iter != t_attributes.MemberEnd(); ++iter){
            SVMap<SVString, s32> t_attribuesMap;
            cptr8 key = (iter->name).GetString();
            RAPIDJSON_NAMESPACE::Value &value = iter->value;
            t_attribuesMap.append(SVString(key), value.GetInt());
            _primitive->attributes.append(t_attribuesMap);
        }
    }
    
    // Look for morph targets
    if (_item.HasMember("targets") && _item["targets"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_targets = _item["targets"];
        for(auto iter = t_targets.MemberBegin(); iter != t_targets.MemberEnd(); ++iter){
            SVMap<SVString, s32> targetAttribues;
            cptr8 key = (iter->name).GetString();
            RAPIDJSON_NAMESPACE::Value &value = iter->value;
            targetAttribues.append(SVString(key), value.GetInt());
            _primitive->targets.append(targetAttribues);
        }
    }
    return true;
}

bool SVGLTF::_parseNode(Node *_node, RAPIDJSON_NAMESPACE::Value &_item){
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _node->name = _item["name"].GetString();
    }
    
    f64 skin = -1.0;
    if (_item.HasMember("skin") && _item["skin"].IsNumber()) {
        skin = _item["skin"].GetDouble();
    }
    _node->skin = static_cast<s32>(skin);
    
    // Matrix and T/R/S are exclusive
    if (_item.HasMember("matrix") && _item["matrix"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_matrixs = _item["matrix"];
        for (s32 i=0; i<t_matrixs.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_matrixItem = t_matrixs[i];
            _node->matrix.append(t_matrixItem.GetDouble());
        }
    }
    if (_item.HasMember("rotation") && _item["rotation"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_rotations = _item["rotation"];
        for (s32 i=0; i<t_rotations.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_rotationItem = t_rotations[i];
            _node->rotation.append(t_rotationItem.GetDouble());
        }
    }
    if (_item.HasMember("scale") && _item["scale"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_scales = _item["scale"];
        for (s32 i=0; i<t_scales.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_scaleItem = t_scales[i];
            _node->scale.append(t_scaleItem.GetDouble());
        }
    }
    if (_item.HasMember("translation") && _item["translation"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_translations = _item["translation"];
        for (s32 i=0; i<t_translations.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_translationItem = t_translations[i];
            _node->translation.append(t_translationItem.GetDouble());
        }
    }
    
    f64 camera = -1.0;
    if (_item.HasMember("camera") && _item["camera"].IsNumber()) {
        camera = _item["camera"].GetDouble();
    }
    _node->camera = static_cast<s32>(camera);
    
    f64 mesh = -1.0;
    if (_item.HasMember("mesh") && _item["mesh"].IsNumber()) {
        mesh = _item["mesh"].GetDouble();
    }
    _node->mesh = s32(mesh);
    
    _node->children.clear();
    if (_item.HasMember("children") && _item["children"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_childrens = _item["children"];
        for (s32 i=0; i<t_childrens.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_childrenItem = t_childrens[i];
            _node->children.append(t_childrenItem.GetInt());
        }
    }
    return true;
}

bool SVGLTF::_parseMaterial(Material *_material, RAPIDJSON_NAMESPACE::Value &_item) {
    _material->values.clear();
    _material->additionalValues.clear();
    
    for(auto iter = _item.MemberBegin(); iter != _item.MemberEnd(); ++iter){
        cptr8 key = (iter->name).GetString();
        if (strcmp(key, "pbrMetallicRoughness") == 0) {
            RAPIDJSON_NAMESPACE::Value &itemVal = iter->value;
            for (auto iterVal = itemVal.MemberBegin(); iterVal != itemVal.MemberEnd(); ++iterVal) {
                Parameter param;
                if (_parseParameterProperty(&param, itemVal, (iterVal->name).GetString())) {
                    _material->values.append((iterVal->name).GetString(), param);
                }
            }
        }else if (strcmp(key, "extensions") == 0 || strcmp(key, "extras") == 0) {
            
        } else {
            Parameter param;
            if (_parseParameterProperty(&param, _item, key)) {
                _material->additionalValues.append(key, param);
            }
        }
    }
    return true;
}

bool SVGLTF::_parseParameterProperty(Parameter *_param, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _prop){
    if (_item.HasMember(_prop) && _item[_prop].IsString()) {
        _param->string_value = _item[_prop].GetString();
        return true;
    }
    
    if (_item.HasMember(_prop) && _item[_prop].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_array = _item[_prop];
        for (s32 i=0; i<t_array.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_item = t_array[i];
            _param->number_array.append(t_item.GetDouble());
        }
        return true;
    }

    if (_item.HasMember(_prop) && _item[_prop].IsNumber()) {
        return _param->has_number_value = true;
    }
    
    if (_item.HasMember(_prop) && _item[_prop].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &t_object = _item[_prop];
        for(auto iter = t_object.MemberBegin(); iter != t_object.MemberEnd(); ++iter){
            cptr8 key = (iter->name).GetString();
            if (iter->value.IsNumber()) {
                _param->json_double_value.append(key, iter->value.GetDouble());
            }
        }
        return true;
    }
    
    if (_item.HasMember(_prop) && _item[_prop].IsBool()) {
        return true;
    }
    
    return false;
}


bool SVGLTF::_parseTexture(Texture *_texture, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir) {
    f64 sampler = -1.0;
    f64 source = -1.0;
    if (_item.HasMember("sampler") && _item["sampler"].IsNumber()) {
        sampler = _item["sampler"].GetDouble();
    }
    if (_item.HasMember("source") && _item["source"].IsNumber()) {
        source = _item["source"].GetDouble();
    }
    _texture->sampler = s32(sampler);
    _texture->source = s32(source);
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _texture->name = _item["name"].GetString();
    }
    return true;
}

bool SVGLTF::_parseAnimation(Animation *_animation, RAPIDJSON_NAMESPACE::Value &_item) {
    {
        if (_item.HasMember("channels") && _item["channels"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_channels = _item["channels"];
            for (s32 i=0; i<t_channels.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_channelItem = t_channels[i];
                AnimationChannel channel;
                if (_parseAnimationChannel(&channel, t_channelItem)) {
                    // Only add the channel if the parsing succeeds.
                    _animation->channels.append(channel);
                }
            }
        }
    }
    
    {
        if (_item.HasMember("samplers") && _item["samplers"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_samplers = _item["samplers"];
            for (s32 i=0; i<t_samplers.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_samplerItem = t_samplers[i];
                AnimationSampler sampler;
                f64 inputIndex = -1.0;
                f64 outputIndex = -1.0;
                if (t_samplerItem.HasMember("input") && t_samplerItem["input"].IsNumber()) {
                    inputIndex = t_samplerItem["input"].GetDouble();
                }else{
                    SV_LOG_ERROR("SVglTF Error :'input' field is missing in animation.sampler\n");
                    return false;
                }
                
                if (t_samplerItem.HasMember("interpolation") && t_samplerItem["interpolation"].IsString()) {
                    sampler.interpolation = t_samplerItem["interpolation"].GetString();
                }else{
                    SV_LOG_ERROR("SVglTF Error :'interpolation' field is missing in animation.sampler\n");
                    return false;
                }
                
                if (t_samplerItem.HasMember("output") && t_samplerItem["output"].IsNumber()) {
                    outputIndex = t_samplerItem["output"].GetDouble();
                }else{
                    SV_LOG_ERROR("SVglTF Error :'output' field is missing in animation.sampler\n");
                    return false;
                }
                
                sampler.input = s32(inputIndex);
                sampler.output = s32(outputIndex);
                _animation->samplers.append(sampler);
            }
        }
    }
    
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _animation->name = _item["name"].GetString();
    }
    return true;
}

bool SVGLTF::_parseAnimationChannel(AnimationChannel *_channel, RAPIDJSON_NAMESPACE::Value &_item) {
    f64 samplerIndex = -1.0;
    f64 targetIndex = -1.0;
    if (_item.HasMember("sampler") && _item["sampler"].IsNumber()) {
        samplerIndex = _item["sampler"].GetDouble();
    }
    
    if (_item.HasMember("target") && _item["target"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &t_targetItem = _item["target"];
        if (t_targetItem.HasMember("node") && t_targetItem["node"].IsNumber()) {
            targetIndex = t_targetItem["node"].GetDouble();
        }
        if (t_targetItem.HasMember("path") && t_targetItem["path"].IsString()) {
            _channel->target_path = t_targetItem["path"].GetString();
        }
    }
    
    _channel->sampler = s32(samplerIndex);
    _channel->target_node = s32(targetIndex);
    return true;
}

bool SVGLTF::_parseSkin(Skin *_skin, RAPIDJSON_NAMESPACE::Value &_item) {
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _skin->name = _item["name"].GetString();
    }
    
    SVArray<f64> joints;
    if (_item.HasMember("joints") && _item["joints"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_joints = _item["joints"];
        for (s32 i=0; i<t_joints.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_jointItem = t_joints[i];
            if (t_jointItem.IsNumber()) {
                _skin->joints.append(t_jointItem.GetInt());
            }
        }
    }
    
    f64 skeleton = -1.0;
    if (_item.HasMember("skeleton") && _item["skeleton"].IsNumber()) {
        skeleton = _item["skeleton"].GetDouble();
    }
    _skin->skeleton = s32(skeleton);
    
    f64 invBind = -1.0;
    if (_item.HasMember("inverseBindMatrices") && _item["inverseBindMatrices"].IsNumber()) {
        invBind = _item["inverseBindMatrices"].GetDouble();
    }
    _skin->inverseBindMatrices = s32(invBind);
    
    return true;
}

bool SVGLTF::_parseSampler(Sampler *_sampler, RAPIDJSON_NAMESPACE::Value &_item) {
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _sampler->name = _item["name"].GetString();
    }
    
    f64 minFilter = f64(SVGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR);
    f64 magFilter = f64(SVGLTF_TEXTURE_FILTER_LINEAR);
    f64 wrapS = f64(SVGLTF_TEXTURE_WRAP_REPEAT);
    f64 wrapT = f64(SVGLTF_TEXTURE_WRAP_REPEAT);
    if (_item.HasMember("minFilter") && _item["minFilter"].IsNumber()) {
        minFilter = _item["minFilter"].GetDouble();
    }
    if (_item.HasMember("magFilter") && _item["magFilter"].IsNumber()) {
        magFilter = _item["magFilter"].GetDouble();
    }
    if (_item.HasMember("wrapS") && _item["wrapS"].IsNumber()) {
        wrapS = _item["wrapS"].GetDouble();
    }
    if (_item.HasMember("wrapT") && _item["wrapT"].IsNumber()) {
        wrapT = _item["wrapT"].GetDouble();
    }
    
    _sampler->minFilter = s32(minFilter);
    _sampler->magFilter = s32(magFilter);
    _sampler->wrapS = s32(wrapS);
    _sampler->wrapT = s32(wrapT);
    return true;
}

bool SVGLTF::_loadExternalFile(SVDataSwapPtr _dataOut, cptr8 _filename, s64 _reqBytes){
    SVDataChunk tDataStream;
    bool tflag = mApp->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, _filename);
    if (!tflag){
        SV_LOG_ERROR("SVglTF Error:load ExternalFile failed\n");
        return false;
    }
    SV_LOG_INFO("SVglTF :ExternalFile filedata %s\n", tDataStream.m_data);
    if (!tDataStream.m_data) {
        SV_LOG_ERROR("SVglTF Error:ExternalFile data stream is null");
        return false;
    }
    
    //SVDataChunk 空间多申请了一个字节
    if ((tDataStream.m_size - 1) != _reqBytes) {
        SV_LOG_ERROR("SVglTF Error:ExternalFile File size mismatch requestedBytes");
        return false;
    }
    _dataOut->writeData(tDataStream.m_data, tDataStream.m_size);
    return true;
}

bool SVGLTF::_parseImage(Image *_image, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir){
    // A glTF image must either reference a bufferView or an image uri
    bool hasBufferView = false;
    if (_item.HasMember("bufferView")) {
        hasBufferView = true;
    }
    
    bool hasURI = false;
    if (_item.HasMember("uri")) {
        hasURI = true;
    }
    
    if (hasBufferView && hasURI) {
        SV_LOG_ERROR("SVglTF Error:Only one of `bufferView` or `uri` should be defined, but both are defined for Image.\n");
        return false;
    }
    
    if (!hasBufferView && !hasURI) {
        SV_LOG_ERROR("SVglTF Error:Neither required `bufferView` nor `uri` defined for Image.\n");
        return false;
    }
    
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _image->name = _item["name"].GetString();
    }
    
    if (hasBufferView) {
        f64 bufferView = -1;
        if (_item.HasMember("bufferView") && _item["bufferView"].IsNumber()) {
            bufferView = _item["bufferView"].GetDouble();
        }
        SVString mime_type;
        if (_item.HasMember("mimeType") && _item["mimeType"].IsString()) {
            mime_type = _item["mimeType"].GetString();
        }
        
        f64 width = 0.0;
        if (_item.HasMember("width") && _item["width"].IsNumber()) {
            width = _item["width"].GetDouble();
        }
        
        f64 height = 0.0;
        if (_item.HasMember("height") && _item["height"].IsNumber()) {
            height = _item["height"].GetDouble();
        }
        _image->bufferView = s32(bufferView);
        _image->mimeType = mime_type;
        _image->width = s32(width);
        _image->height = s32(height);
        return true;
    }
    
    // Parse URI & Load image data.
    SVString uri;
    if (_item.HasMember("uri") && _item["uri"].IsString()) {
        uri = _item["uri"].GetString();
    }
    if (uri.size() == 0) {
        SV_LOG_ERROR("SVglTF Error:Failed to load external 'uri' for image parameter\n");
        return false;
    }

    if (_isDataURI(uri)) {
        //nothing, maybe will..
        return false;
    } else {
        _image->uri = uri;
        SVString t_baseDir = _basedir;
        s32 t_pos = t_baseDir.rfind('/');
        SVString t_imageUri = SVString::substr(t_baseDir.c_str(), 0, t_pos + 1) + uri;
        _image->texture = mApp->getTexMgr()->getTextureSync(t_imageUri, true);
    }
    return true;
}

bool SVGLTF::_isDataURI(cptr8 _inData){
    SVString header = "data:application/octet-stream;base64,";
    SVString inData = _inData;
    if (inData.find(header) == 0) {
        return true;
    }
    
    header = "data:image/jpeg;base64,";
    if (inData.find(header) == 0) {
        return true;
    }
    
    header = "data:image/png;base64,";
    if (inData.find(header) == 0) {
        return true;
    }
    
    header = "data:image/bmp;base64,";
    if (inData.find(header) == 0) {
        return true;
    }
    
    header = "data:image/gif;base64,";
    if (inData.find(header) == 0) {
        return true;
    }
    
    header = "data:text/plain;base64,";
    if (inData.find(header) == 0) {
        return true;
    }
    
    header = "data:application/gltf-buffer;base64,";
    if (inData.find(header) == 0) {
        return true;
    }
    
    return false;
}

bool SVGLTF::_decodeDataURI(SVString &_outData, SVString &_mime_type,
                    cptr8 _inData, s64 _reqBytes, bool _checkSize){
    SVString header = "data:application/octet-stream;base64,";
    SVString inData = _inData;
    SVString data;
    if (inData.find(header) == 0) {
        data = _base64_decode(SVString::substr(inData.c_str(), header.size()));  // cut mime string.
    }
    
    if (data.empty()) {
        header = "data:image/jpeg;base64,";
        if (inData.find(header) == 0) {
            _mime_type = "image/jpeg";
            data = _base64_decode(SVString::substr(inData.c_str(), header.size()));  // cut mime string.
        }
    }
    
    if (data.empty()) {
        header = "data:image/png;base64,";
        if (inData.find(header) == 0) {
            _mime_type = "image/png";
            data = _base64_decode(SVString::substr(inData.c_str(), header.size()));  // cut mime string.
        }
    }
    
    if (data.empty()) {
        header = "data:image/bmp;base64,";
        if (inData.find(header) == 0) {
            _mime_type = "image/bmp";
            data = _base64_decode(SVString::substr(inData.c_str(), header.size()));  // cut mime string.
        }
    }
    
    if (data.empty()) {
        header = "data:image/gif;base64,";
        if (inData.find(header) == 0) {
            _mime_type = "image/gif";
            data = _base64_decode(SVString::substr(inData.c_str(), header.size()));  // cut mime string.
        }
    }
    
    if (data.empty()) {
        header = "data:text/plain;base64,";
        if (inData.find(header) == 0) {
            _mime_type = "text/plain";
            data = _base64_decode(SVString::substr(inData.c_str(), header.size()));
        }
    }
    
    if (data.empty()) {
        header = "data:application/gltf-buffer;base64,";
        if (inData.find(header) == 0) {
            data = _base64_decode(SVString::substr(inData.c_str(), header.size()));
        }
    }
    
    if (data.empty()) {
        return false;
    }
    _outData = data;
    
//    if (checkSize) {
//        if (data.size() != reqBytes) {
//            return false;
//        }
//        out->resize(reqBytes);
//    } else {
//        out->resize(data.size());
//    }
//    std::copy(data.begin(), data.end(), out->begin());
    return true;
}

static const SVString base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

cptr8 SVGLTF::_base64_encode(unsigned char const *bytes_to_encode,
                          unsigned int in_len) {
    SVString ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] =
            ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] =
            ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for (i = 0; (i < 4); i++) ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j < 3; j++) char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] =
        ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] =
        ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        
        for (j = 0; (j < i + 1); j++) ret += base64_chars[char_array_4[j]];
        
        while ((i++ < 3)) ret += '=';
    }
    
    return ret.c_str();
}

cptr8 SVGLTF::_base64_decode(SVString const &encoded_string){
    int in_len = static_cast<int>(encoded_string.size());
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    SVString ret;
    
    while (in_len-- && (encoded_string[in_] != '=') &&
           is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] =
                static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
            
            char_array_3[0] =
            (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] =
            ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++) ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j < 4; j++) char_array_4[j] = 0;
        
        for (j = 0; j < 4; j++)
            char_array_4[j] =
            static_cast<unsigned char>(base64_chars.find(char_array_4[j]));
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] =
        ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret.c_str();
}

void SVGLTF::_loadMeshData(GLTFModelPtr _model){
    _model->m_meshes.clear();
    for (s32 i = 0; i<_model->meshes.size(); i++) {
        Mesh mesh = _model->meshes[i];
        SVGLTFMeshPtr gltfMesh = MakeSharedPtr<SVGLTFMesh>();
        gltfMesh->m_name = mesh.name;
        for (s32 j = 0; j<mesh.primitives.size(); j++) {
            //
            ModelRenderDataPtr renderMesh = MakeSharedPtr<ModelRenderData>();
            renderMesh->m_pMesh = MakeSharedPtr<SVRenderMesh>(mApp);
            renderMesh->m_pMtl = MakeSharedPtr<SVMtl3D>(mApp, "normal3d_notex");
            renderMesh->m_boundBox.clear();
            _model->m_renderMeshData.append(renderMesh);
            //
            //debug
            ModelRenderDataPtr debugRenderMesh = MakeSharedPtr<ModelRenderData>();
            debugRenderMesh->m_pMesh = MakeSharedPtr<SVRenderMesh>(mApp);
            debugRenderMesh->m_pMtl = MakeSharedPtr<SVMtlCore>(mApp, "debugNormalLine");
            _model->m_renderDebugMeshData.append(debugRenderMesh);
            //
            SVGLTFSubMeshPtr gltfSubMesh = MakeSharedPtr<SVGLTFSubMesh>();
            Primitive primitive = mesh.primitives[j];
            gltfSubMesh->m_primitiveType = primitive.mode;
            //basetexture
            SVGLTFMaterialPtr gltfMaterial = MakeSharedPtr<SVGLTFMaterial>();
            s32 materialID = primitive.material;
            Material material = _model->materials[materialID];
            ParameterMap values = material.values;
            ParameterMap::Iterator baseTexValue = values.find("baseColorTexture");
            if( baseTexValue!=values.end() ) {
                Parameter parameter = baseTexValue->data;
                s32 textureIndex = parameter.TextureIndex();
                Texture texture = _model->textures[textureIndex];
                Image image = _model->images[texture.source];
                gltfMaterial->m_baseColorTexture = image.texture;
                renderMesh->m_pMtl = MakeSharedPtr<SVMtl3D>(mApp, "normal3d");
                renderMesh->m_pMtl->setTexture(0,image.texture);
            }
            //basecolor
            FVec4 color;
            color.set(1.0f, 1.0f, 1.0f, 1.0f);
            ParameterMap::Iterator baseColorValue = values.find("baseColorFactor");
            if( baseColorValue!=values.end() ) {
                Parameter parameter = baseColorValue->data;
                SVArray<f64> colorFactor = parameter.number_array;
                color.set(colorFactor[0], colorFactor[1], colorFactor[2], colorFactor[3]);
                gltfMaterial->m_baseColorFactor = color;
            }
            Accessor indicesAccessor = _model->accessors[primitive.indices];
            BufferView bufferView = _model->bufferViews[indicesAccessor.bufferView];
            Buffer buffer = _model->buffers[bufferView.buffer];
            // index
            SVGLTFAccessorPtr gltfIndicesAccessor = MakeSharedPtr<SVGLTFAccessor>();
            s32 byteStride = indicesAccessor.ByteStride(bufferView);
            s64 count = indicesAccessor.count;
            u8 *dataAddress = (u8 *)buffer.data->getData() + bufferView.byteOffset +indicesAccessor.byteOffset;
            gltfIndicesAccessor->m_count = count;
            gltfIndicesAccessor->m_offset = indicesAccessor.byteOffset;
            gltfIndicesAccessor->m_componentType = indicesAccessor.componentType;
            gltfIndicesAccessor->m_dimensionType = indicesAccessor.type;
            gltfIndicesAccessor->m_bufferData = MakeSharedPtr<SVDataSwap>();
            gltfIndicesAccessor->m_bufferData->writeData(dataAddress, byteStride*count);
            gltfIndicesAccessor->m_minValues = indicesAccessor.minValues;
            gltfIndicesAccessor->m_maxValues = indicesAccessor.maxValues;
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
                    renderMesh->m_indexCount = count;
                    renderMesh->m_pRenderIndex = indicesData;
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
                    renderMesh->m_indexCount = count;
                    renderMesh->m_pRenderIndex = indicesData;
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
                    renderMesh->m_indexCount = count;
                    renderMesh->m_pRenderIndex = indicesData;
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
                    renderMesh->m_indexCount = count;
                    renderMesh->m_pRenderIndex = indicesData;
                    break;
                }
                default:
                break;
            }
            switch (primitive.mode) {
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
                    SVArray<FVec4> t_joints0;
                    SVArray<FVec4> t_weights;
                    SVMap<SVString, s32>::Iterator primitiveIt = primitive.attributes.begin();
                    while ( primitiveIt!=primitive.attributes.end() ) {
                        const s32 attributID = primitiveIt->data;
                        Accessor attribAccessor = _model->accessors[attributID];
                        BufferView bufferView = _model->bufferViews[attribAccessor.bufferView];
                        Buffer buffer = _model->buffers[bufferView.buffer];
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
                            renderMesh->m_boundBox.expand(minV);
                            FVec3 maxV;
                            maxV.x = attribAccessor.maxValues[0];
                            maxV.y = attribAccessor.maxValues[1];
                            maxV.z = attribAccessor.maxValues[2];
                            renderMesh->m_boundBox.expand(maxV);
                            
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
                        //joints_0
                        if (strcmp(primitiveIt->key, "JOINTS_0") == 0) {
                            switch (attribAccessor.type) {
                                case SVGLTF_TYPE_VEC4: {
                                    switch (attribAccessor.componentType) {
                                        case SVGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
                                            u16 *data = (u16 *)dataPtr;
                                            for (s32 pt = 0; pt<count*4; pt += 4) {
                                                FVec4 t_joint;
                                                t_joint.x = data[pt + 0];
                                                t_joint.y = data[pt + 1];
                                                t_joint.z = data[pt + 2];
                                                t_joint.w = data[pt + 3];
                                                t_joints0.append(t_joint);
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
                        //weights
                        if (strcmp(primitiveIt->key, "WEIGHTS_0") == 0) {
                            switch (attribAccessor.type) {
                                case SVGLTF_TYPE_VEC4: {
                                    switch (attribAccessor.componentType) {
                                        case SVGLTF_COMPONENT_TYPE_FLOAT: {
                                            f32 *data = (f32 *)dataPtr;
                                            for (s32 pt = 0; pt<count*4; pt += 4) {
                                                FVec4 t_weight;
                                                t_weight.x = data[pt + 0];
                                                t_weight.y = data[pt + 1];
                                                t_weight.z = data[pt + 2];
                                                t_weight.w = data[pt + 3];
                                                t_weights.append(t_weight);
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
                    SVArray<V3_N_C_T0> renderVertexData;
                    for (s32 vi = 0; vi <t_postions.size(); vi++) {
                        V3_N_C_T0 vertexPt;
                        vertexPt.x = t_postions[vi].x;
                        vertexPt.y = t_postions[vi].y;
                        vertexPt.z = t_postions[vi].z;
                        vertexPt.r = (u8)(color.x*255);
                        vertexPt.g = (u8)(color.y*255);
                        vertexPt.b = (u8)(color.z*255);
                        vertexPt.a = ( u8)(color.w*255);
                        vertexPt.nx = t_normals[vi].x;
                        vertexPt.ny = t_normals[vi].y;
                        vertexPt.nz = t_normals[vi].z;
                        if (vi < t_texcoord0s.size()) {
                            vertexPt.t0x = t_texcoord0s[vi].x;
                            vertexPt.t0y = t_texcoord0s[vi].y;
                        }
                        renderVertexData.append(vertexPt);
                    }
                    SVDataSwapPtr vertexData = MakeSharedPtr<SVDataSwap>();
                    s32 t_len = (s32) (sizeof(V3_N_C_T0) * renderVertexData.size());
                    vertexData->writeData(renderVertexData.get(), t_len);
                    renderMesh->m_vertexCount = renderVertexData.size();
                    renderMesh->m_pRenderVertex = vertexData;
                    //debug
                    SVArray<V3_C> renderDebugData;
                    for (s32 vi = 0; vi < t_postions.size(); vi++) {
                        V3_C vertexPt1;
                        vertexPt1.x = t_postions[vi].x;
                        vertexPt1.y = t_postions[vi].y;
                        vertexPt1.z = t_postions[vi].z;
                        vertexPt1.r = 0;
                        vertexPt1.g = 255;
                        vertexPt1.b = 0;
                        vertexPt1.a = 255;
                        renderDebugData.append(vertexPt1);
                        
                        f32 t_normalX = t_normals[vi].x*0.5;
                        f32 t_normalY = t_normals[vi].y*0.5;
                        f32 t_normalZ = t_normals[vi].z*0.5;
                        V3_C vertexPt2;
                        vertexPt2.x = t_normalX + t_postions[vi].x;
                        vertexPt2.y = t_normalY + t_postions[vi].y;
                        vertexPt2.z = t_normalZ + t_postions[vi].z;
                        vertexPt2.r = 0;
                        vertexPt2.g = 255;
                        vertexPt2.b = 0;
                        vertexPt2.a = 255;
                        renderDebugData.append(vertexPt2);
                    }
                    SVDataSwapPtr vertexDebugData = MakeSharedPtr<SVDataSwap>();
                    s32 t_debugLen = (s32) (sizeof(V3_C) * renderDebugData.size());
                    vertexDebugData->writeData(renderDebugData.get(), t_debugLen);
                    debugRenderMesh->m_vertexCount = renderDebugData.size();
                    debugRenderMesh->m_pRenderVertex = vertexDebugData;
                    break;
                }
            }
        }
    }
    //skins
    
    //
    _loadModelNodeData(_model);
}

void SVGLTF::_loadAnimationData(GLTFModelPtr _model){
    for (s32 i = 0; i<_model->animations.size(); i++) {
        Animation t_animation = _model->animations[i];
        SVGLTFAnimationPtr animation = MakeSharedPtr<SVGLTFAnimation>();
        _model->m_animations.append(animation);
        for (s32 j = 0; j<t_animation.channels.size(); j++) {
            AnimationChannel t_channel = t_animation.channels[j];
            SVGLTFAnimationChannelPtr channel = MakeSharedPtr<SVGLTFAnimationChannel>();
            animation->m_channels.append(channel);
            channel->m_targeNodeIndex = t_channel.target_node;
            channel->m_targetPath = t_channel.target_path;
            AnimationSampler t_sampler = t_animation.samplers[t_channel.sampler];
            SVGLTFAnimationSamplerPtr sampler = MakeSharedPtr<SVGLTFAnimationSampler>();
            channel->m_sampler = sampler;
            sampler->m_inputData = MakeSharedPtr<SVDataSwap>();
            sampler->m_outputData = MakeSharedPtr<SVDataSwap>();
            //input
            Accessor t_inputAccessor = _model->accessors[t_sampler.input];
            BufferView t_inputBufferView = _model->bufferViews[t_inputAccessor.bufferView];
            s32 t_inputByteStride = t_inputBufferView.byteStride;
            s64 t_inputCount = t_inputAccessor.count;
            Buffer t_inputBuffer = _model->buffers[t_inputBufferView.buffer];
            const u8 *t_inputDataAddress = (u8 *)t_inputBuffer.data->getData() + t_inputBufferView.byteOffset +t_inputAccessor.byteOffset;
            sampler->m_inputData->writeData((void *)t_inputDataAddress, t_inputCount*4);
            //output
            Accessor t_outputAccessor = _model->accessors[t_sampler.output];
            BufferView t_outputBufferView = _model->bufferViews[t_outputAccessor.bufferView];
            s32 t_outputByteStride = t_outputBufferView.byteStride;
            s64 t_outputCount = t_outputAccessor.count;
            Buffer t_outputBuffer = _model->buffers[t_outputBufferView.buffer];
            const u8 *t_outputDataAddress = (u8 *)t_outputBuffer.data->getData() + t_outputBufferView.byteOffset +t_outputAccessor.byteOffset;
            sampler->m_outputData->writeData((void *)t_outputDataAddress, t_outputCount*4);
            sampler->m_interpolationMode = _getInterpolationMode(t_sampler.interpolation);
        }
    }
}

void SVGLTF::_loadSkinsData(GLTFModelPtr _model){
    if (!_model) {
        return;
    }
    for (s32 i = 0; i < _model->skins.size(); i++) {
        Skin t_skin = _model->skins[i];
        SVGLTFSkinPtr skin = MakeSharedPtr<SVGLTFSkin>();
        skin->m_skinIndex = i;
        Accessor t_accessor = _model->accessors[t_skin.inverseBindMatrices];
        BufferView t_bufView = _model->bufferViews[t_accessor.bufferView];
        Buffer t_buf = _model->buffers[t_bufView.buffer];
        for (s32 j = 0; j < t_skin.joints.size(); j++) {
            SVGLTFJointPtr joint = MakeSharedPtr<SVGLTFJoint>();
            s32 t_jointIndex = t_skin.joints[j];
            joint->m_jointIndex = t_jointIndex;
            Node t_node = _model->nodes[t_jointIndex];
            //
            FMat4 t_globalJointTransform;
            t_globalJointTransform.setIdentity();
            //translate
            FMat4 matT;
            matT.setIdentity();
            if (t_node.translation.size() > 0) {
                matT.setTranslate(FVec3(t_node.translation[0], t_node.translation[1], t_node.translation[2]));
            }
            //rotation
            FMat4 matR;
            matR.setIdentity();
            if (t_node.rotation.size() > 0) {
                matR.set(SVQuat(FVec4(t_node.rotation[0], t_node.rotation[1], t_node.rotation[2], t_node.rotation[3])));
            }
            //scale
            FMat4 matS;
            matS.setIdentity();
            if (t_node.scale.size() > 0) {
                matS.setScale(FVec3(t_node.scale[0], t_node.scale[1], t_node.scale[2]));
            }
            t_globalJointTransform = matT * matR * matS;
            
            //matrix
            if (t_node.matrix.size() > 0) {
                FVec4 col0(t_node.matrix[0], t_node.matrix[1], t_node.matrix[2], t_node.matrix[3]);
                FVec4 col1(t_node.matrix[4], t_node.matrix[5], t_node.matrix[6], t_node.matrix[7]);
                FVec4 col2(t_node.matrix[8], t_node.matrix[9], t_node.matrix[10], t_node.matrix[11]);
                FVec4 col3(t_node.matrix[12], t_node.matrix[13], t_node.matrix[14], t_node.matrix[15]);
                t_globalJointTransform.setColumn(0, col0);
                t_globalJointTransform.setColumn(1, col1);
                t_globalJointTransform.setColumn(2, col2);
                t_globalJointTransform.setColumn(3, col3);
            }
            joint->m_globalJointTransform = t_globalJointTransform;
            
        }
    }
}

void SVGLTF::_loadModelNodeData(GLTFModelPtr _model){
    if (!_model) {
        return;
    }
    SVArray<s32> sceneNodes = _model->scenes[_model->defaultScene].nodes;
    for (s32 i = 0; i<sceneNodes.size(); i++) {
        s32 nodeIndex = sceneNodes[i];
        Node node = _model->nodes[nodeIndex];
        FMat4 mat;
        mat.setIdentity();
        _refreshMeshGlobalMat(_model, node, mat);
    }
}

void SVGLTF::_refreshMeshGlobalMat(GLTFModelPtr _model, Node _node, FMat4 _mat4){
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
        ModelRenderDataPtr renderData = _model->m_renderMeshData[_node.mesh];
        renderData->m_globalTransform = mat;
        renderData->m_boundBox.setTransform(mat);
        ModelRenderDataPtr debugRenderData = _model->m_renderDebugMeshData[_node.mesh];
        debugRenderData->m_globalTransform = mat;
        debugRenderData->m_boundBox.setTransform(mat);
    }
    
    for (s32 i = 0; i<_node.children.size(); i++) {
        s32 childIndex = _node.children[i];
        Node node = _model->nodes[childIndex];
        _refreshMeshGlobalMat(_model, node, mat);
    }
}
//
SVGLTFScene::SVGLTFScene(){
    
}

SVGLTFScene::~SVGLTFScene(){
    
}
//
SVGLTFMesh::SVGLTFMesh(){
    
}

SVGLTFMesh::~SVGLTFMesh(){
    m_meshes.destroy();
}
//
SVGLTFSubMesh::SVGLTFSubMesh(){
    m_material = nullptr;
    m_indexAccessor = nullptr;
}

SVGLTFSubMesh::~SVGLTFSubMesh(){
    m_material = nullptr;
    m_indexAccessor = nullptr;
    m_accessorsForAttributes.clear();
}
//
SVGLTFAccessor::SVGLTFAccessor(){
    m_bufferData = nullptr;
}

SVGLTFAccessor::~SVGLTFAccessor(){
    m_bufferData = nullptr;
}
//
SVGLTFMaterial::SVGLTFMaterial(){
    m_baseColorTexture = nullptr;
    m_metallicRoughnessTexture = nullptr;
    m_normalTexture = nullptr;
    m_emissiveTexture = nullptr;
    m_occlusionTexture = nullptr;
}

SVGLTFMaterial::~SVGLTFMaterial(){
    m_baseColorTexture = nullptr;
    m_metallicRoughnessTexture = nullptr;
    m_normalTexture = nullptr;
    m_emissiveTexture = nullptr;
    m_occlusionTexture = nullptr;
}
//
SVGLTFNode::SVGLTFNode(){
    
}

SVGLTFNode::~SVGLTFNode(){
    
}
//
SVGLTFSkin::SVGLTFSkin(){
    
}

SVGLTFSkin::~SVGLTFSkin(){
    
}
//
SVGLTFJoint::SVGLTFJoint(){
    
}

SVGLTFJoint::~SVGLTFJoint(){
    
}
//
SVGLTFAnimation::SVGLTFAnimation(){
    
}

SVGLTFAnimation::~SVGLTFAnimation(){
    m_channels.destroy();
    m_samplers.clear();
}

SVGLTFAnimationChannel::SVGLTFAnimationChannel(){
    
}

SVGLTFAnimationChannel::~SVGLTFAnimationChannel(){
    
}

SVGLTFAnimationSampler::SVGLTFAnimationSampler(){
    
}

SVGLTFAnimationSampler::~SVGLTFAnimationSampler(){
    
}

ModelRenderData::ModelRenderData(){
    m_indexCount    = 0;
    m_vertexCount   = 0;
    m_pRenderVertex = nullptr;
    m_pRenderIndex  = nullptr;
    m_pMtl = nullptr;
    m_pMesh = nullptr;
    m_globalTransform.setIdentity();
}

ModelRenderData::~ModelRenderData(){
    m_indexCount    = 0;
    m_vertexCount   = 0;
    m_pRenderVertex = nullptr;
    m_pRenderIndex  = nullptr;
    m_pMtl = nullptr;
    m_pMesh = nullptr;
}
