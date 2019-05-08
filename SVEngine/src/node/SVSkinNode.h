//
// SVSkinNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SKINNODE_H
#define SV_SKINNODE_H

#include "SVNode.h"
#include "../base/SVMap.h"


namespace sv {
    
    namespace node{
        
        class SVSkinNode : public SVNode {
        public:
            SVSkinNode(SVInst *_app);
            
            ~SVSkinNode();
            
//            void update(f32 dt);
//            
//            void render();
//            
//            void loadModel(cptr8 pPath ,cptr8 pDir);
//            
//            void addAnimate(cptr8 pName , s32 iIndex , s32 iStart, s32 iEnd);
//            
//            void setModel(SVSkinModelPtr _model);
//            
//            SVSkinModelPtr getModel();
//            
//            void setstate(E_ANISTATE _state);
//            
//            E_ANISTATE getstate();
//            
//            void play();
//            
//            void play(cptr8 actname);
//            
//            void pause();
//            
//            void stop();
//            
//            void complete();
//            
//            void reset();
//            
//        protected:
//            void _updateRenderMesh(f32 dt);
//            
//            void _initMesh(s32 iMeshIndex, SVRenderMesh *pRenderMesh, aiMesh *pMeshData);
//            
//            struct stuAnimate{
//                s32 m_iIndex ;
//                s32 m_iStart;
//                s32 m_iEnd;
//            };
//            
//            typedef SVMap<s32, SVRenderMeshPtr> MESHMAP;
//            MESHMAP m_meshMap;
//            
//            typedef SVMap<s32, SVMtlSkinModelPtr> MATMAP;
//            MATMAP m_matMap;
//            
//            typedef SVMap<s32, SVMatSkinModelSelectPtr> MATSELECTMAP;
//            MATSELECTMAP m_matSelectMap;
//            
//            typedef SVMap<SVString , stuAnimate> SKINANIMATE;
//            SKINANIMATE m_skinAniMap;
//            
//            SVRenderObjectPtr m_renderObject;
//            
//            SVSkinModelPtr m_pModel;
//            
//            E_ANISTATE m_state;
//            
//            f32 m_fAniPlayedTime;
//            
//            s32 m_iCurrentAnimateIndex;
            
        };
        
    }//!namespace node
    
}//!namespace sv

//
#endif //SV_SKINNODE_H


