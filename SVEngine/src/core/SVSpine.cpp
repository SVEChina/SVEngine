//
// SVSpine.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSpine.h"
#include "SVVertDef.h"
#include "../app/SVGlobalMgr.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderMesh.h"
#include "../mtl/SVTexMgr.h"
#include <spine/SkeletonJson.h>
#include <spine/extension.h>
#include <spine/AnimationState.h>
#include <spine/Event.h>
#include <spine/Atlas.h>
#include <spine/SkeletonData.h>
#include <spine/AnimationStateData.h>
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>
#include <spine/Bone.h>
#include <spine/Slot.h>
#include <spine/Attachment.h>
#include "SVSpineObjc.h"

#define MESH_MAX_VERTEXNUM 1000

//
SpineMeshData::SpineMeshData(){
    m_blendMode = SP_BLEND_MODE_NORMAL;
    m_indexCount    = 0;
    m_vertexCount   = 0;
    m_pRenderVertex = nullptr;
    m_pRenderIndex  = nullptr;
    m_pRenderMesh   =  nullptr;
    m_pTex = nullptr;
    m_aRenderVecData.destroy();
    m_aRenderIndexData.destroy();
}

SpineMeshData::~SpineMeshData(){
    m_blendMode = SP_BLEND_MODE_NORMAL;
    m_pRenderMesh   = nullptr;
    m_pRenderVertex = nullptr;
    m_pRenderIndex  = nullptr;
    m_pTex = nullptr;
    m_indexCount    = 0;
    m_vertexCount   = 0;
    m_aRenderVecData.destroy();
    m_aRenderIndexData.destroy();
}

//
SVSpinePtr
SVSpine::createSpine(SVInst *_app, cptr8 skefname, cptr8 atlasfname,f32 scale) {
    spAtlas *_atlas = spAtlas_createFromFile(_app, atlasfname, 0);
    if (!_atlas)
        return nullptr;
    //骨架json
    spSkeletonJson *json = spSkeletonJson_create(_atlas);
    json->scale = scale;
    //动画数据
    spSkeletonData *skeletonData = spSkeletonJson_readSkeletonDataFile(_app, json, skefname);
    spSkeletonJson_dispose(json);
    if (!skeletonData)
        return nullptr;
    SVSpinePtr t_spine = MakeSharedPtr<SVSpine>(_app);//new SVSpine(_app);
    t_spine->init(skeletonData, _atlas);
    SVString t_json_file = skefname;
    s32 t_pos = t_json_file.rfind('/');
    s32 t_len = t_json_file.size();
    SVString t_json_name = SVString::substr(t_json_file, t_pos+1, t_len - t_pos-1);
    t_spine->m_spine_json = t_json_name;
    SVString t_atlas_file = atlasfname;
    t_pos = t_atlas_file.rfind('/');
    t_len = t_atlas_file.size();
    SVString t_atlas_name = SVString::substr(t_atlas_file, t_pos+1, t_len - t_pos-1);
    t_spine->m_spine_atlas = t_atlas_name;
    return t_spine;
}

SVSpine::SVSpine(SVInst *_app)
:SVGBase(_app){
    m_pSpineAniState = nullptr;
    m_pSkeleton = nullptr;
    m_pRootBone = nullptr;
    m_pAtlas = nullptr;
    //
    m_preMultAlpha = false;
    m_spineTimeScale = 1.0;
    //
    m_cb_startListener = nullptr;
    m_cb_endListener = nullptr;
    m_cb_completeListener = nullptr;
    m_cb_eventListener = nullptr;
}

SVSpine::~SVSpine() {
    //
    m_spineDataPool.destroy();
    //
    if (m_pAtlas) {
        spAtlas_dispose(m_pAtlas);
        m_pAtlas = nullptr;
    }
    if (m_ownsSkeData) {
        spSkeletonData_dispose(m_pSkeleton->data);
    }
    if (m_pSkeleton) {
        spSkeleton_dispose(m_pSkeleton);
        m_pSkeleton = nullptr;
    }
    if (m_ownsAniStateData) {
        spAnimationStateData_dispose(m_pSpineAniState->data);
    }
    if (m_pSpineAniState) {
        spAnimationState_dispose(m_pSpineAniState);
        m_pSpineAniState = nullptr;
    }
    FREE(m_pWorldVerts);
}

