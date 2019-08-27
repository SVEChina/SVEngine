//
// SVLoaderGLTF.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVLoaderGLTF.h"
#include "SVFileMgr.h"
#include "../app/SVInst.h"
#include "../base/SVDataChunk.h"
#include "../base/SVDataSwap.h"
#include "../base/SVQuat.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtl3D.h"
#include "../mtl/SVMtlGLTF.h"
#include "../file/SVParseDef.h"
#include "../file/SVFileMgr.h"
#include "../rendercore/SVRenderMesh.h"
#include "../node/SVSkinNode.h"
#include "../node/SVModelNode.h"
#include "../node/SVMorphNode.h"
#include "../node/SVScene.h"
#include "../basesys/SVSceneMgr.h"
#include "../core/SVModel.h"
#include "../core/SVMesh.h"
#include "../core/SVModel.h"
#include "../core/SVAnimateSkin.h"
#include "../core/SVAnimateMorph.h"
#include "../base/SVMap.h"
#include "../base/SVArray.h"
#include "../base/svstr.h"
#include "../basesys/SVModelMgr.h"
#include "../rendercore/SVResVBO.h"

SVLoaderGLTF::SVLoaderGLTF(SVInst *_app)
:SVGBase(_app) {
}

SVLoaderGLTF::~SVLoaderGLTF() {
    m_nodeArray.clear();
}

