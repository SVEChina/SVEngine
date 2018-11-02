//
// SVglTF.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVglTF.h"
#include "../file/SVParseDef.h"
#include "../base/SVDataChunk.h"
#include "../file/SVFileMgr.h"
#include "../app/SVInst.h"
SVGLTF::SVGLTF(SVInst *_app)
:SVGBase(_app), bin_data_(nullptr), bin_size_(0), is_binary_(false) {
    
}

SVGLTF::~SVGLTF() {
    
}

bool SVGLTF::loadFromFile(Model *_model, cptr8 _filename){
    // convert json
    SVDataChunk tDataStream;
    bool tflag = mApp->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, _filename);
    if (!tflag)
        return false;
    SV_LOG_INFO("SVglTF :load glTF JSON sucess\n");
    SV_LOG_INFO("SVglTF :filedata %s\n", tDataStream.m_data);
    if (!tDataStream.m_data) {
        SV_LOG_ERROR("SVglTF :data stream is null");
        return false;
    }
    
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_ERROR("SVglTF :rapidjson error code:%d \n", code);
        return false;
    }
    
    if (doc.HasMember("scenes") && doc["scenes"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s scenes object not found in .gltf or not an array type \n", _filename);
        return false;
    }
    
    if (doc.HasMember("nodes") && doc["nodes"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s nodes object not found in .gltf or not an array type \n", _filename);
        return false;
    }
    
    if (doc.HasMember("accessors") && doc["accessors"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s accessors object not found in .gltf or not an array type \n", _filename);
        return false;
    }
    
    if (doc.HasMember("buffers") && doc["buffers"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s buffers object not found in .gltf or not an array type \n", _filename);
        return false;
    }
    
    if (doc.HasMember("bufferViews") && doc["bufferViews"].IsArray()) {
        //OK
    }else{
        SV_LOG_ERROR("SVglTF :%s bufferViews object not found in .gltf or not an array type \n", _filename);
        return false;
    }
    
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
                    if (!_parseBuffer(&buffer, t_bufferItem, nullptr,
                                     is_binary_, bin_data_, bin_size_)) {
                        return false;
                    }
                    _model->buffers.append(buffer);
                }else{
                    SV_LOG_ERROR("SVglTF Error: 'buffers' does not contain an JSON object.");
                    return false;
                }
            }
        }
    }
    
    // 4. Parse BufferView
    {
        if (doc.HasMember("bufferViews") && doc["bufferViews"].IsArray()) {
            RAPIDJSON_NAMESPACE::Value &t_bufferViews = doc["buffers"];
            for (s32 i = 0; i<t_bufferViews.Size(); i++) {
                RAPIDJSON_NAMESPACE::Value &t_bufferViewItem = t_bufferViews[i];
                if (!t_bufferViewItem.IsObject()) {
                    SV_LOG_ERROR("SVglTF Error: 'bufferViews' does not contain an JSON object.");
                    return false;
                }
                BufferView bufferView;
                if (!_parseBufferView(&bufferView, t_bufferViewItem)) {
                    return false;
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
                    return false;
                }
                Accessor accessor;
                if (!_parseAccessor(&accessor, t_accessorItem)) {
                    return false;
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
                    return false;
                }
                Mesh mesh;
                if (!_parseMesh(&mesh, t_meshItem)) {
                    return false;
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
                    return false;
                }
                Node node;
                if (!_parseNode(&node, t_nodeItem)) {
                    return false;
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
                    return false;
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
//                ParseExtensionsProperty(&scene.extensions, err, o);
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
                    return false;
                }
                Material material;
                if (t_material.HasMember("name") && t_material["name"].IsString()) {
                    material.name = t_material["name"].GetString();
                }
                
                if (!_parseMaterial(&material, t_material)) {
                    return false;
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
                    return false;
                }
//                Image image;
//                if (!ParseImage(&image, err, warn, it.value(), base_dir, &fs,
//                                &this->LoadImageData, load_image_user_data_)) {
//                    return false;
//                }
//
//                if (image.bufferView != -1) {
//                    // Load image from the buffer view.
//                    if (size_t(image.bufferView) >= model->bufferViews.size()) {
//                        if (err) {
//                            std::stringstream ss;
//                            ss << "bufferView \"" << image.bufferView
//                            << "\" not found in the scene." << std::endl;
//                            (*err) += ss.str();
//                        }
//                        return false;
//                    }
//
//                    const BufferView &bufferView =
//                    model->bufferViews[size_t(image.bufferView)];
//                    const Buffer &buffer = model->buffers[size_t(bufferView.buffer)];
//
//                    if (*LoadImageData == nullptr) {
//                        if (err) {
//                            (*err) += "No LoadImageData callback specified.\n";
//                        }
//                        return false;
//                    }
//                    bool ret = LoadImageData(&image, err, warn, image.width, image.height,
//                                             &buffer.data[bufferView.byteOffset],
//                                             static_cast<int>(bufferView.byteLength),
//                                             load_image_user_data_);
//                    if (!ret) {
//                        return false;
//                    }
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
                    return false;
                }
                    Texture texture;
                    if (!_parseTexture(&texture, t_textureItem, _filename)) {
                        return false;
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
                    return false;
                }
                Animation animation;
                if (!_parseAnimation(&animation, t_animationItem)) {
                    return false;
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
                    return false;
                }
                Skin skin;
                if (!_parseSkin(&skin, t_skinItem)) {
                    return false;
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
                    return false;
                }
                Sampler sampler;
                if (!_parseSampler(&sampler, t_samplerItem)) {
                    return false;
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
    
    return true;
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

bool SVGLTF::_parseBuffer(Buffer *_buffer, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir, bool _is_binary, const unsigned char *bin_data, s64 bin_size){
    
    u64 byteLength;
    if (_item.HasMember("byteLength") && _item["byteLength"].IsUint64()) {
        byteLength = _item["byteLength"].GetUint64();
    }else{
        return false;
    }
    
    _buffer->uri = "";
    if (_item.HasMember("uri") && _item["uri"].IsString()) {
        _buffer->uri = _item["uri"].GetString();
    }else{
        return false;
    }
    
    if (_is_binary) {
        
    }else{
        if (_isDataURI((_buffer->uri).c_str())) {
            SVString mime_type;
//            if (!_decodeDataURI(_buffer->data, mime_type, (buffer->uri).c_str(), bytes, true)) {
//                SV_LOG_ERROR("SVglTF :Error Failed to decode 'uri'.");
//                return false;
//            }
        }else{
            // Assume external .bin file.
//            if (!LoadExternalFile(&buffer->data, err, /* warn */ nullptr, buffer->uri,
//                                  basedir, true, bytes, true, fs)) {
//                return false;
//            }
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
        buffer = _item["byteOffset"].GetDouble();
    }
    
    f64 byteLength = 1.0;
    if (_item.HasMember("byteLength") && _item["byteLength"].IsNumber()) {
        buffer = _item["byteLength"].GetDouble();
    }
    
    s64 byteStride = 0;
    f64 byteStrideValue = 0.0;
    if (!_item.HasMember("byteStride")) {
        byteStride = 0;
    }else{
        byteStrideValue = _item["byteLength"].GetDouble();
        byteStride = static_cast<s64>(byteStrideValue);
    }
    
    if ((byteStride > 252) || ((byteStride % 4) != 0)) {
        SV_LOG_ERROR("Invalid `byteStride' value. `byteStride' must be the multiple of");
        return false;
    }
    
    f64 target = 0.0;
    if (_item.HasMember("target") && _item["target"].IsNumber()) {
        target = _item["target"].GetDouble();
    }
    s32 targetValue = static_cast<s32>(target);
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
    _bufferView->buffer = static_cast<s32>(buffer);
    _bufferView->byteOffset = static_cast<s64>(byteOffset);
    _bufferView->byteLength = static_cast<s64>(byteLength);
    _bufferView->byteStride = static_cast<s64>(byteStride);
    
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
    
//    ParseExtensionsProperty(&mesh->extensions, err, o);
    return true;
}

bool SVGLTF::_parsePrimitive(Primitive *_primitive, RAPIDJSON_NAMESPACE::Value &_item){
    f64 material = -1.0;
    if (_item.HasMember("material") && _item["material"].IsNumber()) {
        material = _item["material"].GetDouble();
    }
    _primitive->material = static_cast<s32>(material);
    
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
    
    if (_item.HasMember("attributes") && _item["attributes"].IsArray()) {
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
    
//    ParseExtensionsProperty(&node->extensions, err, o);
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
                RAPIDJSON_NAMESPACE::Value &tt_itemVal = iterVal->value;
                Parameter param;
                if (_parseParameterProperty(&param, tt_itemVal, (iterVal->name).GetString())) {
                    _material->values.append((iterVal->name).GetString(), param);
                }
            }
        }else if (strcmp(key, "extensions") == 0 || strcmp(key, "extras") == 0) {
            // done later, skip, otherwise poorly parsed contents will be saved in the
            // parametermap and serialized again later
        } else {
            Parameter param;
            if (_parseParameterProperty(&param, _item, key)) {
                _material->additionalValues.append(key, param);
            }
        }
    }

//    ParseExtensionsProperty(&material->extensions, err, o);
    
    return true;
}

bool SVGLTF::_parseParameterProperty(Parameter *_param, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _prop){
    // A parameter value can either be a string or an array of either a boolean or
    // a number. Booleans of any kind aren't supported here. Granted, it
    // complicates the Parameter structure and breaks it semantically in the sense
    // that the client probably works off the assumption that if the string is
    // empty the vector is used, etc. Would a tagged union work?
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
    
//    ParseExtensionsProperty(&texture->extensions, err, o);
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
//                ParseExtrasProperty(&(sampler.extras), s);
                _animation->samplers.append(sampler);
            }
        }
    }
    
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _animation->name = _item["name"].GetString();
    }
    