//初始化
void SVSpine::init(spSkeletonData *skedata, spAtlas *atlas, bool ownsSkeletonData) {
    m_pAtlas = atlas;
    m_ownsSkeData = ownsSkeletonData;
    m_spineTimeScale = 1;
    m_pSkeleton = spSkeleton_create(skedata);
    m_pRootBone = m_pSkeleton->bones[0];
    m_preMultAlpha = true;
    m_pWorldVerts = MALLOC(f32, MESH_MAX_VERTEXNUM); // Max number of vertices per mesh.
    //
    m_ownsAniStateData = true;
    m_pSpineAniState = spAnimationState_create(spAnimationStateData_create(m_pSkeleton->data));
    m_pSpineAniState->rendererObject = (void *) (this);
    m_pSpineAniState->timeScale = m_spineTimeScale;
    m_pSpineAniState->listener = animationCallback;
    
    spAnimationState_update(m_pSpineAniState, 0.0f);
    spAnimationState_apply(m_pSpineAniState, m_pSkeleton);
    spSkeleton_updateWorldTransform(m_pSkeleton);
    //构建mesh(这里需要统计顶点数目等相关操作)
    for (s32 i = 0, n = m_pSkeleton->slotsCount; i < n; i++) {
        spSlot *t_slot = m_pSkeleton->drawOrder[i];
        SpineMeshDataPtr pMeshData = MakeSharedPtr<SpineMeshData>();
        pMeshData->m_blendMode = t_slot->data->blendMode;
        pMeshData->m_pRenderMesh = MakeSharedPtr<SVRenderMesh>(mApp);
        pMeshData->m_pRenderMesh->setVertexPoolType(GL_DYNAMIC_DRAW);
        pMeshData->m_pRenderMesh->setIndexPoolType(GL_DYNAMIC_DRAW);
        pMeshData->m_pRenderIndex  = MakeSharedPtr<SVDataSwap>();
        pMeshData->m_pRenderVertex = MakeSharedPtr<SVDataSwap>();
        m_spineDataPool.append(pMeshData);
    }
}

SVTexturePtr SVSpine::_getTexture(spAttachment *pAttach) {
    switch (pAttach->type) {
        case SP_ATTACHMENT_REGION: {
            spRegionAttachment *attachment = (spRegionAttachment *) pAttach;
            return _getTextureForRegion(attachment);    //关联纹理
        }
        case SP_ATTACHMENT_MESH: {
            spMeshAttachment *attachment = (spMeshAttachment *) pAttach;
            return _getTextureForMesh(attachment);
        }
        default:;
    }
    return nullptr;
}

