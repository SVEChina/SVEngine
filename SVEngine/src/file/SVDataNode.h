//
// SVDataNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DATANODE_H
#define SV_DATANODE_H

#include "SVDataBase.h"
#include "SVDataFilter.h"
#include "../base/SVMap.h"
#include "../core/SVVertDef.h"
#include "../rendercore/SVRenderDef.h"

namespace sv {
    
    namespace logic{

        //
        class SVDataNode : public SVDataBase{
        public:
            SVDataNode(SVInst *_app);
            
            ~SVDataNode();
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
            virtual SVNodePtr toNode();
            
            void refreshIn(SVNodePtr _node);
            
            void refreshOut(SVNodePtr _node);

            SVString m_name;
            
            //位置
            f32 m_posX;
            
            f32 m_posY;
            
            f32 m_posZ;
            //缩放
            f32 m_scaleX;
            
            f32 m_scaleY;
            
            f32 m_scaleZ;
            //旋转
            f32 m_rotX;
            
            f32 m_rotY;
            
            f32 m_rotZ;
            //绑定索引
            s32 m_bindIndex;
            
            s32 m_personID;
            //绑定偏移
            f32 m_offsetX;
            
            f32 m_offsetY;
            //渲染流
            RENDERSTREAMTYPE m_renderStreamType;
            //流中z顺序
            s32 m_zorder;
            //是否可被选择
            bool m_canSelect;
            //是否绘制包围盒
            bool m_drawAABB;
        protected:
            void _toJsonData(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &locationObj);
            void _fromJsonData(RAPIDJSON_NAMESPACE::Value &item);
            void _toNodeData(SVNodePtr _node);
        };
        
        //带变脸的背景节点
        class SVDataNodeDeform : public SVDataNode {
        public:
            SVDataNodeDeform(SVInst *_app);
            
            ~SVDataNodeDeform();
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
            virtual SVNodePtr toNode();
            
        protected:
            
        };
        
        //
        class  SVDataNodeFilter : public SVDataNode{
        public:
            SVDataNodeFilter(SVInst *_app);
            
            ~SVDataNodeFilter();
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
            virtual SVNodePtr toNode();
            
            inline SVDataFilterPtr getFilter(){
                return m_dataFilter;
            }
            
        protected:
            SVDataFilterPtr m_dataFilter;
        };
        
        class SVDataNodeBmpFont : public SVDataNode{
        public:
            SVDataNodeBmpFont(SVInst *_app);
            
            ~SVDataNodeBmpFont();
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
            virtual SVNodePtr toNode();
            
            void refreshIn(SVBitFontNodePtr _bitfontNode);
            
            void refreshOut(SVBitFontNodePtr _bitfontNode);
            
            SVString m_root_path;
            
            SVString m_file_name;
            
            s32 m_fontW;
            
            s32 m_fontH;
            
            s32 m_innerX;
            
            s32 m_innerY;
            
            SVString m_content;
            
            BITFONT_ATCH_PT m_type;
        };
        
        class SVDataNodeBackGround : public SVDataNode{
        public:
            SVDataNodeBackGround(SVInst *_app);
            
            ~SVDataNodeBackGround();
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
            virtual SVNodePtr toNode();
            
            void refreshIn(SVSpriteNodePtr _spriteNode);
            
            void refreshOut(SVSpriteNodePtr _spriteNode);
            
            SVBackGroundNodePtr m_spritePtr;
            
            SVString m_root_path;
            
            SVString m_file_name;
            
            s32 m_spriteW;
            
            s32 m_spriteH;
        protected:
            SVMap<u32, V2> m_pointMap;
        };
        
#ifdef CONFIG_IS_LOAD_FREETYPE
        
        class SVTTFNodeModelData : public SVDataNode{
        public:
            SVTTFNodeModelData(SVInst *_app);
            
            ~SVTTFNodeModelData();
            
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
            
            virtual SVNodePtr toNode();
            
            void refreshIn(SVFreeTypeNodePtr _ttfNode);
            
            void refreshOut(SVFreeTypeNodePtr _ttfNode);

        };
        
#endif

        
    }//!namespace logic
    
}//!namespace sv

#endif //SV_DATANODE_H
