//
// SVSpine.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SPINE_H
#define SV_SPINE_H

#include "../third/spine/include/spine/Skeleton.h"
#include "../third/spine/include/spine/AnimationState.h"
#include "../third/spine/include/spine/RegionAttachment.h"
#include "SVSpineListen.h"
#include "../mtl/SVTexture.h"
#include "../node/SVNode.h"
#include "../base/SVArray.h"
#include "../base/svstr.h"

struct spMeshAttachment;

struct spMeshAttachment;

static const u16 quadTriangles[6] = {0, 1, 2, 2, 3, 0};

namespace sv {
    
    namespace util{
        class SpineMeshData :public SVObject{
        public:
            SpineMeshData();
            ~SpineMeshData();
            s32                     m_blendMode;
            s32                     m_indexCount;
            s32                     m_vertexCount;
            SVRenderMeshPtr         m_pRenderMesh;
            SVDataSwapPtr           m_pRenderVertex;
            SVDataSwapPtr           m_pRenderIndex;
            SVArray<V2_C_T0>        m_aRenderVecData;
            SVArray<u16>            m_aRenderIndexData;
            SVTexturePtr            m_pTex;
        };
        
        //spine对象
        class SVSpine : public SVGBase {
        public:
            static SVSpinePtr
            createSpine(SVInst *_app, cptr8 skefname,cptr8 atlasfname, f32 scale = 1.0f);
            
            SVSpine(SVInst *_app);
            
            ~SVSpine();
            
            void init(spSkeletonData *skedata, spAtlas *atlas, bool ownsSkeletonData = true);
            
            void setAnimationStateData(spAnimationStateData *stateData);
            
            void getAllAnimationName(SVArray<SVString>& _vecName);
            
            spTrackEntry *setAnimationForTrack(s32 trackindex, cptr8 name, bool loop);
            
            spTrackEntry *addAnimationForTrack(s32 trackindex, cptr8 name, bool loop, s32 delay);
            
            void clearTracks();
            
            void clearTrack(s32 trackIndex);
            
            void setMixFrom(cptr8 fromAnimation, cptr8 toAnimation, f32 duration);
            
            spTrackEntry *getCurrentForTrack(s32 trackIndex);
            
            void setToSetupPose();
            
            void setBonesToSetupPose();
            
            void setSlotsToSetupPose();
            
            /* Returns 0 if the bone was not found. */
            spBone *findBone(cptr8 _boneName);
            
            /* Returns 0 if the slot was not found. */
            spSlot *findSlot(cptr8 _slotName);
            
            bool setSkin(cptr8 _skinName);
            
            /* Returns 0 if the slot or attachment was not found. */
            spAttachment *getAttachment(cptr8 _slotName, cptr8 _attachmentName);
            /* Returns false if the slot or attachment was not found. */
            bool setAttachment(cptr8 _slotName, cptr8 _attachmentName);
            
            // --- Convenience methods for common Skeleton_* functions.
            void updateWorldTransform();
            
            void setListenerForEntryByStart(spTrackEntry *entry, spStartListener listener);
            
            void setListenerForEntryByEnd(spTrackEntry *entry, spEndListener listener);
            
            void setListenerForEntryByComp(spTrackEntry *entry, spCompleteListener listener);
            
            void setListenerForEntryByEvent(spTrackEntry *entry, spEventListener listener);
            
            void onAnimationStateEvent(s32 trackIndex, spEventType type, spEvent *event, s32 loopCount);
            
            void onTrackEntryEvent(s32 trackIndex, spEventType type, spEvent *event, s32 loopCount);
            
            void update(f32 deltaTime);
            
        protected:
            SVTexturePtr _getTexture(spAttachment *pAttach);
            
            SVTexturePtr _getTextureForRegion(spRegionAttachment *_attachment);
            
            SVTexturePtr _getTextureForMesh(spMeshAttachment *_attachment);
            
        public:
            spAtlas *m_pAtlas;
            
            spAnimationState *m_pSpineAniState;
            
            bool m_preMultAlpha;
            
            bool m_ownsSkeData;
            
            f32 *m_pWorldVerts;
            
            spSkeleton *m_pSkeleton;
            
            spBone *m_pRootBone;
            
            bool m_ownsAniStateData;
            
            f32 m_spineTimeScale;
            
            spStartListener m_cb_startListener;
            
            spEndListener m_cb_endListener;
            
            spCompleteListener m_cb_completeListener;
            
            spEventListener m_cb_eventListener;
            
        public:
            SVString m_spineName;
            
            SVString m_spine_json;
            
            SVString m_spine_atlas;
            
            SVString m_activeAct;
            
            cptr8 getSpineName();
            
            void setSpineName(cptr8 name);
            
            SVBoundBox m_aabbBox;
            
            typedef SVArray<SpineMeshDataPtr> SPINEDATAPOOL;
            SPINEDATAPOOL m_spineDataPool;
            
        };
        
    }//!namespace util
    
}//!namespace sv



#endif //SV_SPINE_H