//更新
void SVSpine::update(f32 deltaTime) {
    //更新spine
    spSkeleton_update(m_pSkeleton, deltaTime);
    spAnimationState_update(m_pSpineAniState, deltaTime);
    spAnimationState_apply(m_pSpineAniState, m_pSkeleton);
    spSkeleton_updateWorldTransform(m_pSkeleton);
    //更新mesh
    bool m_isFirstaabbBox = true;
    //绘制前关掉面剪切（3D模型绘制时需要开启）
    s32 blendMode = -1;
    const f32 *uvs = 0;
    s32 verticesCount = 0;
    const u16 *triangles = 0;
    s32 trianglesCount = 0;
    f32 r = 0, g = 0, b = 0, a = 0;
    for (s32 i = 0, n = m_pSkeleton->slotsCount; i < n; i++) {
        spSlot *t_slot = m_pSkeleton->drawOrder[i];
        if (!t_slot->attachment) {
            continue;
        }
        SpineMeshDataPtr r_meshdata = m_spineDataPool[i];
        SVTexturePtr pTex = _getTexture(t_slot->attachment);
        r_meshdata->m_pTex = pTex;
        //拿到对应的数据
        if (!r_meshdata->m_pRenderMesh)
            continue;
        switch (t_slot->attachment->type) {
            case SP_ATTACHMENT_REGION: {
                spRegionAttachment *attachment = (spRegionAttachment *) t_slot->attachment;
                spRegionAttachment_computeWorldVertices(attachment,
                                                        t_slot->bone,
                                                        m_pWorldVerts,
                                                        0,
                                                        2);//这里stride填了2
                uvs = attachment->uvs;      //纹理坐标
                for (s32 j = 0; j < 6; ++j) {
                    r_meshdata->m_aRenderIndexData.append(r_meshdata->m_vertexCount + quadTriangles[j]);
                }
                verticesCount = 8;          //顶点数据数
                r_meshdata->m_vertexCount += verticesCount / 2;
                r_meshdata->m_indexCount += 6;         //三角索引数量
                r = attachment->color.r;
                g = attachment->color.g;
                b = attachment->color.b;
                a = attachment->color.a;
                break;
            }
            case SP_ATTACHMENT_MESH: {
                spMeshAttachment *attachment = (spMeshAttachment *) t_slot->attachment;
                spVertexAttachment_computeWorldVertices(SUPER(attachment),
                                                        t_slot,
                                                        0,
                                                        attachment->super.worldVerticesLength,
                                                        m_pWorldVerts,
                                                        0,
                                                        2);//stride 是2
                uvs = attachment->uvs;

                trianglesCount = attachment->trianglesCount;
                for (s32 j = 0; j < trianglesCount; ++j) {
                    r_meshdata->m_aRenderIndexData.append(r_meshdata->m_vertexCount + attachment->triangles[j]);
                }
                verticesCount = attachment->super.worldVerticesLength;
                r_meshdata->m_vertexCount += verticesCount / 2;
                r_meshdata->m_indexCount += trianglesCount;
                r = attachment->color.r;
                g = attachment->color.g;
                b = attachment->color.b;
                a = attachment->color.a;
                break;
            }
            default:;
        }
        r_meshdata->m_pRenderMesh->setvisible(true);
        //更新mesh
        r_meshdata->m_pRenderMesh->setVertexType(E_VF_V2_C_T0);
        m_preMultAlpha = true;
        if (m_preMultAlpha) {
            a *= m_pSkeleton->color.a * t_slot->color.a;
            r *= m_pSkeleton->color.r * t_slot->color.r * a;
            g *= m_pSkeleton->color.g * t_slot->color.g * a;
            b *= m_pSkeleton->color.b * t_slot->color.b * a;
        } else {
            a *= m_pSkeleton->color.a * t_slot->color.a;
            r *= m_pSkeleton->color.r * t_slot->color.r;
            g *= m_pSkeleton->color.g * t_slot->color.g;
            b *= m_pSkeleton->color.b * t_slot->color.b;
        }
        //
        f32 t_off_x = 0.0f;
        f32 t_off_y = 0.0f;
        for (s32 k = 0; k * 2 < verticesCount; ++k) {
            V2_C_T0 pt;
            pt.x = m_pWorldVerts[k * 2] + t_off_x;
            pt.y = m_pWorldVerts[k * 2 + 1] + t_off_y;
            pt.r = (u8) (r * 255);
            pt.g = (u8) (g * 255);
            pt.b = (u8) (b * 255);
            pt.a = (u8) (a * 255);
            pt.t0x = uvs[k * 2];
            pt.t0y = uvs[k * 2 + 1];
            r_meshdata->m_aRenderVecData.append(pt);
            //更新包围盒
            if(m_isFirstaabbBox){
                m_isFirstaabbBox = false;
                m_aabbBox.clear();
                m_aabbBox.expand(FVec3(pt.x, pt.y, 0.0));
            }else{
                m_aabbBox.expand(FVec3(pt.x, pt.y, 0.0));
            }
        }
    }
    //
    
    for (s32 i = 0, n = m_pSkeleton->slotsCount; i < n; i++) {
        spSlot *t_slot = m_pSkeleton->drawOrder[i];
        SpineMeshDataPtr pMeshData = m_spineDataPool[i];
        if (!t_slot->attachment) {
            continue;
        }
        s32 t_len = (s32) (sizeof(V2_C_T0) * pMeshData->m_aRenderVecData.size());
        if (t_len == 0) {
            pMeshData->m_pRenderMesh->setvisible(false);
        } else {
            pMeshData->m_pRenderMesh->setvisible(true);
            pMeshData->m_pRenderVertex->writeData(pMeshData->m_aRenderVecData.get(),
                                                  t_len);
            pMeshData->m_pRenderIndex->writeData(pMeshData->m_aRenderIndexData.get(),
                                                 pMeshData->m_indexCount *sizeof(u16));
            pMeshData->m_pRenderMesh->setVertexDataNum(pMeshData->m_vertexCount);
            pMeshData->m_pRenderMesh->setVertexData(pMeshData->m_pRenderVertex);
            pMeshData->m_pRenderMesh->setIndexData(pMeshData->m_pRenderIndex,
                                                   pMeshData->m_indexCount);
            pMeshData->m_pRenderMesh->createMesh();
        }
        pMeshData->m_aRenderVecData.clear();
        pMeshData->m_aRenderIndexData.clear();
        pMeshData->m_indexCount = 0;
        pMeshData->m_vertexCount = 0;
    }
}