bool SVLoaderGLTF::loadFromFile(cptr8 _filename){
    SVDataChunk tDataStream;
    bool tflag = mApp->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, _filename);
    if (!tflag)
        return false;
    SV_LOG_INFO("SVLoaderGLTF :load glTF JSON sucess\n");
    SV_LOG_INFO("SVLoaderGLTF :filedata %s\n", tDataStream.m_data);
    if (!tDataStream.m_data) {
        SV_LOG_ERROR("SVLoaderGLTF :data stream is null");
        return false;
    }
    RAPIDJSON_NAMESPACE::Document doc;
    doc.Parse(tDataStream.m_data);
    if (doc.HasParseError()) {
        RAPIDJSON_NAMESPACE::ParseErrorCode code = doc.GetParseError();
        SV_LOG_ERROR("SVLoaderGLTF :rapidjson error code:%d \n", code);
        return false;
    }
    if (!doc.HasMember("scenes"))
        return false;
    if (!doc["scenes"].IsArray())
        return false;
    if (!doc.HasMember("nodes"))
        return false;
    if (!doc["nodes"].IsArray())
        return false;
    if (!doc.HasMember("accessors"))
        return false;
    if (!doc["accessors"].IsArray())
        return false;
    if (!doc.HasMember("buffers"))
        return false;
    if (!doc["buffers"].IsArray())
        return false;
    if (!doc.HasMember("bufferViews"))
        return false;
    if (!doc["bufferViews"].IsArray())
        return false;
    //
    m_gltf.buffers.clear();
    m_gltf.bufferViews.clear();
    m_gltf.accessors.clear();
    m_gltf.meshes.clear();
    m_gltf.cameras.clear();
    m_gltf.nodes.clear();
    m_gltf.extensionsUsed.clear();
    m_gltf.extensionsRequired.clear();
    m_gltf.defaultScene = -1;
    /*1. Parse Asset*/
    if (doc.HasMember("asset") && doc["asset"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &item = doc["asset"];
        _parseAsset(&(m_gltf.asset), item);
    }
    /*2. Parse extensionUsed*/
    if (doc.HasMember("extensionsUsed") && doc["extensionsUsed"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_extensionsUseds = doc["extensionsUsed"];
        for (s32 i = 0; i < t_extensionsUseds.Size(); ++i) {
            RAPIDJSON_NAMESPACE::Value &t_extensionsUsedItem = t_extensionsUseds[i];
            m_gltf.extensionsUsed.append(t_extensionsUsedItem.GetString());
        }
    }
    if (doc.HasMember("extensionsRequired") && doc["extensionsRequired"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_extensionsRequireds = doc["extensionsRequired"];
        for (s32 i = 0; i < t_extensionsRequireds.Size(); ++i) {
            RAPIDJSON_NAMESPACE::Value &t_extensionsRequiredItem = t_extensionsRequireds[i];
            m_gltf.extensionsRequired.append(t_extensionsRequiredItem.GetString());
        }
    }
    /*3. Parse Buffer*/
    if (doc.HasMember("buffers") && doc["buffers"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_buffers = doc["buffers"];
        for (s32 i = 0; i<t_buffers.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_bufferItem = t_buffers[i];
            if (t_bufferItem.IsObject()) {
                Buffer buffer;
                if (!_parseBuffer(&buffer, t_bufferItem, _filename)) {
                    return nullptr;
                }
                m_gltf.buffers.append(buffer);
            }else{
                SV_LOG_ERROR("SVLoaderGLTF Error: 'buffers' does not contain an JSON object.");
                return nullptr;
            }
        }
    }
    /*4. Parse BufferView*/
    if (doc.HasMember("bufferViews") && doc["bufferViews"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_bufferViews = doc["bufferViews"];
        for (s32 i = 0; i<t_bufferViews.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_bufferViewItem = t_bufferViews[i];
            if (!t_bufferViewItem.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error: 'bufferViews' does not contain an JSON object.");
                return nullptr;
            }
            BufferView bufferView;
            if (!_parseBufferView(&bufferView, t_bufferViewItem)) {
                return nullptr;
            }
            m_gltf.bufferViews.append(bufferView);
        }
    }
    /*5. Parse Accessor*/
    if (doc.HasMember("accessors") && doc["accessors"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_accessors = doc["accessors"];
        for (s32 i=0; i<t_accessors.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_accessorItem = t_accessors[i];
            if (!t_accessorItem.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error: `accessors' does not contain an JSON object.");
                return nullptr;
            }
            Accessor accessor;
            if (!_parseAccessor(&accessor, t_accessorItem)) {
                return nullptr;
            }
            m_gltf.accessors.append(accessor);
        }
    }
    /*6. Parse Mesh*/
    if (doc.HasMember("meshes") && doc["meshes"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_meshes = doc["meshes"];
        for (s32 i=0; i<t_meshes.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_meshItem = t_meshes[i];
            if (!t_meshItem.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error:`meshes' does not contain an JSON object.");
                return nullptr;
            }
            Mesh mesh;
            if (!_parseMesh(&mesh, t_meshItem)) {
                return nullptr;
            }
            m_gltf.meshes.append(mesh);
        }
    }
    /*7. Parse Node*/
    if (doc.HasMember("nodes") && doc["nodes"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_nodes = doc["nodes"];
        for (s32 i=0; i<t_nodes.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_nodeItem = t_nodes[i];
            if (!t_nodeItem.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error:`nodes' does not contain an JSON object.");
                return nullptr;
            }
            Node node;
            if (!_parseNode(&node, t_nodeItem)) {
                return nullptr;
            }
            m_gltf.nodes.append(node);
        }
    }
    /*8. Parse scenes.*/
    if (doc.HasMember("scenes") && doc["scenes"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_scenes = doc["scenes"];
        for (s32 i =0; i<t_scenes.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_sceneItem = t_scenes[i];
            if (!t_sceneItem.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error :`scenes' does not contain an JSON object.");
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
            m_gltf.scenes.append(scene);
        }
    }
    /*9. Parse default scenes.*/
    if (doc.HasMember("scene") && doc["scene"].IsNumber()) {
        RAPIDJSON_NAMESPACE::Value &t_scene = doc["scene"];
        const s32 defaultScene = t_scene.GetInt();
        m_gltf.defaultScene = defaultScene;
    }
    /*10. Parse Material*/
    if (doc.HasMember("materials") && doc["materials"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_materials = doc["materials"];
        for (s32 i =0; i<t_materials.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_material = t_materials[i];
            if (!t_material.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error :'materials' does not contain an JSON object.");
                return nullptr;
            }
            Material material;
            if (t_material.HasMember("name") && t_material["name"].IsString()) {
                material.name = t_material["name"].GetString();
            }
            if (!_parseMaterial(&material, t_material)) {
                return nullptr;
            }
            m_gltf.materials.append(material);
        }
    }
    /*11. Parse Image*/
    if (doc.HasMember("images") && doc["images"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_images = doc["images"];
        for (s32 i =0; i<t_images.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_image = t_images[i];
            if (!t_image.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error :'images' does not contain an JSON object.");
                return nullptr;
            }
            Image image;
            if (!_parseImage(&image, t_image, _filename)) {
                return nullptr;
            }
            if (image.bufferView != -1) {
                // Load image from the buffer view.
                
                // not supporte now!!!!!
                SV_LOG_ERROR("SVLoaderGLTF Error :read image from buffer does not supporte now.");
                return nullptr;
            }
            m_gltf.images.append(image);
        }
    }
    /*12. Parse Texture*/
    if (doc.HasMember("textures") && doc["textures"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_textures = doc["textures"];
        for (s32 i=0; i<t_textures.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_textureItem = t_textures[i];
            if (!t_textureItem.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error :'textures' does not contain an JSON object.");
                return nullptr;
            }
            Texture texture;
            if (!_parseTexture(&texture, t_textureItem, _filename)) {
                return nullptr;
            }
            m_gltf.textures.append(texture);
        }
    }
    /*13. Parse Animation*/
    if (doc.HasMember("animations") && doc["animations"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_animations = doc["animations"];
        for (s32 i=0; i<t_animations.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_animationItem = t_animations[i];
            if (!t_animationItem.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error :'animations' does not contain an JSON object.");
                return nullptr;
            }
            Animation animation;
            if (!_parseAnimation(&animation, t_animationItem)) {
                return nullptr;
            }
            m_gltf.animations.append(animation);
        }
    }
    /*14. Parse Skin*/
    if (doc.HasMember("skins") && doc["skins"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_skins = doc["skins"];
        for (s32 i=0; i<t_skins.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_skinItem = t_skins[i];
            if (!t_skinItem.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error :'skins' does not contain an JSON object.");
                return nullptr;
            }
            Skin skin;
            skin.skeleton = i;
            if (!_parseSkin(&skin, t_skinItem)) {
                return nullptr;
            }
            m_gltf.skins.append(skin);
        }
    }
    /*15. Parse Sampler*/
    if (doc.HasMember("samplers") && doc["samplers"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_samplers = doc["samplers"];
        for (s32 i=0; i<t_samplers.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_samplerItem = t_samplers[i];
            if (!t_samplerItem.IsObject()) {
                SV_LOG_ERROR("SVLoaderGLTF Error :'samplers' does not contain an JSON object.");
                return nullptr;
            }
            Sampler sampler;
            if (!_parseSampler(&sampler, t_samplerItem)) {
                return nullptr;
            }
            m_gltf.samplers.append(sampler);
        }
    }
    // 16. Parse Camera
//    if (doc.HasMember("cameras") && doc["cameras"].IsArray()) {
//        RAPIDJSON_NAMESPACE::Value &t_cameras = doc["cameras"];
//        for (s32 i=0; i<t_cameras.Size(); i++) {
//            RAPIDJSON_NAMESPACE::Value &t_cameraItem = t_cameras[i];
//            if (!t_cameraItem.IsObject()) {
//                SV_LOG_ERROR("SVLoaderGLTF Error :'cameras' does not contain an JSON object.");
//                return false;
//            }
//            Camera camera;
//            if (!ParseCamera(&camera, err, it->get<json>())) {
//                return false;
//            }
//
//            model->cameras.push_back(camera);
//        }
//    }
    return true;
}

//
void SVLoaderGLTF::building() {
    SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
    if(!t_sc)
        return ;
    //构建节点
    for(s32 i=0;i<m_gltf.scenes.size();i++) {
        Scene* t_scene = &(m_gltf.scenes[i]);
        for(s32 j=0;j<t_scene->nodes.size();j++) {
            s32 t_index = t_scene->nodes[j];
            Node* t_node = &(m_gltf.nodes[t_index]);
            if(t_node->skin>=0) {
                //有骨骼动画
                SVNodePtr t_svNode = _buildSkinNode(t_node);
                if(t_svNode) {
                    t_sc->addNode(t_svNode);
                    m_nodeArray.append(t_svNode);
                }
            }else if(t_node->mesh>=0) {
                //无骨骼动画 纯mesh
                SVNodePtr t_svNode = _build3DNode(t_node);
                if(t_svNode) {
                    m_nodeArray.append(t_svNode);
                }
            }else if(t_node->camera>=0) {
                //相机
                SVNodePtr t_svNode = _buildCameraNode(t_node);
                if(t_svNode) {
                    //t_sc->addNode(t_svNode);
                    m_nodeArray.append(t_svNode);
                }
            }
        }
    }
}

SVNodePtr SVLoaderGLTF::getNode(cptr8 _nodename)  {
    for(s32 i=0;i<m_nodeArray.size();i++)  {
        if( strcmp(m_nodeArray[i]->getname(),_nodename) == 0 ) {
            return m_nodeArray[i];
        }
    }
    return nullptr;
}

//构建各种节点
SVNodePtr SVLoaderGLTF::_buildSkinNode(Node* _node) {
    SVSkinNodePtr tNode = MakeSharedPtr<SVSkinNode>(mApp);
    tNode->setname(_node->name.c_str());
    if(_node->translation.size() == 3) {
        tNode->m_filePos.x = _node->translation[0];
        tNode->m_filePos.y = _node->translation[1];
        tNode->m_filePos.z = _node->translation[2];
    }
    if(_node->scale.size() == 3) {
        tNode->m_fileScale.x = _node->scale[0];
        tNode->m_fileScale.y = _node->scale[1];
        tNode->m_fileScale.z = _node->scale[2];
    }
    if(_node->rotation.size() == 4) {
        tNode->m_fileRot.x = _node->rotation[0];
        tNode->m_fileRot.y = _node->rotation[1];
        tNode->m_fileRot.z = _node->rotation[2];
        tNode->m_fileRot.w = _node->rotation[3];
    }
    //构建model
    SVModelPtr t_model = _buildModel(_node->mesh);
    tNode->setModel(t_model);
    //构建皮肤
    SVSkeletonPtr t_skin = _buildSkin(_node->skin);
    tNode->setSke(t_skin);
    //构建动画
    //好多动画的 好不好
    for(s32 i=0;i<m_gltf.animations.size();i++){
        SVAnimateSkinPtr t_ani = _buildAnimate(i);
        if(t_ani) {
            tNode->addAni(t_ani);
        }
    }
    return tNode;
}

SVNodePtr SVLoaderGLTF::_build3DNode(Node* _node) {
    SVModelNodePtr tNode = MakeSharedPtr<SVModelNode>(mApp);
    tNode->setname(_node->name.c_str());
    //构建model
    SVModelPtr t_model = _buildModel(_node->mesh);
    if(t_model) {
        tNode->setModel(t_model);
    }
    return tNode;
}

SVNodePtr SVLoaderGLTF::_buildCameraNode(Node* _node) {
    return nullptr;
}

//给出索引 构建皮肤
SVSkeletonPtr SVLoaderGLTF::_buildSkin(s32 _skinIndex){
    if(_skinIndex<0)
        return nullptr;
    Skin* t_skindata = &(m_gltf.skins[_skinIndex]);
    //构建骨架
    SVSkeletonPtr t_ske = MakeSharedPtr<SVSkeleton>();
    s32 t_node_index = t_skindata->skeleton;
    //创建根骨骼,并构建骨架
    SVBonePtr t_rootBone = MakeSharedPtr<SVBone>();
    _buildBone(t_rootBone,t_skindata,t_node_index,t_ske);
    t_ske->m_name = t_skindata->name;
    t_ske->m_root = t_rootBone;
    //初始化骨架基本数据
    Accessor* t_acc = &(m_gltf.accessors[t_skindata->inverseBindMatrices]);
    //
    s8* t_imb_p = _getAccDataPointer(t_acc);
    f32* t_imb_p_f = (f32*)t_imb_p;
    for(s32 i=0;i<t_skindata->joints.size();i++) {
        s32 t_nodeid = t_skindata->joints[i];
        SVBonePtr t_bone = t_ske->getBoneByNodeID(t_nodeid);//获取对应的骨骼
        if(t_bone) {
            t_bone->m_id = i;
            f32* t_imb_p_f_aim = t_imb_p_f+i*16;
            t_bone->m_invertBindMat.set(t_imb_p_f_aim);
        }
    }
    return t_ske;
}

bool SVLoaderGLTF::_buildBone(SVBonePtr _parent,Skin* _skinData,s32 _nodeIndex,SVSkeletonPtr _ske) {
    //
    if(_nodeIndex<0)
        return false;
    //填充数据
    Node* t_node = &(m_gltf.nodes[_nodeIndex]);
    if(!t_node){
        return false;
    }
    //
    _ske->addBone(_parent);
    for(s32 i=0;i<_skinData->joints.size();i++) {
        if( _skinData->joints[i] == _nodeIndex) {
            _parent->m_id = i;
        }
    }
    _parent->m_nodeid = _nodeIndex;
    _parent->m_name = t_node->name; // 骨头名称
    _parent->m_tran.x = t_node->translation[0];
    _parent->m_tran.y = t_node->translation[1];
    _parent->m_tran.z = t_node->translation[2];
    _parent->m_scale.x = t_node->scale[0];
    _parent->m_scale.y = t_node->scale[1];
    _parent->m_scale.z = t_node->scale[2];
    _parent->m_rot.x = t_node->rotation[0];
    _parent->m_rot.y = t_node->rotation[1];
    _parent->m_rot.z = t_node->rotation[2];
    _parent->m_rot.w = t_node->rotation[3];
    //构建子骨骼
    for(s32 i=0;i<t_node->children.size();i++) {
        s32 t_nodeIndex = t_node->children[i];
        SVBonePtr t_bone = MakeSharedPtr<SVBone>();
        t_bone->m_pParent = _parent;
        _buildBone(t_bone,_skinData,t_nodeIndex,_ske);
        _parent->m_children.append(t_bone);
    }
    return true;
}

SVAnimateSkinPtr SVLoaderGLTF::_buildAnimate(s32 _index){
    if(_index<0)
        return nullptr;
    Animation* t_anidata = &(m_gltf.animations[_index]);
    SVAnimateSkinPtr t_ani = MakeSharedPtr<SVAnimateSkin>(mApp,t_anidata->name.c_str());
    //构建轨道
    for(s32 i=0;i<t_anidata->channels.size();i++) {
        AnimationChannel* t_chn_data = &(t_anidata->channels[i]);
        //构建input
        if(t_chn_data->sampler<0) {
            continue;
        }
        //构建输入
        AnimationSampler* t_chn_samp = &(t_anidata->samplers[t_chn_data->sampler]);
        SVChannelPtr t_sve_chn =  t_ani->getChannel(t_chn_data->target_node);
        if(!t_sve_chn) {
            t_sve_chn = MakeSharedPtr<SVChannel>();
            t_sve_chn->m_target = t_chn_data->target_node;
            Accessor* t_acc = &(m_gltf.accessors[t_chn_samp->input]);
            if( t_acc->name == "accessorAnimationInput" ) {
                s8* t_p = _getAccDataPointer(t_acc);
                f32* t_pTime = (f32*)t_p;
                for(s32 i=0;i<t_acc->count;i++) {
                    SVASKeyPtr t_askey = MakeSharedPtr<SVASKey>();
                    t_askey->m_time = *t_pTime;
                    t_pTime++;
                    t_sve_chn->m_keyPool.append(t_askey);//推入key
                }
                t_sve_chn->m_maxTime = t_acc->maxValues[0];
                t_sve_chn->m_minTime = t_acc->minValues[0];
            }
            t_ani->addChannel(t_sve_chn);
        }
        //构建输出
        if( t_chn_data->target_path == "translation") {
            //构建trans
            t_sve_chn->m_intertype_trans = _getInterpolationMode(t_chn_samp->interpolation);
            Accessor* t_acc = &(m_gltf.accessors[t_chn_samp->output]);
            if( t_acc->name == "accessorAnimationPositions" ) {
                if( t_acc->count == t_sve_chn->m_keyPool.size() ) {
                    s8* t_p = _getAccDataPointer(t_acc);
                    f32* t_pPos = (f32*)t_p;
                    for(s32 i=0;i<t_acc->count;i++) {
                        SVASKeyPtr t_askey = t_sve_chn->m_keyPool[i];
                        t_askey->m_pos.x = *t_pPos;
                        t_pPos++;
                        t_askey->m_pos.y = *t_pPos;
                        t_pPos++;
                        t_askey->m_pos.z = *t_pPos;
                        t_pPos++;
                    }
                }
            }
        }else if( t_chn_data->target_path == "rotation") {
            //构建rot
            t_sve_chn->m_intertype_rot = _getInterpolationMode(t_chn_samp->interpolation);
            Accessor* t_acc = &(m_gltf.accessors[t_chn_samp->output]);
            if( t_acc->name == "accessorAnimationRotations" ) {
                if( t_acc->count == t_sve_chn->m_keyPool.size() ) {
                    s8* t_p = _getAccDataPointer(t_acc);
                    f32* t_pRot = (f32*)t_p;
                    for(s32 i=0;i<t_acc->count;i++) {
                        SVASKeyPtr t_askey = t_sve_chn->m_keyPool[i];
                        t_askey->m_rot.x = *t_pRot;
                        t_pRot++;
                        t_askey->m_rot.y = *t_pRot;
                        t_pRot++;
                        t_askey->m_rot.z = *t_pRot;
                        t_pRot++;
                        t_askey->m_rot.w = *t_pRot;
                        t_pRot++;
                    }
                }
            }
        }else if( t_chn_data->target_path == "scale") {
            //构建scale
            t_sve_chn->m_intertype_scale = _getInterpolationMode(t_chn_samp->interpolation);
            Accessor* t_acc = &(m_gltf.accessors[t_chn_samp->output]);
            if( t_acc->name == "accessorAnimationScales" ) {
                if( t_acc->count == t_sve_chn->m_keyPool.size() ) {
                    s8* t_p = _getAccDataPointer(t_acc);
                    f32* t_pScale = (f32*)t_p;
                    for(s32 i=0;i<t_acc->count;i++) {
                        SVASKeyPtr t_askey = t_sve_chn->m_keyPool[i];
                        t_askey->m_scale.x = *t_pScale;
                        t_pScale++;
                        t_askey->m_scale.y = *t_pScale;
                        t_pScale++;
                        t_askey->m_scale.z = *t_pScale;
                        t_pScale++;
                    }
                }
            }
        }else if( t_chn_data->target_path == "weights") {
            //构建weight
//            t_sve_chn->m_intertype_weight = _getInterpolationMode(t_chn_samp->interpolation);
//            Accessor* t_acc = &(m_gltf.accessors[t_chn_samp->input]);
//            if( t_acc->name == "accessorAnimationPositions" ) {
//
//            }
        }
    }
    return t_ani;
}

//
SVModelPtr SVLoaderGLTF::_buildModel(s32 _index){
    if(_index<0)
        return nullptr;
    //这里的pri对应mesh
    //这里mesh对应的是model
    Mesh* t_meshdata = &(m_gltf.meshes[_index]);
    SVString t_modelName = t_meshdata->name.c_str();
    SVModelMgrPtr t_modelMgr = mApp->getModelMgr();
    if (t_modelMgr) {
        SVModelPtr t_model = t_modelMgr->getModel(t_modelName);
        if (t_model) {
            return t_model;
        }
    }
    SVModelPtr t_model = MakeSharedPtr<SVModel>(mApp);
    t_model->setName(t_modelName.c_str());
    //非权重方式
    for(s32 i=0;i<t_meshdata->primitives.size();i++) {
        Primitive* t_pri = &(t_meshdata->primitives[i]);
        SVMeshPtr t_mesh = _buildMeshPri(t_pri);
        if(t_mesh) {
            t_model->addMesh(t_mesh);
        }
    }
    if (t_modelMgr) {
        t_modelMgr->addModel(t_model);
    }
    return t_model;
}

s8* SVLoaderGLTF::_getAccDataPointer(Accessor* acc) {
    s32 t_acc_off = acc->byteOffset;
    s32 t_viewID = acc->bufferView;
    BufferView* t_bufview = &(m_gltf.bufferViews[t_viewID]);
    if(t_bufview) {
        s32 t_bufID = t_bufview->buffer;
        s32 t_view_off = t_bufview->byteOffset;
        s32 t_len = t_bufview->byteLength;
        s32 t_view_stride = t_bufview->byteStride;
        Buffer* t_buf = &(m_gltf.buffers[t_bufID]);
        t_buf->data->lockData();
        s8* p = (s8*)(t_buf->data->getData());
        p += t_view_off;
        p += t_acc_off;
        t_buf->data->unlockData();
        return p;
    }
    return nullptr;
}

SVMeshPtr SVLoaderGLTF::_buildMeshPri(Primitive* _prim) {
    //构建数据
    Accessor* accV3 = nullptr;
    Accessor* accNOR = nullptr;
    Accessor* accTAG = nullptr;
    Accessor* accC0 = nullptr;
    Accessor* accT0 = nullptr;
    Accessor* accB = nullptr;
    Accessor* accW = nullptr;
    s8* pAccV3 = nullptr;
    s8* pAccNOR = nullptr;
    s8* pAccTAG = nullptr;
    s8* pAccC0 = nullptr;
    s8* pAccT0 = nullptr;
    s8* pAccB = nullptr;
    s8* pAccW = nullptr;
    //
    s32 t_vtf = E_VF_BASE;
    SVMap<SVString, s32>::Iterator it= _prim->attributes.begin();
    while( it!=_prim->attributes.end() ) {
        if(it->key == "POSITION" ) {
            t_vtf |= D_VF_V3;
            s32 t_index = it->data;
            accV3 = &(m_gltf.accessors[t_index]);
            pAccV3 = _getAccDataPointer(accV3);
        }else if(it->key == "NORMAL" ) {
            //
            t_vtf |= D_VF_NOR;
            s32 t_index = it->data;
            accNOR = &(m_gltf.accessors[t_index]);
            pAccNOR = _getAccDataPointer(accNOR);
        }else if(it->key == "TANGENT" ) {
            //
            t_vtf |= D_VF_TAG;
            s32 t_index = it->data;
            accTAG = &(m_gltf.accessors[t_index]);
            pAccTAG = _getAccDataPointer(accTAG);
        }else if(it->key == "COLOR_0" ) {
            //
            t_vtf |= D_VF_C0;
            s32 t_index = it->data;
            accC0 = &(m_gltf.accessors[t_index]);
            pAccC0 = _getAccDataPointer(accC0);
        }else if(it->key == "TEXCOORD_0" ) {
            //
            t_vtf |= D_VF_T0;
            s32 t_index = it->data;
            accT0 = &(m_gltf.accessors[t_index]);
            pAccT0 = _getAccDataPointer(accT0);
        }else if(it->key == "JOINTS_0" ) {
            //
            t_vtf |= D_VF_BONE;
            s32 t_index = it->data;
            accB = &(m_gltf.accessors[t_index]);
            pAccB = _getAccDataPointer(accB);
        }else if(it->key == "WEIGHTS_0" ) {
            //
            t_vtf |= D_VF_BONE_W;
            s32 t_index = it->data;
            accW = &(m_gltf.accessors[t_index]);
            pAccW = _getAccDataPointer(accW);
        }
        it++;
    }
    //
    SVDataSwapPtr t_data = MakeSharedPtr<SVDataSwap>();
    if(!accV3){
        //没有pos直接返回算了
        return nullptr;
    }
    s32 t_verSize = SVResVBO::getVertexFormateSize(VFTYPE(t_vtf));
    SVMeshPtr t_mesh = MakeSharedPtr<SVMesh>(mApp);
    //设置box
    s64 t_count = accV3->count;
    if(accV3->minValues.size() == 3  && accV3->maxValues.size()==3) {
        SVBoundBox t_box;
        t_box.set(FVec3(accV3->minValues[0],accV3->minValues[1],accV3->minValues[2]),
                  FVec3(accV3->maxValues[0],accV3->maxValues[1],accV3->maxValues[2]));
        t_mesh->setBox(t_box);
    }
    //拼接数据,为了优化
    t_data->resize(t_verSize*t_count);
    for(s32 i=0;i<t_count;i++) {
        if (t_vtf & D_VF_V3) {
            t_data->appendData(pAccV3, 3*sizeof(f32));
            pAccV3 += 3*sizeof(f32);
        }
        if (t_vtf & D_VF_NOR) {
            t_data->appendData(pAccNOR, 3*sizeof(f32));
            pAccNOR += 3*sizeof(f32);
        }
        if (t_vtf & D_VF_TAG) {
            t_data->appendData(pAccTAG, 3*sizeof(f32));
            pAccTAG += 3*sizeof(f32);
        }
        if (t_vtf & D_VF_C0) {
            f32* t_p = (f32*)pAccC0;
            f32 t_r = *t_p;t_p++;
            f32 t_g = *t_p;t_p++;
            f32 t_b = *t_p;t_p++;
            f32 t_a = *t_p;
            s8 c_r = t_r*255;
            s8 c_g = t_g*255;
            s8 c_b = t_b*255;
            s8 c_a = t_a*255;
            t_data->appendData(&c_r, sizeof(u8));
            t_data->appendData(&c_g, sizeof(u8));
            t_data->appendData(&c_b, sizeof(u8));
            t_data->appendData(&c_a, sizeof(u8));
            pAccC0 += 4*sizeof(f32);
        }
        if (t_vtf & D_VF_T0) {
            t_data->appendData(pAccT0, 2*sizeof(f32));
            pAccT0 += 2*sizeof(f32);
        }
        if (t_vtf & D_VF_BONE) {
            t_data->appendData(pAccB, 4*sizeof(u16));
            pAccB += 4*sizeof(u16);
        }
        if (t_vtf & D_VF_BONE_W) {
            t_data->appendData(pAccW, 4*sizeof(f32));
            pAccW += 4*sizeof(f32);
        }
    }
    //
    SVRenderMeshPtr t_rMesh = MakeSharedPtr<SVRenderMesh>(mApp);
    t_rMesh->setVertexType(VFTYPE(t_vtf));
    t_rMesh->setSeqMode(1);
    t_rMesh->setVertexDataNum(t_count);
    t_rMesh->setVertexData(t_data);
    //索引数据
    if( _prim->indices >=0) {
        Accessor* accIndex = &(m_gltf.accessors[_prim->indices]);
        SVDataSwapPtr pIndex = MakeSharedPtr<SVDataSwap>();
        _fetchDataFromAcc(pIndex,accIndex);
        t_rMesh->setIndexData(pIndex,accIndex->count);
    }
    //绘制模式
    if(_prim->mode == SVGLTF_MODE_POINTS) {
        t_rMesh->setDrawMethod(E_DM_POINTS);
    }else if(_prim->mode == SVGLTF_MODE_LINE) {
        t_rMesh->setDrawMethod(E_DM_LINES);
    }else if(_prim->mode == SVGLTF_MODE_LINE_LOOP) {
        t_rMesh->setDrawMethod(E_DM_LINE_LOOP);
    }else if(_prim->mode == SVGLTF_MODE_TRIANGLES) {
        t_rMesh->setDrawMethod(E_DM_TRIANGLES);
    }else if(_prim->mode == SVGLTF_MODE_TRIANGLE_STRIP) {
        t_rMesh->setDrawMethod(E_DM_TRIANGLE_STRIP);
    }else if(_prim->mode == SVGLTF_MODE_TRIANGLE_FAN) {
        t_rMesh->setDrawMethod(E_DM_TRIANGLE_FAN);
    }
    t_rMesh->createMesh();
    t_mesh->setRenderMesh(t_rMesh);
    //材质
    SVMtlCorePtr t_mtl = _buildMtl(_prim,t_vtf);
    if(t_mtl) {
        t_mesh->setMtl(t_mtl);
    }
    //返回目标mesh
    return t_mesh;
}

SVMtlCorePtr SVLoaderGLTF::_buildMtl(Primitive* _prim,s32 _vtf) {
    if(!_prim)
        return nullptr;
    if(_prim->material<0)
        return nullptr;
    //构建材质
    s32 _index = _prim->material;
    Material* t_mtl = &(m_gltf.materials[_index]);
    SVMtlGLTFPtr tMtl = nullptr;
    if (_vtf & D_VF_BONE) {
        tMtl = MakeSharedPtr<SVMtlGLTFSkin>(mApp);
    }else{
        tMtl = MakeSharedPtr<SVMtlGLTF>(mApp);
    }
    //
    ParameterMap::Iterator it1 = t_mtl->values.begin();
    while (it1!=t_mtl->values.end()) {
        SVString t_key = it1->key;
        if(t_key == "baseColorTexture") {
            Parameter* t_param = &(it1->data);
            if(t_param) {
                s32 textureIndex = t_param->TextureIndex();
                if(textureIndex>=0) {
                    Texture* texture = &(m_gltf.textures[textureIndex]);
                    Image* image = &(m_gltf.images[texture->source]);
                    tMtl->m_pBaseColorTex = image->texture;
                    
                    Sampler* t_sampler = &( m_gltf.samplers[texture->sampler] );
                    //
                    if( t_sampler->minFilter == SVGLTF_TEXTURE_FILTER_NEAREST) {
                        
                    }else if( t_sampler->minFilter == SVGLTF_TEXTURE_FILTER_LINEAR) {
                        
                    }else if( t_sampler->minFilter == SVGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST) {
                        
                    }else if( t_sampler->minFilter == SVGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST) {
                        
                    }else if( t_sampler->minFilter == SVGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR) {
                        
                    }else if( t_sampler->minFilter == SVGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR) {
                        
                    }
                    //
                    if( t_sampler->magFilter == SVGLTF_TEXTURE_FILTER_NEAREST) {
                        
                    }else if( t_sampler->magFilter == SVGLTF_TEXTURE_FILTER_LINEAR) {
                        
                    }else if( t_sampler->magFilter == SVGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST) {
                        
                    }else if( t_sampler->magFilter == SVGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST) {
                        
                    }else if( t_sampler->magFilter == SVGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR) {
                        
                    }else if( t_sampler->magFilter == SVGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR) {
                        
                    }
                    //
                    if( t_sampler->wrapS == SVGLTF_TEXTURE_WRAP_REPEAT ) {
                        tMtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_REPEAT);
                    }else if( t_sampler->wrapS == SVGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE ) {
                        tMtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_CLAMP_TO_EDAGE);
                    }else if( t_sampler->wrapS == SVGLTF_TEXTURE_WRAP_MIRRORED_REPEAT ) {
                        tMtl->setTextureParam(0, E_T_PARAM_WRAP_S, E_T_WRAP_MIRROR);
                    }
                    //
                    if( t_sampler->wrapT == SVGLTF_TEXTURE_WRAP_REPEAT ) {
                        tMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
                    }else if( t_sampler->wrapT == SVGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE ) {
                        tMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_CLAMP_TO_EDAGE);
                    }else if( t_sampler->wrapT == SVGLTF_TEXTURE_WRAP_MIRRORED_REPEAT ) {
                        tMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_MIRROR);
                    }
                    //
                    if( t_sampler->wrapR == SVGLTF_TEXTURE_WRAP_REPEAT ) {
                        //tMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_REPEAT);
                    }else if( t_sampler->wrapR == SVGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE ) {
                        //tMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_CLAMP_TO_EDAGE);
                    }else if( t_sampler->wrapR == SVGLTF_TEXTURE_WRAP_MIRRORED_REPEAT ) {
                        //tMtl->setTextureParam(0, E_T_PARAM_WRAP_T, E_T_WRAP_MIRROR);
                    }
                }
            }
        }else if(t_key == "baseColorFactor") {
            Parameter* t_param = &(it1->data);
            if(t_param->number_array.size() ==3) {
                tMtl->m_baseColorFactor.set(t_param->number_array[0],
                                            t_param->number_array[1],
                                            t_param->number_array[2],
                                            t_param->number_array[3]);
            }
        }else if(t_key == "metallicRoughnessTexture") {
            Parameter* t_param = &(it1->data);
            s32 textureIndex = t_param->TextureIndex();
            Texture* texture = &(m_gltf.textures[textureIndex]);
            Image* image = &(m_gltf.images[texture->source]);
            tMtl->m_pMetallicRoughnessTex = image->texture;
        }else if(t_key == "metallicFactor") {
            Parameter* t_param = &(it1->data);
            tMtl->m_metallicFactor = t_param->Factor();
        }else if(t_key == "roughnessFactor") {
            Parameter* t_param = &(it1->data);
            tMtl->m_roughtnessFactor = t_param->Factor();
        }
        it1++;
    }
    //
    ParameterMap::Iterator it2 = t_mtl->additionalValues.begin();
    while (it2!=t_mtl->additionalValues.end()) {
        SVString t_key = it2->key;
        if(t_key == "normalTexture") {
            Parameter* t_param = &(it2->data);
            if(t_param) {
                s32 textureIndex = t_param->TextureIndex();
                Texture* texture = &(m_gltf.textures[textureIndex]);
                Image* image = &(m_gltf.images[texture->source]);
                tMtl->m_pNormalTex = image->texture;
                tMtl->m_normalScale = t_param->ParamValue("scale");
            }
        }else if(t_key == "occlusionTexture") {
            Parameter* t_param = &(it2->data);
            if(t_param) {
                s32 textureIndex = t_param->TextureIndex();
                Texture* texture = &(m_gltf.textures[textureIndex]);
                Image* image = &(m_gltf.images[texture->source]);
                tMtl->m_pOcclusionTex = image->texture;
                tMtl->m_occlusionStrength = t_param->ParamValue("strength");
            }
        }else if(t_key == "emissiveTexture") {
            Parameter* t_param = &(it2->data);
            if(t_param) {
                s32 textureIndex = t_param->TextureIndex();
                Texture* texture = &(m_gltf.textures[textureIndex]);
                Image* image = &(m_gltf.images[texture->source]);
                tMtl->m_pEmissiveTex = image->texture;
            }
        }else if(t_key == "emissiveFactor") {
            Parameter* t_param = &(it2->data);
            if(t_param && t_param->number_array.size() ==3) {
                tMtl->m_emissiveFactor.set(t_param->number_array[0],
                                           t_param->number_array[1],
                                           t_param->number_array[2]);
            }
        }else if(t_key == "alphaMode") {
            Parameter* t_param = &(it2->data);
            if(t_param ) {
                SVString t_str = t_param->string_value;
            }
        }else if(t_key == "name") {
            //材质名称
            Parameter* t_param = &(it2->data);
            if(t_param ) {
                SVString t_str = t_param->string_value;
            }
        }
        it2++;
    }
    //
    tMtl->refresh();
    return tMtl;
}

void SVLoaderGLTF::_fetchDataFromAcc(SVDataSwapPtr _data,Accessor *_accessor) {
    s32 t_acc_off = _accessor->byteOffset;
    s32 t_viewID = _accessor->bufferView;
    BufferView* t_bufview = &(m_gltf.bufferViews[t_viewID]);
    if(t_bufview) {
        s32 t_bufID = t_bufview->buffer;
        s32 t_view_off = t_bufview->byteOffset;
        s32 t_len = t_bufview->byteLength;
        s32 t_view_stride = t_bufview->byteStride;
        Buffer* t_buf = &(m_gltf.buffers[t_bufID]);
        t_buf->data->lockData();
        char* p = (char*)(t_buf->data->getData());
        p += t_view_off;
        p += t_acc_off;
        s32 t_cmp_size = _getCmpSize(_accessor->componentType);
        s32 t_cmp_num = _getCmpNum(_accessor->type);
        s32 t_s_size =t_cmp_size*t_cmp_num;
        s32 t_total_len = _accessor->count*t_s_size;
        s32 t_aim_size = _data->getSize() + t_total_len;
        _data->extendSize(t_aim_size);
        //拷贝数据
        for(s32 i=0;i<_accessor->count;i++) {
            _data->appendData(p, t_s_size);
            p += t_s_size;
        }
        t_buf->data->unlockData();
    }
}

bool SVLoaderGLTF::_parseAsset(Asset *_asset , RAPIDJSON_NAMESPACE::Value &_item){
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

bool SVLoaderGLTF::_parseBuffer(Buffer *_buffer, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir){
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
        SV_LOG_ERROR("SVLoaderGLTF Error :'uri' is missing from non binary glTF file buffer.\n");
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
        if (!_loadExternalFile(_buffer->data, t_bufferUri.c_str(), bytes)) {    //直接打开数据文件，做数据读取了
            return false;
        }
    }
    return true;
}

bool SVLoaderGLTF::_parseBufferView(BufferView *_bufferView, RAPIDJSON_NAMESPACE::Value &_item){
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
        SV_LOG_ERROR("SVLoaderGLTF Error:Invalid 'byteStride' value. `byteStride' must be the multiple of");
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

bool SVLoaderGLTF::_parseAccessor(Accessor *_accessor, RAPIDJSON_NAMESPACE::Value &_item){
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

//解析网格
bool SVLoaderGLTF::_parseMesh(Mesh *_mesh, RAPIDJSON_NAMESPACE::Value &_item){
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _mesh->name = _item["name"].GetString();
    }
    _mesh->primitives.clear();
    //网格数据
    if (_item.HasMember("primitives") && _item["primitives"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_primitives = _item["primitives"];
        for (s32 i = 0; i<t_primitives.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_primitiveItem = t_primitives[i];
            Primitive primitive;
            if (_parsePrimitive(&primitive, t_primitiveItem)) {
                _mesh->primitives.append(primitive);
            }
        }
    }
    // morph 数据
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
    //权重数据
    if (_item.HasMember("weights") && _item["weights"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_weights = _item["weights"];
        for (s32 i=0; i<t_weights.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_weightItem = t_weights[i];
            _mesh->weights.append(t_weightItem.GetDouble());
        }
    }
    return true;
}

bool SVLoaderGLTF::_parsePrimitive(Primitive *_primitive, RAPIDJSON_NAMESPACE::Value &_item){
    //解析材质
    f64 material = -1.0;
    if (_item.HasMember("material") && _item["material"].IsNumber()) {
        material = _item["material"].GetDouble();
    }
    _primitive->material = s32(material);
    //解析模式
    f64 mode = static_cast<f64>(SVGLTF_MODE_TRIANGLES);
    if (_item.HasMember("mode") && _item["mode"].IsNumber()) {
        mode = _item["mode"].GetDouble();
    }
    s32 primMode = static_cast<s32>(mode);
    _primitive->mode = primMode;  // Why only triangled were supported ?
    //解析索引数据
    f64 indices = -1.0;
    if (_item.HasMember("indices") && _item["indices"].IsNumber()) {
        indices = _item["indices"].GetDouble();
    }
    _primitive->indices = static_cast<s32>(indices);
    //解析格式数据
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

bool SVLoaderGLTF::_parseNode(Node *_node, RAPIDJSON_NAMESPACE::Value &_item){
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _node->name = _item["name"].GetString();
    }
    //解析骨骼
    f64 skin = -1.0;
    if (_item.HasMember("skin") && _item["skin"].IsNumber()) {
        skin = _item["skin"].GetDouble();
    }
    _node->skin = static_cast<s32>(skin);
    //解析基础矩阵
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
    //是否是相机
    f64 camera = -1.0;
    if (_item.HasMember("camera") && _item["camera"].IsNumber()) {
        camera = _item["camera"].GetDouble();
    }
    _node->camera = static_cast<s32>(camera);
    //是否有网格
    f64 mesh = -1.0;
    if (_item.HasMember("mesh") && _item["mesh"].IsNumber()) {
        mesh = _item["mesh"].GetDouble();
    }
    _node->mesh = s32(mesh);
    //是否有子节点
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

bool SVLoaderGLTF::_parseMaterial(Material *_material, RAPIDJSON_NAMESPACE::Value &_item) {
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

bool SVLoaderGLTF::_parseParameterProperty(Parameter *_param, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _prop){
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


bool SVLoaderGLTF::_parseTexture(Texture *_texture, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir) {
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

bool SVLoaderGLTF::_parseAnimation(Animation *_animation, RAPIDJSON_NAMESPACE::Value &_item) {
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
                    SV_LOG_ERROR("SVLoaderGLTF Error :'input' field is missing in animation.sampler\n");
                    return false;
                }
                
                if (t_samplerItem.HasMember("interpolation") && t_samplerItem["interpolation"].IsString()) {
                    sampler.interpolation = t_samplerItem["interpolation"].GetString();
                }else{
                    SV_LOG_ERROR("SVLoaderGLTF Error :'interpolation' field is missing in animation.sampler\n");
                    return false;
                }
                
                if (t_samplerItem.HasMember("output") && t_samplerItem["output"].IsNumber()) {
                    outputIndex = t_samplerItem["output"].GetDouble();
                }else{
                    SV_LOG_ERROR("SVLoaderGLTF Error :'output' field is missing in animation.sampler\n");
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

bool SVLoaderGLTF::_parseAnimationChannel(AnimationChannel *_channel, RAPIDJSON_NAMESPACE::Value &_item) {
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

bool SVLoaderGLTF::_parseSkin(Skin *_skin, RAPIDJSON_NAMESPACE::Value &_item) {
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
    if (_item.HasMember("name") && _item["name"].IsString()) {
        _skin->name = _item["name"].GetString();
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

bool SVLoaderGLTF::_parseSampler(Sampler *_sampler, RAPIDJSON_NAMESPACE::Value &_item) {
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

bool SVLoaderGLTF::_loadExternalFile(SVDataSwapPtr _dataOut, cptr8 _filename, s64 _reqBytes){
    SVDataChunk tDataStream;
    bool tflag = mApp->m_pGlobalMgr->m_pFileMgr->loadFileContentStr(&tDataStream, _filename);
    if (!tflag){
        SV_LOG_ERROR("SVLoaderGLTF Error:load ExternalFile failed\n");
        return false;
    }
    SV_LOG_INFO("SVLoaderGLTF :ExternalFile filedata %s\n", tDataStream.m_data);
    if (!tDataStream.m_data) {
        SV_LOG_ERROR("SVLoaderGLTF Error:ExternalFile data stream is null");
        return false;
    }
    
    //SVDataChunk 空间多申请了一个字节
    if ((tDataStream.m_size - 1) != _reqBytes) {
        SV_LOG_ERROR("SVLoaderGLTF Error:ExternalFile File size mismatch requestedBytes");
        return false;
    }
    _dataOut->writeData(tDataStream.m_data, tDataStream.m_size);
    return true;
}

bool SVLoaderGLTF::_parseImage(Image *_image, RAPIDJSON_NAMESPACE::Value &_item, cptr8 _basedir){
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
        SV_LOG_ERROR("SVLoaderGLTF Error:Only one of `bufferView` or `uri` should be defined, but both are defined for Image.\n");
        return false;
    }
    
    if (!hasBufferView && !hasURI) {
        SV_LOG_ERROR("SVLoaderGLTF Error:Neither required `bufferView` nor `uri` defined for Image.\n");
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
        SV_LOG_ERROR("SVLoaderGLTF Error:Failed to load external 'uri' for image parameter\n");
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

bool SVLoaderGLTF::_isDataURI(cptr8 _inData){
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

bool SVLoaderGLTF::_decodeDataURI(SVString &_outData, SVString &_mime_type,
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
    return true;
}

static const SVString base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static bool is_base64(u8 c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

cptr8 SVLoaderGLTF::_base64_encode(u8 const *bytes_to_encode,u32 in_len) {
    SVString ret;
    s32 i = 0;
    s32 j = 0;
    u8 char_array_3[3];
    u8 char_array_4[4];
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            for (i = 0; (i < 4); i++) {
                ret += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }
    if (i) {
        for (j = i; j < 3; j++){
            char_array_3[j] = '\0';
        }
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        for (j = 0; (j < i + 1); j++) {
            ret += base64_chars[char_array_4[j]];
        }
        while ((i++ < 3)){
            ret += '=';
        }
    }
    return ret.c_str();
}

cptr8 SVLoaderGLTF::_base64_decode(SVString const &encoded_string){
    s32 in_len = static_cast<int>(encoded_string.size());
    s32 i = 0;
    s32 j = 0;
    s32 in_ = 0;
    u8 char_array_4[4], char_array_3[3];
    SVString ret;
    
    while (in_len-- && (encoded_string[in_] != '=') &&
           is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++){
                char_array_4[i] =
                static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
            }
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            for (i = 0; (i < 3); i++) {
               ret += char_array_3[i];
            }
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j < 4; j++) {
            char_array_4[j] = 0;
        }
        for (j = 0; j < 4; j++) {
            char_array_4[j] =
            static_cast<unsigned char>(base64_chars.find(char_array_4[j]));
        }
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        for (j = 0; (j < i - 1); j++){
            ret += char_array_3[j];
        }
    }
    return ret.c_str();
}

void SVLoaderGLTF::_loadAnimationData(){
//    for (s32 i = 0; i<m_gltf.animations.size(); i++) {
//        Animation t_animation = m_gltf.animations[i];
//        SVGLTFAnimationPtr animation = MakeSharedPtr<SVGLTFAnimation>();
//        m_gltf.m_animations.append(animation);
//        for (s32 j = 0; j<t_animation.channels.size(); j++) {
//            AnimationChannel t_channel = t_animation.channels[j];
//            SVGLTFAnimationChannelPtr channel = MakeSharedPtr<SVGLTFAnimationChannel>();
//            animation->m_channels.append(channel);
//            channel->m_targeNodeIndex = t_channel.target_node;
//            channel->m_targetPath = t_channel.target_path;
//            AnimationSampler t_sampler = t_animation.samplers[t_channel.sampler];
//            SVGLTFAnimationSamplerPtr sampler = MakeSharedPtr<SVGLTFAnimationSampler>();
//            channel->m_sampler = sampler;
//            sampler->m_inputData = MakeSharedPtr<SVDataSwap>();
//            sampler->m_outputData = MakeSharedPtr<SVDataSwap>();
//            //input
//            Accessor t_inputAccessor = m_gltf.accessors[t_sampler.input];
//            BufferView t_inputBufferView = m_gltf.bufferViews[t_inputAccessor.bufferView];
//            s32 t_inputByteStride = t_inputBufferView.byteStride;
//            s64 t_inputCount = t_inputAccessor.count;
//            Buffer t_inputBuffer = m_gltf.buffers[t_inputBufferView.buffer];
//            const u8 *t_inputDataAddress = (u8 *)t_inputBuffer.data->getData() + t_inputBufferView.byteOffset +t_inputAccessor.byteOffset;
//            sampler->m_inputData->writeData((void *)t_inputDataAddress, t_inputCount*4);
//            //output
//            Accessor t_outputAccessor = m_gltf.accessors[t_sampler.output];
//            BufferView t_outputBufferView = m_gltf.bufferViews[t_outputAccessor.bufferView];
//            s32 t_outputByteStride = t_outputBufferView.byteStride;
//            s64 t_outputCount = t_outputAccessor.count;
//            Buffer t_outputBuffer = m_gltf.buffers[t_outputBufferView.buffer];
//            const u8 *t_outputDataAddress = (u8 *)t_outputBuffer.data->getData() + t_outputBufferView.byteOffset +t_outputAccessor.byteOffset;
//            sampler->m_outputData->writeData((void *)t_outputDataAddress, t_outputCount*4);
//            sampler->m_interpolationMode = _getInterpolationMode(t_sampler.interpolation);
//        }
//    }
}

void SVLoaderGLTF::_loadSkinsData(){
//    if (!_model) {
//        return;
//    }
//    for (s32 i = 0; i < m_gltf.skins.size(); i++) {
//        Skin t_skin = m_gltf.skins[i];
//        SVGLTFSkinPtr skin = MakeSharedPtr<SVGLTFSkin>();
//        skin->m_skinIndex = i;
//        Accessor t_accessor = m_gltf.accessors[t_skin.inverseBindMatrices];
//        BufferView t_bufView = m_gltf.bufferViews[t_accessor.bufferView];
//        Buffer t_buf = m_gltf.buffers[t_bufView.buffer];
//        for (s32 j = 0; j < t_skin.joints.size(); j++) {
//            SVGLTFJointPtr joint = MakeSharedPtr<SVGLTFJoint>();
//            s32 t_jointIndex = t_skin.joints[j];
//            joint->m_jointIndex = t_jointIndex;
//            Node t_node = m_gltf.nodes[t_jointIndex];
//            //
//            FMat4 t_globalJointTransform;
//            t_globalJointTransform.setIdentity();
//            //translate
//            FMat4 matT;
//            matT.setIdentity();
//            if (t_node.translation.size() > 0) {
//                matT.setTranslate(FVec3(t_node.translation[0], t_node.translation[1], t_node.translation[2]));
//            }
//            //rotation
//            FMat4 matR;
//            matR.setIdentity();
//            if (t_node.rotation.size() > 0) {
//                matR.set(SVQuat(FVec4(t_node.rotation[0], t_node.rotation[1], t_node.rotation[2], t_node.rotation[3])));
//            }
//            //scale
//            FMat4 matS;
//            matS.setIdentity();
//            if (t_node.scale.size() > 0) {
//                matS.setScale(FVec3(t_node.scale[0], t_node.scale[1], t_node.scale[2]));
//            }
//            t_globalJointTransform = matT * matR * matS;
//
//            //matrix
//            if (t_node.matrix.size() > 0) {
//                FVec4 col0(t_node.matrix[0], t_node.matrix[1], t_node.matrix[2], t_node.matrix[3]);
//                FVec4 col1(t_node.matrix[4], t_node.matrix[5], t_node.matrix[6], t_node.matrix[7]);
//                FVec4 col2(t_node.matrix[8], t_node.matrix[9], t_node.matrix[10], t_node.matrix[11]);
//                FVec4 col3(t_node.matrix[12], t_node.matrix[13], t_node.matrix[14], t_node.matrix[15]);
//                t_globalJointTransform.setColumn(0, col0);
//                t_globalJointTransform.setColumn(1, col1);
//                t_globalJointTransform.setColumn(2, col2);
//                t_globalJointTransform.setColumn(3, col3);
//            }
//            joint->m_globalJointTransform = t_globalJointTransform;
//        }
//    }
}