//    ParseExtrasProperty(&(animation->extras), o);
    
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
            samplerIndex = _item["node"].GetDouble();
        }
        if (t_targetItem.HasMember("path") && t_targetItem["path"].IsString()) {
            _channel->target_path = _item["path"].GetString();
        }
    }
    
    _channel->sampler = s32(samplerIndex);
    _channel->target_node = s32(targetIndex);
    
//    ParseExtrasProperty(&(channel->extras), o);
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
    
//    ParseExtrasProperty(&(sampler->extras), o);
    
    return true;
}

bool SVGLTF::_loadExternalFile(SVString &_outData, cptr8 _filename, cptr8 _basedir, bool _required, s64 _reqBytes, bool _checkSize){

//    std::vector<std::string> paths;
//    paths.push_back(_basedir);
//    paths.push_back(".");
//
//
//    std::vector<unsigned char> buf;
//    std::string fileReadErr;
//    bool fileRead =
//    fs->ReadWholeFile(&buf, &fileReadErr, filepath, fs->user_data);
//    if (!fileRead) {
//        if (failMsgOut) {
//            (*failMsgOut) +=
//            "File read error : " + filepath + " : " + fileReadErr + "\n";
//        }
//        return false;
//    }
//
//    size_t sz = buf.size();
//    if (sz == 0) {
//        if (failMsgOut) {
//            (*failMsgOut) += "File is empty : " + filepath + "\n";
//        }
//        return false;
//    }
//
//    if (checkSize) {
//        if (reqBytes == sz) {
//            out->swap(buf);
//            return true;
//        } else {
//            std::stringstream ss;
//            ss << "File size mismatch : " << filepath << ", requestedBytes "
//            << reqBytes << ", but got " << sz << std::endl;
//            if (failMsgOut) {
//                (*failMsgOut) += ss.str();
//            }
//            return false;
//        }
//    }
//
//    out->swap(buf);
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
    std::string ret;
    
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