void SVSpine::setAnimationStateData(spAnimationStateData *stateData) {
    if (nullptr == stateData){
        printf("stateData cannot be null.");
    }
    if (m_ownsAniStateData) {
        spAnimationStateData_dispose(m_pSpineAniState->data);
    }
    spAnimationState_dispose(m_pSpineAniState);
    m_ownsAniStateData = false;
    m_pSpineAniState = spAnimationState_create(stateData);
    m_pSpineAniState->rendererObject = (void *) this;
    m_pSpineAniState->listener = animationCallback;
}

//控制类接口

//
void SVSpine::getAllAnimationName(SVArray<SVString>& _vecName) {
    if(m_pSkeleton && m_pSkeleton->data) {
        for(s32 i=0; i<m_pSkeleton->data->animationsCount; i++){
            SVString t_name = m_pSkeleton->data->animations[i]->name;
            _vecName.append(t_name);
        }
    }
}

cptr8 SVSpine::getAnimationNameByIndex(s32 _index){
    
    if(m_pSkeleton && m_pSkeleton->data) {
        if (_index >=0 && _index < m_pSkeleton->data->animationsCount) {
            cptr8 t_name = m_pSkeleton->data->animations[_index]->name;
            return t_name;
        }
    }
    return "";
}

spTrackEntry *SVSpine::setAnimationForTrack(s32 trackindex, cptr8 name, bool loop) {
    spAnimation *animation = spSkeletonData_findAnimation(m_pSkeleton->data, name);
    if (!animation) {
        return nullptr;
    }
    return spAnimationState_setAnimation(m_pSpineAniState, trackindex, animation, loop);
}

spTrackEntry *
SVSpine::addAnimationForTrack(s32 trackindex, cptr8 name, bool loop, s32 delay) {
    spAnimation *animation = spSkeletonData_findAnimation(m_pSkeleton->data, name);
    if (!animation) {
        return nullptr;
    }
    return spAnimationState_addAnimation(m_pSpineAniState, trackindex, animation, loop, delay);
}

void SVSpine::clearTracks() {
    spAnimationState_clearTracks(m_pSpineAniState);
}

void SVSpine::clearTrack(s32 trackIndex) {
    spAnimationState_clearTrack(m_pSpineAniState, trackIndex);
}

void SVSpine::setMixFrom(cptr8 fromAnimation, cptr8 toAnimation, f32 duration) {
    spAnimationStateData_setMixByName(m_pSpineAniState->data, fromAnimation, toAnimation, duration);
}

spTrackEntry *SVSpine::getCurrentForTrack(s32 trackIndex) {
    return spAnimationState_getCurrent(m_pSpineAniState, trackIndex);
}

//////
SVTexturePtr SVSpine::_getTextureForRegion(spRegionAttachment *_attachment) {
    
    return ((SVSpineObjc *)((spAtlasRegion *) _attachment->rendererObject)->page->pRenderObj)->m_texture;
}

SVTexturePtr SVSpine::_getTextureForMesh(spMeshAttachment *_attachment) {
   
    return ((SVSpineObjc *)((spAtlasRegion *) _attachment->rendererObject)->page->pRenderObj)->m_texture;
}

//render spine
void SVSpine::updateWorldTransform() {
    spSkeleton_updateWorldTransform(m_pSkeleton);
}

void SVSpine::setToSetupPose() {
    spSkeleton_setToSetupPose(m_pSkeleton);
}

void SVSpine::setBonesToSetupPose() {
    spSkeleton_setBonesToSetupPose(m_pSkeleton);
}

void SVSpine::setSlotsToSetupPose() {
    spSkeleton_setSlotsToSetupPose(m_pSkeleton);
}

