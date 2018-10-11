//
// SVSpineNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SPINENODE_H
#define SV_SPINENODE_H

#include "SVNode.h"

class spAttachment;
struct spSlot;
struct spBone;

//回调
typedef void (*sv_spine_callback)(SVSpineNodePtr _node,void* _obj,s32 _status);

namespace sv {
    
    namespace node{
        //
        class SVSpineNode : public SVNode {
        public:
            SVSpineNode(SVInst *_app);
            
            ~SVSpineNode();
            
            void update(f32 dt);
            
            void render();
            
            virtual void setPosition(f32 x, f32 y, f32 z);
            
            virtual void setScale(f32 x, f32 y, f32 z);
            
            virtual void setPosition(FVec3& _pos);
            
            virtual void setScale(FVec3& _scale);
            
            virtual void setAlpha(f32 _alpha);
            
            void setSpine(SVSpinePtr _spine);
            
            SVSpinePtr getSpine();
            
            void clearSpine();
            
            void play(cptr8 _actname);
            
            void addAni(cptr8 _actname);
            
            void pause();
            
            void stop();
            
            void setstate(E_ANISTATE _state);
            
            E_ANISTATE getstate();
            
            cptr8 getCurAniName();
            
            void setCurAniName(cptr8 _name);
            
            void setloop(bool _loop);
            
            bool getloop();
            
            void setSpineCallback(sv_spine_callback _cb,void* _obj);
            
            bool getBonePosition(f32 &px, f32 &py, cptr8 bonename);
            
            bool getBoneScale(f32 &sx, f32 &sy, cptr8 bonename);
            
            f32 getSlotAlpha(cptr8 bonename);
            
            void setStableBoundingBoxScale(f32 _scale);
        protected:
            
            void _fixBoundingBox();
            
            void _spine_start();
            
            void _spine_complete();
            
            void _spine_stop();//spine播完之后spine回调自动触发执行此函数
            
            void _sendAniEvent(cptr8 _eventName);
            
            SVSpinePtr m_spine;
            
            SVMultMeshMtlRenderObjectPtr m_pRObj;
            
            E_ANISTATE m_state;
            
            bool m_loop;
            
            SVString m_cur_aniname;
            
            void* m_p_cb_obj;
            
            sv_spine_callback m_spine_callback;
            
            f32 m_box_scale;
            
            SVBoundBox m_stable_box;
            
            bool m_stable_box_dirty;
            
            bool m_update_stable_box;
        };
        
    }//!namespace node
    
}//!namespace sv



//
#endif //SV_SPINENODE_H


