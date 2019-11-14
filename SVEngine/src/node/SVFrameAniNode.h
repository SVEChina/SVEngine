//
// SVFrameAniNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FRAMEANI_NODE_H
#define SV_FRAMEANI_NODE_H

#include "SVNode.h"
#include "../base/SVMap.h"

namespace sv {
    
    namespace node{
        
        /*
         帧动画节点
         */
    
        class SVFrameAniNode : public SVNode {
        private:
            struct FrameTex {
                SVTexturePtr m_pTex;
                SVString m_pTexName;
                s32 m_index;
            };
            
        public:
            SVFrameAniNode(SVInst *_app);
            
            ~SVFrameAniNode();
            
            //
            void setSize(f32 _w,f32 _h);
            
            f32 getRelativeWidth();

            f32 getRelativeHeight();
            
            //更新
            virtual void update(f32 dt);

            //渲染
            virtual void render();
            
            //播放控制
            void play();
            
            void pause();
            
            void stop();
            
            void pushFrame(cptr8 _texname);
            
            void clearFrame();

            //序列化接口
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        protected:
            SVTexturePtr _selectTex(f32 _time);
            
            void _preload();    //预加载
            
            void _unload();     //卸载
            
            typedef SVArray<FrameTex> FRAMEPOOL;
            FRAMEPOOL m_framePool;
            
            f32 m_accTime;
            f32 m_totalTime;
            f32 m_frameRate;
            f32 m_width;
            f32 m_height;
            bool m_loop;
            bool m_preloadframe;        //预加载的帧数
            
            s32 m_curFrame; //当前帧
            s32 m_aimFrame; //目标帧
            s32 m_preFrame; //预先帧
            
            SVTexturePtr m_pActTex;
            SVRenderMeshPtr m_pMesh;
            SVRenderObjectPtr m_pRenderObj;
        };
        
    }//!namespace node
    
}//!namespace sv

//
#endif //SV_FRAMEANI_NODE_H