spBone *SVSpine::findBone(cptr8 _boneName) {
    return spSkeleton_findBone(m_pSkeleton, _boneName);
}

spSlot *SVSpine::findSlot(cptr8 _slotName) {
    return spSkeleton_findSlot(m_pSkeleton, _slotName);
}

bool SVSpine::setSkin(cptr8 _skinName) {
    return (bool) spSkeleton_setSkinByName(m_pSkeleton, _skinName ? _skinName : 0);
}

spAttachment *SVSpine::getAttachment(cptr8 _slotName, cptr8 _attachmentName) {
    return spSkeleton_getAttachmentForSlotName(m_pSkeleton, _slotName, _attachmentName);
}

bool SVSpine::setAttachment(cptr8 _slotName, cptr8 _attachmentName) {
    return (bool) spSkeleton_setAttachment(m_pSkeleton, _slotName, _attachmentName);
}

void SVSpine::setListenerForEntryByStart(spTrackEntry *entry, spStartListener listener) {
//    getListeners(entry)->m_cb_startListener =  listener->copy;
}

void SVSpine::setListenerForEntryByEnd(spTrackEntry *entry, spEndListener listener) {
//    getListeners(entry)->m_cb_endListener = listener.copy();

}

void SVSpine::setListenerForEntryByComp(spTrackEntry *entry, spCompleteListener listener) {
//    getListeners(entry)->m_cb_completeListener = [listener copy];
}

void SVSpine::setListenerForEntryByEvent(spTrackEntry *entry, spEventListener listener) {
//    getListeners(entry)->m_cb_eventListener = [listener copy];
}

void
SVSpine::onAnimationStateEvent(s32 trackIndex, spEventType type, spEvent *event, s32 loopCount) {
    switch (type) {
        case SP_ANIMATION_START: {
            if (m_cb_startListener)
                m_cb_startListener(trackIndex);
            break;
        }
        case SP_ANIMATION_END: {
            if (m_cb_endListener)
                m_cb_endListener(trackIndex);
            break;
        }
        case SP_ANIMATION_COMPLETE: {
            if (m_cb_completeListener)
                m_cb_completeListener(trackIndex, loopCount);
            break;
        }
        case SP_ANIMATION_EVENT: {
            if (m_cb_eventListener)
                m_cb_eventListener(trackIndex, event);
            break;
        }
    }
}

void SVSpine::onTrackEntryEvent(s32 trackIndex, spEventType type, spEvent *event, s32 loopCount) {
    spTrackEntry *entry = spAnimationState_getCurrent(m_pSpineAniState, trackIndex);
    if (!entry->rendererObject)
        return;
    _TrackEntryListeners *listeners = (_TrackEntryListeners *) entry->rendererObject;
    switch (type) {
        case SP_ANIMATION_START:
            if (listeners->m_cb_startListener) {
                listeners->m_cb_startListener(trackIndex);
            }
            break;
        case SP_ANIMATION_END:
            if (listeners->m_cb_endListener) {
                listeners->m_cb_endListener(trackIndex);
            }
            break;
        case SP_ANIMATION_COMPLETE:
            if (listeners->m_cb_completeListener) {
                listeners->m_cb_completeListener(trackIndex, loopCount);
            }
            break;
        case SP_ANIMATION_EVENT:
            if (listeners->m_cb_eventListener) {
                listeners->m_cb_eventListener(trackIndex, event);
            }
            break;
    }
}

cptr8 SVSpine::getSpineName() {
    return m_spineName.c_str();
}

void SVSpine::setSpineName(cptr8 name) {
    m_spineName = name;
}

//cptr8 SVSpine::getActiveAct() {
//    return m_activeAct.c_str();
//}
//
//void SVSpine::setActiveAct(cptr8 actname) {
//    m_activeAct = actname;
//}

//spTrackEntry *SVSpine::addAnimationForTrack(s32 trackIndex, cptr8 name, bool loop, s32 delay) {
//    spAnimation* animation = spSkeletonData_findAnimation(m_pSkeleton->data, name);
//    if (!animation) {
//        printf("Spine: Animation not found: %s",name);
//        return 0;
//    }
//    return spAnimationState_addAnimation(m_pSpineAniState, trackIndex, animation, loop, delay);
//}





