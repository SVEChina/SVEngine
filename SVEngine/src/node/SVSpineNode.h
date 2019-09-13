//
// SVSpineNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SPINENODE_H
#define SV_SPINENODE_H

#include "SVNode.h"

struct spAttachment;
struct spSlot;
struct spBone;

//回调
typedef void (*sv_spine_callback)(SVSpineNodePtr _node,void* _obj,s32 _status);

namespace sv {
    
    namespace node{
        /*
         2D骨骼动画
         */
        class SVSpineNode : public SVNode {
        public:
            SVSpineNode(SVInst *_app);
            
            ~SVSpineNode();
            
            void update(f32 dt);
            
            void render();
            
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
            
            bool isImmediatelyPlay();//是否立即播放
            
            void setSpineCallback(sv_spine_callback _cb,void* _obj);
          
            bool getBonePosition(f32 &px, f32 &py, cptr8 bonename);
            
            bool getBoneScale(f32 &sx, f32 &sy, cptr8 bonename);
            
            bool getBoneRotation(f32 &rotation, cptr8 bonename);
            
            f32 getSlotAlpha(cptr8 bonename);
            
            void setAABBBoxScale(f32 _scale);
        protected:
            
            void _computeAABBBox();
            
            void _spine_start();
            
            void _spine_complete();
            
            void _spine_stop();//spine播完之后spine回调自动触发执行此函数
            
            void _sendAniEvent(cptr8 _eventName);
            
            SVSpinePtr m_spine;
            
            SVMultMeshMtlRenderObjectPtr m_pRObj;
            
            f32 m_aabbBox_scale;
            
            void* m_p_cb_obj;
            
            sv_spine_callback m_spine_callback;
            
            E_ANISTATE m_state;
            
            //序列化属性
            SVString m_cur_aniname;
    
            bool m_loop;
            
            bool m_immediatelyPlay;
            
        public:
            //序列化接口
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };
        
    }//!namespace node
    
}//!namespace sv



//
#endif //SV_SPINENODE_H


